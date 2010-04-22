//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "qmessagebox.h"

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include "qtwindowlistmenu.h"
#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qttreepropertybrowser.h"
#include <QtStringPropertyManager>

#include <QFileDialog>
#include <QDesktopServices>
#include <QTextStream>
#include <QUrl>

#include "simpleinputbox.hh"
#include "renderwindow.hh"
#include "quatschsourceeditor.hh"

#include "redshift/include/jobfile.hh"


namespace {

        // Some helper functions for the lack of a method in QtProperty
        // that lets us select sub-items directly by name.

        template <typename T>
        T readValue (QString name, QList<QtProperty*> list) {
                QtProperty* retty = 0;
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                retty = looky;
                                break;
                        }
                }
                if (0 != retty) {
                        const QtVariantProperty *v = (QtVariantProperty*)retty;
                        const QVariant vv = v->value();
                        const T ret = vv.value<T>();
                        return ret;
                }
                return T();
        }
        template <typename T>
        T readValue (QString name, QtProperty* prop) {
                return readValue<T> (name, prop->subProperties());
        }



        template <typename T>
        void writeValue (QString name, QList<QtProperty*> list, T const &val) {
                QtProperty* retty = 0;
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                retty = looky;
                                break;
                        }
                }
                if (0 != retty) {
                        QtVariantProperty *v = (QtVariantProperty*)retty;
                        v->setValue(val);
                }
        }
        template <typename T>
        void writeValue (QString name, QtProperty* prop, T const &val) {
                return writeValue<T> (name, prop->subProperties(), val);
        }



        QString readValueText (QString name, QList<QtProperty*> list) {
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                return looky->valueText();
                        }
                }
                return "";
        }
        QString readValueText (QString name, QtProperty *prop) {
                return readValueText(name, prop->subProperties());
        }


        QList<QtProperty*> readSubProperties (QString name, QList<QtProperty*> list) {
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                return looky->subProperties();
                        }
                }
                return QList<QtProperty*>();
        }

        QList<QtProperty*> readSubProperties (QString name, QtProperty* prop) {
                return readSubProperties(name, prop->subProperties());
        }


        QtProperty* readSubProperty (QString name, QList<QtProperty*> list) {
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                return looky;
                        }
                }
                return 0;
        }

        QtProperty* readSubProperty (QString name, QtProperty* prop) {
                return readSubProperty (name, prop->subProperties());
        }


        QtProperty *findParent(QtProperty *root, QtProperty* child) {
                if (root == child)
                        return 0;
                foreach (QtProperty *prop, root->subProperties()) {
                        if(prop == child)
                                return root;
                        if (QtProperty *n = findParent(prop, child))
                                return n;
                }
                return 0;
        }

        QtProperty *findParent(QList<QtProperty*> props, QtProperty* child) {
                foreach (QtProperty *prop, props) {
                        if (QtProperty *curr = findParent (prop, child))
                                return curr;
                }
                return 0;
        }

        void collapse (QtTreePropertyBrowser *browser,
                       QtProperty* property
        ) {
                foreach (QtBrowserItem *item, browser->items (property)) {
                        browser->setExpanded(item, false);
                }
        }



        ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c) {
                using redshift::scenefile::Color;

                ColorPickerColor ret;

                switch (c.type) {
                case Color::RGB:
                        ret.mode = ColorPickerColor::Tristimulus;
                        ret.tristimulus.setRedF(c.rgb.r);
                        ret.tristimulus.setGreenF(c.rgb.g);
                        ret.tristimulus.setBlueF(c.rgb.b);
                        break;
                case Color::Spectrum:
                        ret.mode = ColorPickerColor::Spectral;
                        for (size_t i=0; i<c.spectrum.samples.size(); ++i) {
                                SpectralSample ss;
                                ss.wavelength = c.spectrum.samples[i].wavelength;
                                ss.amplitude = c.spectrum.samples[i].amplitude;
                                ret.spectral.push_back(ss);
                        }
                }

                return ret;
        }

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pssSunSkyProperty(0),
    currentBrowserItem(0),
    nonRecurseLock(false),
    settingsContextMenu(this)
{
        setupUi();
        setDefaultScene();

        objectsMenu.addAction(ui->actionAdd_Horizon_Plane);
        objectsMenu.addAction(ui->actionAdd_Water_Plane);
        objectsMenu.addAction(ui->actionAdd_Lazy_Quadtree);
        objectsMenu.setTitle("new object");

        volumesMenu.addAction(ui->actionAdd_Homogeneous_Volume);
        volumesMenu.addAction(ui->actionAdd_Exponential_Volume);
        volumesMenu.setTitle("new volume");

        ui->settings->setRootIsDecorated(true);
        //ui->settings->setIndentation(32);
        ui->settings->setHeaderVisible(false);

        refreshWindowTitle();
        //menuBar()->repaint();
}



MainWindow::~MainWindow() {
        if (changed) {
                if (QMessageBox::Yes == QMessageBox::question(
                        this, "Unsaved data",
                        "You have unsaved changes. Save now?",
                        QMessageBox::Yes | QMessageBox::No)
                ) {
                        on_actionSave_as_triggered();
                }
        }
        delete ui;
}



void MainWindow::initializeScene() {
        ui->settings->clear();
        initializeFilmSettings();
        initializeRenderSettings();
        initializeCameraSettings();
        initializeObjects();
        initializeVolumes();
        initializeBackgrounds();
}



void MainWindow::setupUi() {
        changed = true;
        ui->setupUi(this);


        ui->actionShow_Command_Pile->setChecked(true);


        // code editor
        ui->codeEditor->setEnabled(false);
        ui->codeEditor->setVisible(false);

        // WindowList
        QtWindowListMenu *winMenu = new QtWindowListMenu(menuBar());
        winMenu->attachToMdiArea(ui->mdiArea);
        winMenu->setTitle("Windows");
        menuBar()->addMenu(winMenu);

        // Properties
        ui->settings->setAlternatingRowColors(false);

        variantManager = new QtVariantPropertyManager(this);
        connect (variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                 this, SLOT(variantManager_valueChanged(QtProperty*,QVariant)));

        rsTitleManager = new QtStringPropertyManager (this);
        connect(rsTitleManager, SIGNAL(valueChanged (QtProperty *, const QString &)),
                this, SLOT(rsTitleManager_valueChanged(QtProperty*,const QString &)));

        groupManager = new QtGroupPropertyManager(this);

        enumManager = new QtEnumPropertyManager(this);
        connect (enumManager, SIGNAL(valueChanged(QtProperty*,int)),
                 this, SLOT(enumManager_valueChanged(QtProperty*,int)));

        colorEditManager = new ColorEditManager (this);
        connect (colorEditManager, SIGNAL(valueChanged(QtProperty*,ColorPickerColor)),
                 this, SLOT(colorEditManager_valueChanged(QtProperty*,ColorPickerColor)));
        colorEditFactory = new ColorEditFactory (this, ui->mdiArea);

        transformEnumManager = new QtEnumPropertyManager(this);
        connect(transformEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(transformEnumManager_valueChanged(QtProperty*,int)));

        objectTypeEnumManager = new QtEnumPropertyManager(this);
        connect(objectTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(objectTypeEnumManager_valueChanged(QtProperty*,int)));

        volumeTypeEnumManager = new QtEnumPropertyManager(this);
        connect(volumeTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(volumeTypeEnumManager_valueChanged(QtProperty*,int)));

        surfaceIntegratorTypeEnumManager = new QtEnumPropertyManager(this);
        connect(surfaceIntegratorTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(surfaceIntegratorTypeEnumManager_valueChanged(QtProperty*,int)));

        cameraTypeEnumManager = new QtEnumPropertyManager(this);
        connect(cameraTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(cameraTypeEnumManager_valueChanged(QtProperty*,int)));

        codeEditManager = new QtVariantPropertyManager(this);
        connect(codeEditManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                this, SLOT(code_valueChanged(QtProperty*, QVariant)));

        comboBoxFactory = new QtEnumEditorFactory(this);
        lineEditFactory = new QtLineEditFactory(this);

        variantFactory = new QtVariantEditorFactory(this);
        ui->settings->setFactoryForManager(variantManager, variantFactory);
        ui->settings->setFactoryForManager(enumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(transformEnumManager, comboBoxFactory);
        //ui->settings->setFactoryForManager(objectTypeEnumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(cameraTypeEnumManager, comboBoxFactory);
        //ui->settings->setFactoryForManager(volumeTypeEnumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(surfaceIntegratorTypeEnumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(rsTitleManager, lineEditFactory);
        ui->settings->setFactoryForManager(colorEditManager, colorEditFactory);

        setUnchanged();
}



void MainWindow::setChanged() {
        if (changed) return;
        changed = true;
        ui->menuBar->setStyleSheet("background-color:#A33;");
        refreshWindowTitle();
}



void MainWindow::setUnchanged() {
        if (!changed) return;
        changed = false;
        ui->menuBar->setStyleSheet("");
        refreshWindowTitle();
}



void MainWindow::loadScene (redshift::scenefile::Scene const &scene) {
        initializeScene();
        setFilmSettings(scene.filmSettings());
        for (unsigned int i=0; i<scene.renderSettingsCount(); ++i)
                addRenderSettings(scene.renderSettings(i));
        for (unsigned int i=0; i<scene.cameraCount(); ++i)
                addCamera(scene.camera(i));
        for (unsigned int i=0; i<scene.objectCount(); ++i)
                addObject(scene.object(i));
        for (unsigned int i=0; i<scene.volumeCount(); ++i)
                addVolume(scene.volume(i));
        if (scene.backgroundCount())
                setBackground(scene.background(0));


        foreach (QtBrowserItem *it, ui->settings->topLevelItems())
                ui->settings->setExpanded(it, false);
}



void MainWindow::setDefaultScene() {
        changed = true;
        initializeScene();
        // - render settings
        {
                using namespace redshift::scenefile;

                RenderSettings rs;
                rs.width = 320;
                rs.height = 240;
                rs.min_y = 0;
                rs.max_y = 0xFFFFF;
                rs.samplesPerPixel = 1;
                rs.surfaceIntegrator.type = SurfaceIntegrator::whitted;
                rs.surfaceIntegrator.numAmbientSamples = 10;
                rs.userSeed = 0;
                rs.volumeIntegrator.type = VolumeIntegrator::none;
                rs.volumeIntegrator.cutoffDistance = 5000;
                rs.volumeIntegrator.stepSize = 400;
                rs.title = "preview";

                addRenderSettings(rs);
        }
        {
                using namespace redshift::scenefile;

                RenderSettings rs;
                rs.width = 800;
                rs.height = 600;
                rs.min_y = 0;
                rs.max_y = 0xFFFFF;
                rs.samplesPerPixel = 10;
                rs.surfaceIntegrator.type = SurfaceIntegrator::redshift;
                rs.surfaceIntegrator.numAmbientSamples = 10;
                rs.userSeed = 0;
                rs.volumeIntegrator.type = VolumeIntegrator::none;
                rs.volumeIntegrator.cutoffDistance = 50000;
                rs.volumeIntegrator.stepSize = 70;
                rs.title = "production";

                addRenderSettings(rs);
        }

        // - camera
        redshift::scenefile::Camera cam;
        cam.title = "hello-world";
        cam.type = redshift::scenefile::Camera::pinhole;
        cam.pinholeParams.front = 1;

        redshift::scenefile::Transform t;
        t.type = redshift::scenefile::Transform::move;
        t.y = 1;
        cam.transforms.push_back(t);
        t = redshift::scenefile::Transform();
        t.type = redshift::scenefile::Transform::yaw;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::pitch;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::roll;
        cam.transforms.push_back(t);

        addCamera(cam);

        // - objects
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        o.horizonPlaneParams.height = 0;
        o.horizonPlaneParams.material.color.type = redshift::scenefile::Color::RGB;
        o.horizonPlaneParams.material.color.rgb.r = 0.6;
        o.horizonPlaneParams.material.color.rgb.g = 0.5;
        o.horizonPlaneParams.material.color.rgb.b = 0.4;
        addObject(o);

        // - background
        setBackground (redshift::scenefile::Background());


        foreach (QtBrowserItem *it, ui->settings->topLevelItems())
                ui->settings->setExpanded(it, false);

        setUnchanged();
}



void MainWindow::initializeFilmSettings() {
        QtVariantProperty *item = 0;

        filmSettingsProperty = groupManager->addProperty(QLatin1String("film-settings"));

        // convert-to-srgb
        item = variantManager->addProperty(
                        QVariant::Bool,
                        QLatin1String("convert-to-srgb"));
        item->setValue(true);
        filmSettingsProperty->addSubProperty(item);

        // color-scale
        item = variantManager->addProperty(
                        QVariant::Double,
                        QLatin1String("color-scale"));
        item->setValue(0.007);
        item->setAttribute(QLatin1String("singleStep"), 0.01);
        item->setAttribute(QLatin1String("decimals"), 6);
        item->setAttribute(QLatin1String("minimum"), 0);
        item->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        filmSettingsProperty->addSubProperty(item);


        ui->settings->addProperty(filmSettingsProperty);

        ui->settings->setBackgroundColor(
                        ui->settings->topLevelItem(filmSettingsProperty),
                        QColor(130,110,90));
}
void MainWindow::setFilmSettings(redshift::scenefile::FilmSettings const &fs) {
        writeValue<bool>("convert-to-srgb", filmSettingsProperty, fs.convertToSrgb);
        writeValue<double>("color-scale", filmSettingsProperty, fs.colorscale);
}



void MainWindow::initializeRenderSettings() {
        QtProperty *topItem = groupManager->addProperty("render-settings");
        renderSettingsProperty = topItem;
        ui->settings->addProperty(topItem);

        ui->settings->setBackgroundColor(
                     ui->settings->topLevelItem(renderSettingsProperty),
                     QColor(130,90,90));
}



void MainWindow::initializeCameraSettings () {
        camerasProperty = groupManager->addProperty("cameras");
        ui->settings->addProperty(camerasProperty);
        ui->settings->setBackgroundColor(
                        ui->settings->topLevelItem(camerasProperty),
                        QColor(110,110,110));
}



void MainWindow::initializeObjects() {
        objectsProperty = groupManager->addProperty("objects");
        ui->settings->addProperty(objectsProperty);

        ui->settings->setBackgroundColor(
                        ui->settings->topLevelItem(objectsProperty),
                        QColor(90,130,90));
}



void MainWindow::initializeVolumes() {
        volumesProperty = groupManager->addProperty("volumes");
        ui->settings->addProperty(volumesProperty);

        ui->settings->setBackgroundColor(
                        ui->settings->topLevelItem(volumesProperty),
                        QColor(120,120,160));
}



void MainWindow::initializeBackgrounds () {
        backgroundsProperty = groupManager->addProperty("backgrounds");
        ui->settings->addProperty(backgroundsProperty);

        ui->settings->setBackgroundColor(
                        ui->settings->topLevelItem(backgroundsProperty),
                        QColor(90,90,130));
}



void MainWindow::setBackground (
        redshift::scenefile::Background const &b
) {
        pssSunSkyProperty = groupManager->addProperty("pss-sunsky");
        backgroundsProperty->addSubProperty(pssSunSkyProperty);

        // Sun Direction.
        QtProperty *sunDir = groupManager->addProperty("sun-direction");
        pssSunSkyProperty->addSubProperty(sunDir);

        // TODO: write custom property for vectors
        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.x);
        sunDir->addSubProperty(vp);

        vp= variantManager->addProperty(QVariant::Double, "up");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.y);
        sunDir->addSubProperty(vp);

        vp = variantManager->addProperty(QVariant::Double, "forward");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.z);
        sunDir->addSubProperty(vp);


        // Turbidity.
        vp = variantManager->addProperty(QVariant::Double, "turbidity");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.25);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 1.7);
        vp->setAttribute(QLatin1String("maximum"), 30);
        vp->setValue(b.turbidity);
        pssSunSkyProperty->addSubProperty(vp);

        // Sun size.
        vp = variantManager->addProperty(QVariant::Double, "sun-size-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 1.0);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunSizeFactor);
        pssSunSkyProperty->addSubProperty(vp);

        // Sun brightness factor.
        vp = variantManager->addProperty(QVariant::Double, "sun-brightness-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunBrightnessFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Atmosphere brightness factor.
        vp = variantManager->addProperty(QVariant::Double, "atmosphere-brightness-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.atmosphereBrightnessFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Atmosphere effects distance factor.
        vp = variantManager->addProperty(QVariant::Double, "atmospheric-effects-distance-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.atmosphericFxDistanceFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Overcast.
        vp = variantManager->addProperty(QVariant::Double, "overcast");
        vp->setValue(0);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), 1);
        vp->setValue(b.overcast);
        pssSunSkyProperty->addSubProperty(vp);

        // Atmospheric Effects.
        vp = variantManager->addProperty(QVariant::Bool, "atmospheric-effects");
        vp->setValue(b.atmosphericEffects ? 1 : 0);
        pssSunSkyProperty->addSubProperty(vp);

        collapse (ui->settings, pssSunSkyProperty);
}



redshift::scenefile::Color
MainWindow::readColor (
        QList<QtProperty*> subs, QString name
) const {
        QtProperty *color = readSubProperty (name, subs);
        if (!color) return redshift::scenefile::Color();
        const ColorPickerColor c = colorEditManager->value(color);

        redshift::scenefile::Color ret;

        switch (c.mode) {
        case ColorPickerColor::Spectral:
                ret.type = redshift::scenefile::Color::Spectrum;
                foreach (SpectralSample ss, c.spectral) {
                        redshift::scenefile::WavelengthAmplitudePair wap;
                        wap.wavelength = ss.wavelength;
                        wap.amplitude = ss.amplitude;
                        ret.spectrum.samples.push_back(wap);
                }
                break;
        case ColorPickerColor::Tristimulus:
                ret.type = redshift::scenefile::Color::RGB;
                QColor rgb = c.toQColor();
                ret.rgb = redshift::scenefile::Rgb(rgb.redF(),
                                                   rgb.greenF(),
                                                   rgb.blueF());
                break;
        }

        return ret;
}



redshift::scenefile::Material
MainWindow::readMaterial (
        QList<QtProperty*> subs, QString name
) const {
        QtProperty *material = readSubProperty(name, subs);
        if (!material) return redshift::scenefile::Material();

        redshift::scenefile::Material mat;
        mat.color = readColor (material->subProperties(), "color");
        return mat;
}



redshift::shared_ptr<redshift::scenefile::Scene>
        MainWindow::createScene () const
{
        typedef QList<QtProperty*> Props;
        typedef QtProperty* Prop;
        typedef QtVariantProperty* VProp;


        Props topProps = ui->settings->properties();

        QString ret;
        using namespace redshift;
        shared_ptr<scenefile::Scene> scene = shared_ptr<scenefile::Scene>(
                        new scenefile::Scene());

        // FilmSettings.
        const Props filmSettings = readSubProperties("film-settings", topProps);
        scenefile::FilmSettings fs;
        fs.colorscale = readValue<double>("color-scale", filmSettings);
        fs.convertToSrgb = readValue<bool>("convert-to-srgb", filmSettings);
        scene->setFilmSettings(fs);

        // RenderSettings.
        const Props renderSettings = readSubProperties("render-settings", topProps);
        if (renderSettings.count()==0) {
                throw std::runtime_error("No Render-Setting present.");
        }

        foreach (QtProperty *mooh, renderSettingsProperty->subProperties()) {
                Props subs = mooh->subProperties();

                scenefile::RenderSettings rs;
                rs.title = readValueText("title", subs).toStdString();
                rs.width = readValue<unsigned int>("width", subs);
                rs.height = readValue<unsigned int>("height", subs);
                rs.samplesPerPixel = readValue<unsigned int>("samples-per-pixel", subs);
                rs.min_y = readValue<unsigned int>("min-y", subs);
                rs.max_y = readValue<unsigned int>("max-y", subs);
                rs.userSeed = readValue<unsigned int>("seed", subs);

                // Surface Integrator.
                Props si = readSubProperties("surface-integrator", subs);
                const QString sintegT = readValueText("type", si);
                if("none" == sintegT) {
                        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::none;
                } else if("whitted" == sintegT) {
                        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::whitted;
                } else if("redshift" == sintegT) {
                        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::redshift;
                        rs.surfaceIntegrator.numAmbientSamples = readValue<int>("ambient-samples", si);
                } else throw std::runtime_error((QString() + "The surface-integrator '" + sintegT + "' "
                                             "is not supported. This is probably "
                                             "an oversight by the incapable "
                                             "programmers, please report this issue.").toStdString().c_str());

                // Volume Integrator.
                Props vp = readSubProperties("volume-integrator", subs);
                rs.volumeIntegrator.stepSize = readValue<double>("step-size", vp);
                rs.volumeIntegrator.cutoffDistance = readValue<double>("cutoff-distance", vp);
                const QString integT = readValueText("type", vp);
                if ("none" == integT)
                        rs.volumeIntegrator.type = scenefile::VolumeIntegrator::none;
                else if ("emission" == integT)
                        rs.volumeIntegrator.type = scenefile::VolumeIntegrator::emission;
                else if ("single" == integT)
                        rs.volumeIntegrator.type = scenefile::VolumeIntegrator::single;
                else throw std::runtime_error((QString() +
                                              "The volume-integrator '" + integT + "' "
                                              "is not supported. This is probably "
                                              "an oversight by the incapable "
                                              "programmers, please report this issue.").toStdString().c_str());

                scene->addRenderSettings(rs);
        }



        // Camera.
        const Props cameras = readSubProperties("cameras", topProps);
        if (cameras.count()==0) {
                throw std::runtime_error("No Camera present.");
        }

        foreach (QtProperty *cam, camerasProperty->subProperties()) {

                scenefile::Camera camera;
                camera.title = cam->propertyName().toStdString();

                const QString type = readValueText("type", cam);
                if (type == "pinhole") {
                        camera.type = scenefile::Camera::pinhole;
                        camera.pinholeParams.front = readValue<double>("front", cam);
                } else if (type == "cubemap_left") {
                        camera.type = scenefile::Camera::cubemap_left;
                } else if (type == "cubemap_right") {
                        camera.type = scenefile::Camera::cubemap_right;
                } else if (type == "cubemap_bottom") {
                        camera.type = scenefile::Camera::cubemap_bottom;
                } else if (type == "cubemap_top") {
                        camera.type = scenefile::Camera::cubemap_top;
                } else if (type == "cubemap_front") {
                        camera.type = scenefile::Camera::cubemap_front;
                } else if (type == "cubemap_back") {
                        camera.type = scenefile::Camera::cubemap_back;
                } else {
                        throw std::runtime_error((QString() +
                              "Unsupported camera type",
                              "The camera-type '" + type + "' "
                              "is not supported. This is probably "
                              "an oversight by the incapable "
                              "programmers, please report this issue."
                              ).toStdString().c_str());
                }

                const Props xforms = readSubProperties("transform", cam->subProperties());
                foreach (Prop xform, xforms) {
                        typedef scenefile::Transform Xf;
                        Xf transform;

                        // When tweaking here, also look at XCVBN
                        const Props xfsubs = xform->subProperties();
                        const QString type = readValueText("type", xfsubs);


                        if (type == "move") {
                                transform.type = Xf::move;
                                transform.x = readValue<double>("right", xfsubs);
                                transform.y = readValue<double>("up", xfsubs);
                                transform.z = readValue<double>("forward", xfsubs);
                        } else if (type == "move-left") {
                                transform.type = Xf::move_left;
                                transform.x = readValue<double>("distance", xfsubs);
                        } else if (type == "move-right") {
                                transform.type = Xf::move_right;
                                transform.x = readValue<double>("distance", xfsubs);
                        } else if (type == "move-up") {
                                transform.type = Xf::move_up;
                                transform.y = readValue<double>("distance", xfsubs);
                        } else if (type == "move-down") {
                                transform.type = Xf::move_down;
                                transform.y = readValue<double>("distance", xfsubs);
                        } else if (type == "move-forward") {
                                transform.type = Xf::move_forward;
                                transform.z = readValue<double>("distance", xfsubs);
                        } else if (type == "move-backward") {
                                transform.type = Xf::move_backward;
                                transform.z = readValue<double>("distance", xfsubs);
                        } else if (type == "yaw") {
                                transform.type = Xf::yaw;
                                transform.angle = readValue<double>("angle", xfsubs);
                        } else if (type == "pitch") {
                                transform.type = Xf::pitch;
                                transform.angle = readValue<double>("angle", xfsubs);
                        } else if (type == "roll") {
                                transform.type = Xf::roll;
                                transform.angle = readValue<double>("angle", xfsubs);
                        } else {
                                throw std::runtime_error((QString() +
                                                      "The transform-type '" + type + "' "
                                                      "is not supported. This is probably "
                                                      "an oversight by the incapable "
                                                      "programmers, please report this issue.").toStdString().c_str());
                        }
                        camera.transforms.push_back(transform);
                }

                scene->addCamera(camera);
        }


        // Objects.
        const Props objects = readSubProperties("objects", topProps);
        foreach (Prop object, objects) {
                using scenefile::Object;

                const Props subs = object->subProperties();
                const QString type = readValueText("type", subs);

                scenefile::Object object;

                if (type == "water-plane") {
                        object.type = Object::water_plane;
                        const std::string tmp = readValue<QString>("code", subs).toStdString();
                        if(!tmp.empty())
                                object.waterPlaneParams.code = tmp;
                        //object.waterPlaneParams.color
                        object.waterPlaneParams.height = readValue<double>("height", subs);
                        object.waterPlaneParams.material = readMaterial (subs);
                } else if (type == "horizon-plane") {
                        object.type = Object::horizon_plane;
                        //object.horizonPlaneParams.color
                        object.horizonPlaneParams.height = readValue<double>("height", subs);
                        object.horizonPlaneParams.material = readMaterial (subs);
                } else if (type == "lazy-quadtree") {
                        object.type = Object::lazy_quadtree;
                        const std::string tmp = readValue<QString>("code", subs).toStdString();
                        if(!tmp.empty())
                                object.lazyQuadtreeParams.code = tmp;
                        //object.lazyQuadtreeParams.color;
                        object.lazyQuadtreeParams.lodFactor = readValue<double>("lod-factor", subs);
                        object.lazyQuadtreeParams.maxRecursion = readValue<unsigned int>("max-recursion", subs);
                        object.lazyQuadtreeParams.size = readValue<double>("size", subs);
                        object.lazyQuadtreeParams.material = readMaterial (subs);

                } else {
                        throw std::runtime_error((QString() + "The object-type '" + type + "' "
                                              "is not supported. This is probably "
                                              "an oversight by the incapable "
                                              "programmers, please report this issue.").toStdString().c_str());
                }

                scene->addObject(object);

        }


        const Props volumes = readSubProperties("volumes", topProps);
        foreach (Prop volume, volumes) {
                using scenefile::Volume;

                const Props subs = volume->subProperties();
                const QString type = readValueText("type", subs);

                scenefile::Volume volume;

                volume.sigma_a = readColor(subs, "absorption");
                volume.sigma_s = readColor(subs, "out-scatter");
                volume.Lve     = readColor(subs, "emission");
                volume.hg      = readValue<double> ("phase-function", subs);

                // Common Properties.


                if (type == "homogeneous") {
                        volume.type = Volume::homogeneous;
                } else if (type == "exponential") {
                        volume.type = Volume::exponential;

                        Props v = readSubProperties("up", subs);
                        volume.up.x = readValue<double>("right", v);
                        volume.up.y = readValue<double>("up", v);
                        volume.up.z = readValue<double>("forward", v);

                        v = readSubProperties("min", subs);
                        volume.min.x = readValue<double>("right", v);
                        volume.min.y = readValue<double>("up", v);
                        volume.min.z = readValue<double>("forward", v);

                        volume.baseFactor = readValue<double>("base-factor", subs);
                        volume.exponentFactor = readValue<double>("exponent-factor", subs);
                        volume.epsilon = readValue<double>("epsilon", subs);
                } else {
                        throw std::runtime_error((QString() + "The volume-type '" + type + "' "
                                              "is not supported. This is probably "
                                              "an oversight by the incapable "
                                              "programmers, please report this issue.").toStdString().c_str());
                }

                scene->addVolume(volume);

        }


        // Background.
        if (pssSunSkyProperty) {
                scenefile::Background sunsky;
                const Props sunDir = readSubProperties("sun-direction", pssSunSkyProperty);

                sunsky.sunDirection.x = readValue<double>("right", sunDir);
                sunsky.sunDirection.y = readValue<double>("up", sunDir);
                sunsky.sunDirection.z = readValue<double>("forward", sunDir);
                sunsky.sunSizeFactor = readValue<double>("sun-size-factor", pssSunSkyProperty);
                sunsky.sunBrightnessFactor = readValue<double>("sun-brightness-factor", pssSunSkyProperty);

                sunsky.atmosphericEffects = readValue<bool>("atmospheric-effects", pssSunSkyProperty);
                sunsky.atmosphereBrightnessFactor = readValue<double>("atmosphere-brightness-factor", pssSunSkyProperty);
                sunsky.atmosphericFxDistanceFactor = readValue<double>("atmospheric-effects-distance-factor", pssSunSkyProperty);

                sunsky.turbidity = readValue<double>("turbidity", pssSunSkyProperty);
                sunsky.overcast = readValue<double>("overcast", pssSunSkyProperty);
                scene->addBackground(sunsky);
        }

        return scene;
}



void MainWindow::addRenderSettings (
        redshift::scenefile::RenderSettings const &rs
) {

        QtProperty *topItem = groupManager->addProperty(QString::fromStdString(rs.title));
        renderSettingsProperty->addSubProperty(topItem);

        QtProperty *title = rsTitleManager->addProperty("title");
        rsTitleManager->setRegExp(title, QRegExp("([a-z0-9]|-|_)+", Qt::CaseInsensitive, QRegExp::RegExp));
        rsTitleManager->setValue(title, QString::fromStdString(rs.title));
        topItem->addSubProperty(title);


        QtVariantProperty *it = variantManager->addProperty(QVariant::Int, "width");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.width);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "height");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.height);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "samples-per-pixel");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.samplesPerPixel);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "min-y");
        it->setAttribute(QLatin1String("minimum"), 0);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.min_y);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "max-y");
        it->setAttribute(QLatin1String("minimum"), 0);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.max_y);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "seed");
        it->setAttribute(QLatin1String("minimum"), 0);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(rs.userSeed);
        it->setToolTip("Use this to configure another random seed for rendering.");
        topItem->addSubProperty(it);


        {
                QtProperty *surfaceIntegrator = groupManager
                                                ->addProperty("surface-integrator");
                topItem->addSubProperty(surfaceIntegrator);

                QtProperty *integratorType = surfaceIntegratorTypeEnumManager->addProperty("type");
                QStringList enumNames;
                enumNames << "none" << "whitted" << "redshift";
                surfaceIntegratorTypeEnumManager->setEnumNames(integratorType, enumNames);

                surfaceIntegrator->addSubProperty(integratorType);


                switch (rs.surfaceIntegrator.type) {
                case redshift::scenefile::SurfaceIntegrator::none:
                        surfaceIntegratorTypeEnumManager->setValue(integratorType, 0);
                        surfaceIntegratorTypeEnumManager_valueChanged(integratorType, 0);
                        break;
                case redshift::scenefile::SurfaceIntegrator::whitted:
                        surfaceIntegratorTypeEnumManager->setValue(integratorType, 1);
                        surfaceIntegratorTypeEnumManager_valueChanged(integratorType, 1);
                        break;
                case redshift::scenefile::SurfaceIntegrator::redshift:
                        surfaceIntegratorTypeEnumManager->setValue(integratorType, 2);
                        surfaceIntegratorTypeEnumManager_valueChanged(integratorType, 2);
                        break;
                default:
                        QMessageBox::warning(this,
                          "Unsupported Surface Integrator",
                          "It has been tried to load a surface integrator "
                          "\""
                          + QString::fromStdString(redshift::scenefile::SurfaceIntegrator::Typenames[rs.surfaceIntegrator.type])
                          + "\", but this type is currently not supported here."
                        );
                        surfaceIntegratorTypeEnumManager->setValue(integratorType, 0);
                        surfaceIntegratorTypeEnumManager_valueChanged(integratorType, 0);
                }
        }
        {
                QtProperty *volumeIntegrator = groupManager->addProperty("volume-integrator");
                topItem->addSubProperty(volumeIntegrator);

                QtProperty *integratorType = enumManager->addProperty("type");
                QStringList enumNames;
                enumNames << "none" << "emission" << "single";
                enumManager->setEnumNames(integratorType, enumNames);
                volumeIntegrator->addSubProperty(integratorType);
                it = variantManager->addProperty(QVariant::Double, "step-size");
                it->setAttribute(QLatin1String("minimum"), 1.);
                //it->setAttribute(QLatin1String("maximum"), 32768.);
                it->setAttribute(QLatin1String("singleStep"), 1.);
                it->setAttribute(QLatin1String("decimals"), 1);
                it->setValue(rs.volumeIntegrator.stepSize);
                volumeIntegrator->addSubProperty(it);

                it = variantManager->addProperty(QVariant::Double, "cutoff-distance");
                it->setAttribute(QLatin1String("minimum"), 1.);
                //it->setAttribute(QLatin1String("maximum"), 32768.);
                it->setAttribute(QLatin1String("singleStep"), 1.);
                it->setAttribute(QLatin1String("decimals"), 1);
                it->setValue(rs.volumeIntegrator.cutoffDistance);
                volumeIntegrator->addSubProperty(it);


                switch (rs.volumeIntegrator.type) {
                case redshift::scenefile::VolumeIntegrator::none:
                        enumManager->setValue(integratorType, 0);
                        break;
                case redshift::scenefile::VolumeIntegrator::emission:
                        enumManager->setValue(integratorType, 1);
                        break;
                case redshift::scenefile::VolumeIntegrator::single:
                        enumManager->setValue(integratorType, 2);
                        break;
                default:
                        QMessageBox::warning(this,
                          "Unsupported Volume Integrator",
                          "It has been tried to load a volume integrator "
                          "\""
                          + QString::fromStdString(redshift::scenefile::VolumeIntegrator::Typenames[rs.volumeIntegrator.type])
                          + "\", but this type is currently not supported here."
                        );
                        enumManager->setValue(integratorType, 0);
                }
        }
        collapse (ui->settings, topItem);
        resyncRenderSettingConfig();
}



void MainWindow::resyncRenderSettingConfig () {
        ui->renderSettingConfig->clear();
        foreach (QtProperty *prop, renderSettingsProperty->subProperties()) {
                ui->renderSettingConfig->addItem(prop->propertyName());
        }
}



void MainWindow::resyncCameraConfig () {
        ui->cameraConfig->clear();
        foreach (QtProperty *prop, camerasProperty->subProperties()) {
                ui->cameraConfig->addItem(prop->propertyName());
        }
}



void MainWindow::addCamera(redshift::scenefile::Camera const& c) {
        using redshift::scenefile::Camera;

        QtProperty *camera = groupManager->addProperty(c.title.c_str());
        camerasProperty->addSubProperty(camera);

        QtProperty *cameraType = cameraTypeEnumManager->addProperty("type");
        QStringList enumNames;
        enumNames << "pinhole"
                  << "cubemap_left"
                  << "cubemap_right"
                  << "cubemap_bottom"
                  << "cubemap_top"
                  << "cubemap_front"
                  << "cubemap_back"
                  ;
        cameraTypeEnumManager->setEnumNames(cameraType, enumNames);
        camera->addSubProperty(cameraType);

        // TODO: should not be hardcoded, enumNames should directly correspond
        //       to redshift types
        switch (c.type) {
        case Camera::pinhole:
                cameraTypeEnumManager->setValue(cameraType, 0);
                cameraTypeEnumManager_valueChanged(cameraType, 0);
                writeValue("front", camera, c.pinholeParams.front);
                break;
        case Camera::cubemap_left:
                cameraTypeEnumManager->setValue(cameraType, 1);
                cameraTypeEnumManager_valueChanged(cameraType, 1);
                break;
        case Camera::cubemap_right:
                cameraTypeEnumManager->setValue(cameraType, 2);
                cameraTypeEnumManager_valueChanged(cameraType, 2);
                break;
        case Camera::cubemap_bottom:
                cameraTypeEnumManager->setValue(cameraType, 3);
                cameraTypeEnumManager_valueChanged(cameraType, 3);
                break;
        case Camera::cubemap_top:
                cameraTypeEnumManager->setValue(cameraType, 4);
                cameraTypeEnumManager_valueChanged(cameraType, 4);
                break;
        case Camera::cubemap_front:
                cameraTypeEnumManager->setValue(cameraType, 5);
                cameraTypeEnumManager_valueChanged(cameraType, 5);
                break;
        case Camera::cubemap_back:
                cameraTypeEnumManager->setValue(cameraType, 6);
                cameraTypeEnumManager_valueChanged(cameraType, 6);
                break;
        default:
                QMessageBox::warning(this,
                  "Unsupported Camera Type",
                  "It has been tried to set a camera type "
                  "\""
                  + QString::fromStdString(redshift::scenefile::Camera::Typenames[c.type])
                  + "\", but this type is currently not supported here."
                );
                cameraTypeEnumManager->setValue(cameraType, 0);
                cameraTypeEnumManager_valueChanged(cameraType, 0);
                break;
        };

        QtProperty *transformRoot = groupManager->addProperty("transform");
        camera->addSubProperty(transformRoot);

        collapse (ui->settings, camera);

        for (int i=0; i<c.transforms.size(); ++i) {
                addTransform (transformRoot, c.transforms[i]);
        }

        collapse (ui->settings, transformRoot);

        resyncCameraConfig();
}



void MainWindow::addTransform (QtProperty *transformRoot,
                               redshift::scenefile::Transform const & t) {
        typedef redshift::scenefile::Transform Xf;

        QtProperty *transform = groupManager->addProperty("---");
        transformRoot->addSubProperty(transform);


        QtProperty *transformType = transformEnumManager->addProperty("type");
        transform->addSubProperty(transformType);

        QStringList enumNames;
        int i = 0, def = 0;
        enumNames << "move";
        if (t.type == Xf::move) def = i; i++;
        enumNames << "move-left";
        if (t.type == Xf::move_left) def = i; i++;
        enumNames << "move-right";
        if (t.type == Xf::move_right) def = i; i++;
        enumNames << "move-up";
        if (t.type == Xf::move_up) def = i; i++;
        enumNames << "move-backward";
        if (t.type == Xf::move_backward) def = i; i++;
        enumNames << "move-forward";
        if (t.type == Xf::move_forward) def = i; i++;
        enumNames << "yaw";
        if (t.type == Xf::yaw) def = i; i++;
        enumNames << "pitch";
        if (t.type == Xf::pitch) def = i; i++;
        enumNames << "roll";
        if (t.type == Xf::roll) def = i; i++;
        transformEnumManager->setEnumNames(transformType, enumNames);
        transformEnumManager->setValue(transformType, def);

        switch (t.type) {
        case Xf::move:
                writeValue("right", transform, t.x);
                writeValue("up", transform, t.y);
                writeValue("forward", transform, t.z);
                break;
        case Xf::move_left: case Xf::move_right:
                writeValue("distance", transform, t.x);
                break;
        case Xf::move_down: case Xf::move_up:
                writeValue("distance", transform, t.y);
                break;
        case Xf::move_backward: case Xf::move_forward:
                writeValue("distance", transform, t.z);
                break;
        case Xf::yaw: case Xf::pitch: case Xf::roll:
                writeValue("angle", transform, t.angle);
                break;
        }
}



void MainWindow::addObject (redshift::scenefile::Object const &o) {
        QtProperty *object = groupManager->addProperty("---");
        objectsProperty->addSubProperty(object);

        QtProperty *objectType = objectTypeEnumManager->addProperty("type");
        object->addSubProperty(objectType);


        QStringList enumNames;
        enumNames << "horizon-plane"
                  << "water-plane"
                  << "lazy-quadtree"
                  ;
        objectTypeEnumManager->setEnumNames(objectType, enumNames);

        QtProperty *tmp;

        switch (o.type) {
        case redshift::scenefile::Object::horizon_plane:
                objectTypeEnumManager->setValue(objectType, 0);
                objectTypeEnumManager_valueChanged(objectType, 0);

                writeValue ("height", object, o.horizonPlaneParams.height);

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.horizonPlaneParams.material.color));
                break;
        case redshift::scenefile::Object::water_plane:
                objectTypeEnumManager->setValue(objectType, 1);
                objectTypeEnumManager_valueChanged(objectType, 1);

                writeValue ("height", object, o.waterPlaneParams.height);
                writeValue ("code", object, QString::fromStdString(o.waterPlaneParams.code));

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.waterPlaneParams.material.color));
                break;
        case redshift::scenefile::Object::lazy_quadtree:
                objectTypeEnumManager->setValue(objectType, 2);
                objectTypeEnumManager_valueChanged(objectType, 2);

                writeValue ("code", object, QString::fromStdString(o.lazyQuadtreeParams.code));
                writeValue ("max-recursion", object, o.lazyQuadtreeParams.maxRecursion);
                writeValue ("lod-factor", object, o.lazyQuadtreeParams.lodFactor);
                writeValue ("size", object, o.lazyQuadtreeParams.size);

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.lazyQuadtreeParams.material.color));
                break;
        }

        collapse (ui->settings, object);
}



void MainWindow::transformEnumManager_valueChanged(
        QtProperty* prop,
        int index
) {
        setChanged();
        if (QtProperty *t = findParent(ui->settings->properties(), prop)) {
                const QStringList enumNames =
                                transformEnumManager->enumNames(prop);
                const QString type = enumNames[index];

                t->setPropertyName(type);

                // Remove all properties not titled "type".
                foreach (QtProperty *nt, t->subProperties()){
                        if (nt->propertyName() != "type") {
                                t->removeSubProperty(nt);
                        }
                }

                // When tweaking here, also look at XCVBN
                if (type == "move") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"right"));
                        QtVariantProperty *tmp = variantManager->addProperty(QVariant::Double,"up");
                        tmp->setValue(1.);
                        t->addSubProperty(tmp);
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"forward"));
                } else if (type == "move-left") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-right") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-up") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-down") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-forward") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-backward") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "yaw") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else if (type == "pitch") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else if (type == "roll") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else QMessageBox::critical(this, "Unsupported object",
                                                  "The transform '" + type + "' "
                                                  "is not supported. This is probably "
                                                  "an oversight by the incapable "
                                                  "programmers, please report this issue.");

        }
}



void MainWindow::variantManager_valueChanged (QtProperty *, QVariant) {
        setChanged();
}



void MainWindow::enumManager_valueChanged (QtProperty *, int) {
        setChanged();
}



void MainWindow::colorEditManager_valueChanged(QtProperty *, ColorPickerColor) {
        setChanged();
}



void MainWindow::objectTypeEnumManager_valueChanged (
        QtProperty* prop,
        int index
) {
        setChanged();
        // parent
        //   |
        //   +-----type
        //   |
        //   +-----param*

        // We have 'type', now find 'parent'
        QtProperty *t = findParent(ui->settings->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames =
                        objectTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        t->setPropertyName(type);

        // Remove all properties not titled "type" to make place for the right
        // properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type") {
                        t->removeSubProperty(nt);
                }
        }

        if (type == "horizon-plane") {
                QtVariantProperty* it = variantManager->addProperty(QVariant::Double,"height");
                t->addSubProperty(it);
                it->setAttribute(QLatin1String("decimals"), 6);

                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);

                /*QtProperty *tit = variantManager->addProperty(QVariant::Color, "color");
                colorTerms->addSubProperty(tit);*/

                /*it->setAttribute(QLatin1String("color"), 6);*/
        } else if (type == "water-plane") {
                QtVariantProperty* height = variantManager->addProperty(QVariant::Double,"height");
                t->addSubProperty(height);
                height->setAttribute(QLatin1String("decimals"), 6);
                QtProperty* code = codeEditManager->addProperty(QVariant::String, "code");
                t->addSubProperty(code);

                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);
        } else if (type == "lazy-quadtree") {
                //t->addSubProperty(variantManager->addProperty(QVariant::UserType,"color"));

                QtProperty* code = codeEditManager->addProperty(QVariant::String, "code");
                t->addSubProperty(code);

                QtVariantProperty* maxrec = variantManager->addProperty(QVariant::Int,"max-recursion");
                maxrec->setToolTip("Be careful. Slowly approach values beyond 10. For preview renderings, values less than 10 seem to work well.");
                maxrec->setAttribute(QLatin1String("minimum"), 1);
                maxrec->setAttribute(QLatin1String("maximum"), 20);
                maxrec->setValue(7);
                t->addSubProperty(maxrec);

                QtVariantProperty* lodfac = variantManager->addProperty(QVariant::Double,"lod-factor");
                lodfac->setToolTip("Very sensible factor. The smaller, the less detail-loss upon increasing distance.");
                lodfac->setAttribute(QLatin1String("minimum"), 0.000001);
                lodfac->setAttribute(QLatin1String("maximum"), 1);
                lodfac->setAttribute(QLatin1String("singleStep"), 0.00001);
                lodfac->setAttribute(QLatin1String("decimals"), 6);
                lodfac->setValue(0.000125);
                t->addSubProperty(lodfac);

                QtVariantProperty* size = variantManager->addProperty(QVariant::Double,"size");
                lodfac->setToolTip("Picogen's quadtree scales quite well. Feel free to try out very large values of 100k and bigger.");
                size->setAttribute(QLatin1String("minimum"), 1);
                size->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                size->setAttribute(QLatin1String("singleStep"), 1000);
                size->setAttribute(QLatin1String("decimals"), 1);
                size->setValue(50000);
                t->addSubProperty(size);


                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);

                //t->addSubProperty(variantManager->addProperty(QVariant::V,"color"));
        } else {
                QMessageBox::critical(this, "Unsupported object",
                                      "The object-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void MainWindow::addVolume (redshift::scenefile::Volume const &v) {
        QtProperty *volume = groupManager->addProperty("---");
        volumesProperty->addSubProperty(volume);

        QtProperty *volumeType = volumeTypeEnumManager->addProperty("type");
        volume->addSubProperty(volumeType);


        QStringList enumNames;
        enumNames << "homogeneous"
                  << "exponential"
                  ;
        volumeTypeEnumManager->setEnumNames(volumeType, enumNames);

        QtProperty *tmp;

        switch (v.type) {
        case redshift::scenefile::Volume::homogeneous:
                volumeTypeEnumManager->setValue(volumeType, 0);
                volumeTypeEnumManager_valueChanged(volumeType, 0);
                break;
        case redshift::scenefile::Volume::exponential:
                volumeTypeEnumManager->setValue(volumeType, 1);
                volumeTypeEnumManager_valueChanged(volumeType, 1);

                tmp = readSubProperty("up", volume);
                writeValue ("right",   tmp, v.up.x);
                writeValue ("up",      tmp, v.up.y);
                writeValue ("forward", tmp, v.up.z);

                tmp = readSubProperty("min", volume);
                writeValue ("right",   tmp, v.min.x);
                writeValue ("up",      tmp, v.min.y);
                writeValue ("forward", tmp, v.min.z);

                writeValue ("base-factor", volume, v.baseFactor);
                writeValue ("exponent-factor", volume, v.exponentFactor);
                writeValue ("epsilon", volume, v.epsilon);
                break;
        }

        tmp = readSubProperty("absorption", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.sigma_a));
        tmp = readSubProperty("out-scatter", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.sigma_s));
        tmp = readSubProperty("emission", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.Lve));
        writeValue ("phase-function", volume, v.hg);

        collapse (ui->settings, volume);
}



void MainWindow::volumeTypeEnumManager_valueChanged (
        QtProperty* prop,
        int index
) {
        setChanged();
        QtProperty *t = findParent(ui->settings->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames =
                        volumeTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        t->setPropertyName(type);

        // Remove all properties not titled "type" to make place for the right
        // properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type") {
                        t->removeSubProperty(nt);
                }
        }


        // Common.
        t->addSubProperty(colorEditManager->addProperty("absorption"));
        t->addSubProperty(colorEditManager->addProperty("out-scatter"));
        t->addSubProperty(colorEditManager->addProperty("emission"));

        QtVariantProperty* hg = variantManager->addProperty(QVariant::Double,"phase-function");
        hg->setAttribute(QLatin1String("minimum"), -1);
        hg->setAttribute(QLatin1String("maximum"), 1);
        hg->setAttribute(QLatin1String("singleStep"), 0.1);
        hg->setAttribute(QLatin1String("decimals"), 5);
        hg->setValue(0);
        t->addSubProperty(hg);


        if (type == "homogeneous") {

        } else if (type == "exponential") {
                {
                        QtProperty *upVector = groupManager->addProperty("up");
                        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        upVector->addSubProperty(vp);

                        vp= variantManager->addProperty(QVariant::Double, "up");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(1);
                        upVector->addSubProperty(vp);

                        vp = variantManager->addProperty(QVariant::Double, "forward");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        upVector->addSubProperty(vp);

                        t->addSubProperty(upVector);
                }
                {
                        QtProperty *minVector = groupManager->addProperty("min");
                        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        minVector->addSubProperty(vp);

                        vp= variantManager->addProperty(QVariant::Double, "up");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(1);
                        minVector->addSubProperty(vp);

                        vp = variantManager->addProperty(QVariant::Double, "forward");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        minVector->addSubProperty(vp);

                        t->addSubProperty(minVector);
                }

                QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "base-factor");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);

                vp = variantManager->addProperty(QVariant::Double, "exponent-factor");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);

                vp = variantManager->addProperty(QVariant::Double, "epsilon");
                vp->setToolTip("The minimum desired density. This can then be used by picogen "
                               "for calculating the boundary and thus enable optimiziations.");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);
        } else {
                QMessageBox::critical(this, "Unsupported volume",
                                      "The volume-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void MainWindow::surfaceIntegratorTypeEnumManager_valueChanged(
        QtProperty* prop, int index
){
        setChanged();
        QtProperty *t = findParent(ui->settings->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames =
                        surfaceIntegratorTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        // Remove all properties not titled "type" to make place for the right
        // properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type") {
                        t->removeSubProperty(nt);
                }
        }

        if (type == "none") {
                // has no options.
        } else if (type == "redshift") {
                QtVariantProperty* numSamples = variantManager->addProperty(QVariant::Int,"ambient-samples");
                numSamples->setToolTip("Low values give a noisy look, but yield faster renderings.");
                numSamples->setAttribute(QLatin1String("minimum"), 1);
                numSamples->setAttribute(QLatin1String("maximum"), 0xFFFF);
                numSamples->setValue(10);
                t->addSubProperty(numSamples);
        } else if (type == "whitted") {
                // has no options.
        } else {
                QMessageBox::critical(this, "Unsupported object",
                                      "The object-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void MainWindow::cameraTypeEnumManager_valueChanged(
        QtProperty* prop, int index
) {
        setChanged();
        QtProperty *t = findParent(ui->settings->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames = cameraTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        // Remove all properties not titled "type"/"transform" to make place for
        // the right properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type"
                    && nt->propertyName() != "transform"
                ) {
                        t->removeSubProperty(nt);
                }
        }

        if (type == "pinhole") {
                QtVariantProperty* front = variantManager->addProperty(QVariant::Double,"front");
                front->setAttribute(QLatin1String("minimum"), 0.0001);
                front->setAttribute(QLatin1String("singlestep"), 0.1);
                front->setValue(1.);
                t->addSubProperty(front);
        } else if (type == "cubemap_left"
                   || type == "cubemap_right"
                   || type == "cubemap_bottom"
                   || type == "cubemap_top"
                   || type == "cubemap_front"
                   || type == "cubemap_back"
        ) {
                // do nothing
        } else {
                QMessageBox::critical(this, "Unsupported camera type",
                                      "The camera-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void MainWindow::rsTitleManager_valueChanged (
        QtProperty * property,
        const QString & value
) {
        setChanged();
        if (QtProperty *par = findParent (ui->settings->properties(), property)) {
                par->setPropertyName(value);
        }
        resyncRenderSettingConfig();
}



void MainWindow::changeEvent(QEvent *e) {
        QMainWindow::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void MainWindow::on_settings_currentItemChanged(QtBrowserItem * current) {

        settingsContextMenu.clear();
        settingsContextMenu.addAction(ui->actionNew_Render_Setting);
        settingsContextMenu.addAction(ui->actionNew_Camera);        
        settingsContextMenu.addMenu(&objectsMenu);
        settingsContextMenu.addMenu(&volumesMenu);
        settingsContextMenu.addSeparator();

        this->currentBrowserItem = current;
        QString name = (current==0) ? "" : current->property()->propertyName();

        QtProperty *parentProp = (current==0)
                               ? 0
                               : findParent(ui->settings->properties(),
                                            current->property());
        QtProperty *parentParentProp = (parentProp==0)
                               ? 0
                               : findParent(ui->settings->properties(),
                                            parentProp);

        // TODO: below are a bit fragile, but for now work very well.

        const bool isCode = name == "code";

        if ((parentProp != 0)
            && (parentProp->propertyName() == "objects")
        ) {
                ui->deleteObjectButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Object);
        } else {
                ui->deleteObjectButton->setEnabled(false);
        }

        if ((parentProp != 0)
             && (parentProp->propertyName() == "volumes")
        ) {
                ui->deleteVolumeButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Volume);
        } else {
                ui->deleteVolumeButton->setEnabled(false);
        }


        const bool isTransform      = name  == "transform";
        const bool isSubTransform   = (parentProp != 0)
                                      && (parentProp->propertyName()
                                          == "transform");
        const bool isCamera         = (parentProp != 0)
                                      && (parentProp == camerasProperty);
        const bool isRenderSetting  = (parentProp != 0)
                                      && (parentProp == renderSettingsProperty);

        currentCameraProperty        = 0;
        currentTransformProperty     = 0;
        currentRenderSettingProperty = 0;

        if (isSubTransform) {
                currentCameraProperty    = parentParentProp;
                currentTransformProperty = parentProp;
        }
        if (isTransform) {
                currentCameraProperty    = parentProp;
                currentTransformProperty = current->property();
        }
        if (isCamera) {
                currentCameraProperty    = current->property();
        }
        if (isRenderSetting) {
                currentRenderSettingProperty = current->property();
        }

        if (isCode) {
                ui->codeEditor->setEnabled(true);
                ui->codeEditor->setVisible(true);
                ui->codeEditor->setCode(codeEditManager->value(current->property()).toString());
        } else {
                ui->codeEditor->setEnabled(false);
                ui->codeEditor->setVisible(false);
        }

        if (currentTransformProperty != 0) {
                ui->newSubTransformButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionNew_Sub_Transform);
        } else {
                ui->newSubTransformButton->setEnabled(false);
        }
        if (isSubTransform) {
                ui->deleteSubTransformButton->setEnabled (true);
                settingsContextMenu.addAction(ui->actionDelete_Sub_Transform);
        } else {
                ui->deleteSubTransformButton->setEnabled (false);
        }


        if (isRenderSetting) {
                ui->deleteRsButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Render_Setting);
        } else {
                ui->deleteRsButton->setEnabled(false);
        }


        if (isCamera) {
                ui->deleteCameraButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Camera);
        } else {
                ui->deleteCameraButton->setEnabled(false);
        }
}



void MainWindow::on_actionShow_redshift_job_code_triggered() {
        using namespace actuarius;
        using namespace redshift;

        try {
                const shared_ptr<scenefile::Scene> scene = createScene ();
                std::stringstream ss;
                OArchive (ss) & pack("scene", *scene);
                QMessageBox::information(this, QString("Teh codes"), ss.str().c_str());
        } catch (std::exception const &e) {
                QMessageBox::critical(this, "Critical",
                                      e.what());
        }
}



void MainWindow::on_actionRender_triggered() {
        render();
}



void MainWindow::on_newRsButton_clicked() {
        on_actionNew_Render_Setting_triggered();
}



void MainWindow::on_newWaterPlaneButton_clicked() {
        on_actionAdd_Water_Plane_triggered();
}
void MainWindow::on_newHorizonPlaneButton_clicked() {
        on_actionAdd_Horizon_Plane_triggered();
}
void MainWindow::on_newLazyQuadtreeButton_clicked() {
        on_actionAdd_Lazy_Quadtree_triggered();
}



void MainWindow::on_newExponentialVolumeButton_clicked() {
        on_actionAdd_Exponential_Volume_triggered();
}
void MainWindow::on_newHomogeneousVolumeButton_clicked() {
        on_actionAdd_Homogeneous_Volume_triggered();
}



void MainWindow::on_codeEditor_codeChanged() {
        // We assume that this can only be triggered if the current item
        // is some code.
        if (nonRecurseLock)
                return;
        nonRecurseLock = true;
        codeEditManager->setValue(currentBrowserItem->property(),
                                  ui->codeEditor->code());
        nonRecurseLock = false;
}



void MainWindow::code_valueChanged(QtProperty*, QVariant code) {
        setChanged();
        // This is under the assumption that the code window is open for the
        // property that just emitted this signal.
        if (nonRecurseLock)
                return;
        nonRecurseLock = true;
        ui->codeEditor->setCode(code.toString());
        nonRecurseLock = false;
}



void MainWindow::on_deleteObjectButton_clicked() {
        on_actionDelete_Object_triggered();
}



void MainWindow::on_deleteVolumeButton_clicked() {
        on_actionDelete_Volume_triggered();
}



void MainWindow::on_deleteSubTransformButton_clicked() {
        on_actionDelete_Sub_Transform_triggered();
}



void MainWindow::on_deleteRsButton_clicked() {
        on_actionDelete_Render_Setting_triggered();
}



void MainWindow::on_newSubTransformButton_clicked() {
        on_actionNew_Sub_Transform_triggered();
}



void MainWindow::on_newCameraButton_clicked() {
        on_actionNew_Camera_triggered();
}



void MainWindow::on_deleteCameraButton_clicked() {
        on_actionDelete_Camera_triggered();
}



QString MainWindow::askForNewSaveFilename() {
        again:

        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setWindowTitle("Set a filename for saving the scene");

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (dialog.exec()) {
                QString const name = dialog.selectedFiles()[0];

                // Check if overwrites.
                if (QFile::exists(name) &&
                    QMessageBox::question(this, "Overwrite file?",
                                          QString()+
                                          "Do you really want to overwrite the file "
                                          + "\"" + name + "\"?",
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No
                                          ) == QMessageBox::No
                ) {
                        goto again;
                }

        /*QString name = QFileDialog::getSaveFileName(this, "Select a file to save to");
        if (name != "") {*/

                // Check if we can write.
                QFile file (name);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QMessageBox::warning(this, "Failed to save",
                             "The file \"" + file.fileName() + "\" could not be"
                             +" opened for writing. Please select another file "
                             +" or one that does not exist yet.");
                        goto again;
                }
                return name;
        }
        return "";
}



QString MainWindow::getAndUpdateSaveFilename() {
        if (saveFilename == "") {
                saveFilename = askForNewSaveFilename();
                return saveFilename;
        }
        return saveFilename;
}



void MainWindow::on_action_Save_triggered() {
        QString code = sceneToCode();
        QString file = getAndUpdateSaveFilename();
        if (file == "")
                return;

        std::ofstream ofs (file.toStdString().c_str());
        if (ofs.is_open()) {
                ofs << code.toStdString() << std::endl;
        } else {
                QMessageBox::warning(this, "Must use another filename",
                                     "Cannot save the file as \""
                                     + saveFilename + "\", please use "
                                     "another name.");
                on_actionSave_as_triggered();
        }

        setUnchanged();
        refreshWindowTitle();
}



void MainWindow::on_actionSave_as_triggered() {
        again:
        QString code = sceneToCode();
        QString newName = askForNewSaveFilename();
        if (newName == "") {
                return;
        }

        std::ofstream ofs (newName.toStdString().c_str());
        if (ofs.is_open()) {
                ofs << code.toStdString() << std::endl;
                saveFilename = newName;
        } else {
                QMessageBox::warning(this, "Must use another filename",
                                     "Cannot save the file as \""
                                     + saveFilename + "\", please use "
                                     "another name.");
                goto again;
        }

        setUnchanged();
}



void MainWindow::on_actionSave_copy_as_triggered() {
        QString code = sceneToCode();
        QString file = askForNewSaveFilename();
        if (file == "")
                return;

        //QTextStream out(&file, QIODevice::WriteOnly);
        //out << code;
        std::ofstream ofs (file.toStdString().c_str());
        if (ofs.is_open()) {
                ofs << code.toStdString() << std::endl;
        } else {
                QMessageBox::warning(this, "Must use another filename",
                                     "Cannot save the file as \""
                                     + saveFilename + "\", please use "
                                     "another name.");
        }
}



void MainWindow::refreshWindowTitle() {
        const QString alpha = saveFilename == "" ? "<new scene>" : saveFilename;
        setWindowTitle (
                alpha + (changed ? " * " : "")
                + " - picogen:SimplexGui"
                );
}



QString MainWindow::sceneToCode() {
        QString tehCodes;
        try {
                using namespace actuarius;
                using namespace redshift;

                const shared_ptr<scenefile::Scene> scene = createScene ();
                std::stringstream ss;
                OArchive (ss) & pack("scene", *scene);
                return QString::fromStdString(ss.str());
        } catch (std::exception const &e) {
                QMessageBox::critical(this, "Critical",
                                      e.what());
                return "";
        }
}



void MainWindow::on_renderButton_clicked() {
        render();
}



void MainWindow::render() {
        try {
                const int
                      renderSettings = ui->renderSettingConfig->currentIndex(),
                      camera = ui->cameraConfig->currentIndex();

                RenderWindow *rw = new RenderWindow (
                                createScene(),
                                renderSettings, camera,
                                this);
                ui->mdiArea->addSubWindow(rw);
                rw->show();
        } catch (std::exception const &ex) {
                QMessageBox::critical(this, "Error", QString()+
                                      "An exception occured:\n\n"
                                      + ex.what());
        }
}



void MainWindow::on_actionLoad_triggered() {
        redshift::scenefile::Scene scene;
        redshift::scenefile::Scene oldScene = *createScene();

        // I somewhat find the OS' own file dialog to be disturbing
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setWindowTitle("Select a file to load");

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (!dialog.exec()) {
                return;
        }

        const QString name = dialog.selectedFiles()[0];

        if (!QFile::exists(name)) {
                QMessageBox::critical(this,
                      "Error upon loading",
                      "The file \"" + name + "\" does not exist."
                );
                return;
        }

        try {
                std::ifstream ss(name.toStdString().c_str());
                actuarius::IArchive (ss) & actuarius::pack("scene", scene);
                loadScene (scene);
                saveFilename = name;
                refreshWindowTitle();
        } catch (std::exception const &e){
                loadScene(oldScene);
                QMessageBox::critical(this,
                      "Error upon loading",
                      "The selected file \"" + name + "\" could not be loaded, "
                      "are you sure this is a valid picogen file?"
                      );
        }

        setUnchanged();
}



void MainWindow::on_actionProduction_Render_triggered() {
        try {
                again:
                if (saveFilename == "" || !QFile::exists(saveFilename)) {
                        if (QMessageBox::Ok ==
                                QMessageBox::question(this, "Unsaved scene",
                                  "To do a production-render, the scene must be "
                                  "saved first.",
                                  QMessageBox::Ok | QMessageBox::Cancel,
                                  QMessageBox::Ok
                                )
                        ) {
                                on_action_Save_triggered();
                                goto again;
                        }
                        return;
                }
                const int
                      renderSettings = ui->renderSettingConfig->currentIndex(),
                      camera = ui->cameraConfig->currentIndex();

                RenderWindow::RenderProcess (
                                saveFilename,
                                renderSettings, camera);
        } catch (std::exception const &ex) {
                QMessageBox::critical(this, "Error", QString()+
                                      "An exception occured:\n\n"
                                      + ex.what());
        }
}



void MainWindow::on_settings_customContextMenuRequested(QPoint pos) {
        settingsContextMenu.exec(ui->settings->mapToGlobal(pos));
}



void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
        /*QMenu menu(this);
        menu.addAction(ui->actionLoad);
        menu.exec(event->globalPos());*/
}



void MainWindow::on_actionDelete_Render_Setting_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        renderSettingsProperty->removeSubProperty(currentRenderSettingProperty);
        resyncRenderSettingConfig();
}
void MainWindow::on_actionNew_Render_Setting_triggered() {
        setChanged();
        redshift::scenefile::RenderSettings rs;
        rs.title = "new-setting";
        addRenderSettings (rs);
}



void MainWindow::on_actionAdd_Homogeneous_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::homogeneous;
        addVolume (v);
}
void MainWindow::on_actionAdd_Exponential_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::exponential;
        addVolume (v);
}
void MainWindow::on_actionDelete_Volume_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        volumesProperty->removeSubProperty(currentBrowserItem->property());
}



void MainWindow::on_actionNew_Camera_triggered() {
        setChanged();
        redshift::scenefile::Camera cam;
        cam.title = "new-camera";
        cam.type = redshift::scenefile::Camera::pinhole;
        cam.pinholeParams.front = 1;

        redshift::scenefile::Transform t;
        t.type = redshift::scenefile::Transform::move;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::yaw;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::pitch;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::roll;
        cam.transforms.push_back(t);

        addCamera(cam);
        resyncCameraConfig();
}
void MainWindow::on_actionDelete_Camera_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        camerasProperty->removeSubProperty(currentCameraProperty);
        resyncCameraConfig();
}



void MainWindow::on_actionNew_Sub_Transform_triggered() {
        setChanged();
        // We assume that newTransform can only clicked when the current-item
        // is a transform.
        addTransform (currentTransformProperty,
                      redshift::scenefile::Transform());
}
void MainWindow::on_actionDelete_Sub_Transform_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        currentTransformProperty->removeSubProperty(
                        currentBrowserItem->property());
}



void MainWindow::on_actionAdd_Water_Plane_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::water_plane;
        addObject (o);
}
void MainWindow::on_actionAdd_Horizon_Plane_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        addObject (o);
}
void MainWindow::on_actionAdd_Lazy_Quadtree_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::lazy_quadtree;
        addObject (o);
}
void MainWindow::on_actionDelete_Object_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        objectsProperty->removeSubProperty(currentBrowserItem->property());
}



void MainWindow::on_actionShow_Command_Pile_triggered(bool checked) {
        ui->commandPileGroupBox->setEnabled(checked);
        ui->commandPileGroupBox->setVisible(checked);
}
