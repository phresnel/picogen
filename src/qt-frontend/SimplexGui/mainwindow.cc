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
#include <QProcess>
#include <QKeyEvent>

#include "simpleinputbox.hh"
#include "renderwindow.hh"
#include "quatschsourceeditor.hh"
#include "OpenSceneFile/openscenefile.hh"

#include "redshift/include/jobfile.hh"

#include "propertybrowser-helpers.hh"

#include "objectpropertybrowser.hh"
#include "volumepropertybrowser.hh"
#include "rendersettingspropertybrowser.hh"
#include "cameraspropertybrowser.hh"
#include "filmsettingspropertybrowser.hh"


// TODO: this should be in a header
ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c) {
        using redshift::scenefile::Color;

        ColorPickerColor ret;

        switch (c.type) {
        case Color::RGB:
                ret.mode = ColorPickerColor::Tristimulus;
                ret.tristimulus = TristimulusColor(
                                (double)c.rgb.r,
                                (double)c.rgb.g,
                                (double)c.rgb.b
                                );
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



MainWindow::MainWindow(
                QString openFilename,
                QString initialFilename,
                QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pssSunSkyProperty(0),
    currentBrowserItem(0),
    nonRecurseLock(false),
    settingsContextMenu(this)
{
        setupUi();

        connect(ui->codeEditor, SIGNAL(helpBrowserVisibilityRequested()),
                this, SLOT(helpBrowserVisibilityRequested()));

        if (openFilename != "") {
                try {
                        redshift::scenefile::Scene scene;
                        std::ifstream ss(initialFilename.toStdString().c_str());
                        actuarius::IArchive (ss) & actuarius::pack("scene", scene);
                        loadScene(scene);

                        saveFilename = openFilename;
                        QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
                        refreshWindowTitle();

                        //if (openFilename == initialFilename)
                                setUnchanged();
                        /*else
                                setChanged();*/
                } catch (...) {
                        setDefaultScene();
                }
        } else {
                setDefaultScene();
        }

        objectsMenu.addAction(ui->actionAdd_Horizon_Plane);
        objectsMenu.addAction(ui->actionAdd_Water_Plane);
        objectsMenu.addAction(ui->actionAdd_Lazy_Quadtree);
        objectsMenu.setTitle("New Object");

        volumesMenu.addAction(ui->actionAdd_Homogeneous_Volume);
        volumesMenu.addAction(ui->actionAdd_Exponential_Volume);
        volumesMenu.setTitle("New Volume");

        ui->settings->setRootIsDecorated(true);
        //ui->settings->setIndentation(32);
        ui->settings->setHeaderVisible(false);
        ui->settings->installEventFilter(this);

        refreshWindowTitle();
        //menuBar()->repaint();

        ui->actionShow_Command_Pile->setChecked(false);
        on_actionShow_Command_Pile_triggered (ui->actionShow_Command_Pile->isChecked());

        ui->actionShow_Picohelp_Browser->setChecked(true);
        on_actionShow_Picohelp_Browser_triggered (ui->actionShow_Picohelp_Browser->isChecked());

        ui->codeEditor->setPicohelpBrowser(ui->picohelp);
}



MainWindow::~MainWindow() {
        delete ui;
}



void MainWindow::initializeScene() {
        ui->settings->clear();
        ui->settings->setProperty("picohelp", "SimplexGUI_Property_Editor.html");

        filmSettingsPropertyBrowser = new FilmSettingsPropertyBrowser(
                        this,
                        ui->settings
                        );
        connect(filmSettingsPropertyBrowser, SIGNAL(updateUi()), this, SLOT(updateUi()));
        connect(filmSettingsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(setChanged()));

        renderSettingsPropertyBrowser = new RenderSettingsPropertyBrowser(
                        this,
                        ui->settings,
                        groupManager,
                        variantManager,
                        codeEditManager,
                        colorEditManager);
        connect(renderSettingsPropertyBrowser, SIGNAL(updateUi()), this, SLOT(updateUi()));
        connect(renderSettingsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(resyncRenderSettingConfig()));


        camerasPropertyBrowser = new CamerasPropertyBrowser(this,
                                                            ui->settings,
                                                            groupManager,
                                                            variantManager,
                                                            codeEditManager,
                                                            colorEditManager);
        connect(camerasPropertyBrowser, SIGNAL(updateUi()), this, SLOT(updateUi()));
        connect(camerasPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(resyncCameraConfig()));

        objectPropertyBrowser = new ObjectPropertyBrowser(this,
                                                          ui->settings,
                                                          groupManager,
                                                          variantManager,
                                                          codeEditManager,
                                                          colorEditManager);
        connect(objectPropertyBrowser, SIGNAL(updateUi()), this, SLOT(updateUi()));
        connect(objectPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(setChanged()));

        volumePropertyBrowser = new VolumePropertyBrowser (this,
                                                           ui->settings,
                                                           groupManager,
                                                           variantManager,
                                                           codeEditManager,
                                                           colorEditManager);
        connect(volumePropertyBrowser, SIGNAL(updateUi()), this, SLOT(updateUi()));
        connect(volumePropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(setChanged()));

        initializeBackgrounds();
}



void MainWindow::setupUi() {
        changed = true;
        ui->setupUi(this);


        ui->actionShow_Command_Pile->setChecked(true);


        // code editor
        ui->codeEditorFrameOuter->setEnabled(false);
        ui->codeEditorFrameOuter->setVisible(false);

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

        groupManager = new QtGroupPropertyManager(this);

        enumManager = new QtEnumPropertyManager(this);
        connect (enumManager, SIGNAL(valueChanged(QtProperty*,int)),
                 this, SLOT(enumManager_valueChanged(QtProperty*,int)));

        colorEditManager = new ColorEditManager (this);
        connect (colorEditManager, SIGNAL(valueChanged(QtProperty*,ColorPickerColor)),
                 this, SLOT(colorEditManager_valueChanged(QtProperty*,ColorPickerColor)));
        colorEditFactory = new ColorEditFactory (this, ui->mdiArea);

        codeEditManager = new QtVariantPropertyManager(this);
        connect(codeEditManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                this, SLOT(code_valueChanged(QtProperty*, QVariant)));

        comboBoxFactory = new QtEnumEditorFactory(this);
        lineEditFactory = new QtLineEditFactory(this);

        variantFactory = new QtVariantEditorFactory(this);
        ui->settings->setFactoryForManager(variantManager, variantFactory);
        ui->settings->setFactoryForManager(enumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(colorEditManager, colorEditFactory);

        setChanged();
}



void MainWindow::setChanged() {
        if (changed) return;
        changed = true;
        ui->menuBar->setStyleSheet("background-color:#A33;");
        updateUi();
}



void MainWindow::setUnchanged() {
        if (!changed) return;
        changed = false;
        ui->menuBar->setStyleSheet("");
        updateUi();
}



void MainWindow::loadScene (redshift::scenefile::Scene const &scene) {
        initializeScene();
        filmSettingsPropertyBrowser->setFilmSettings(scene.filmSettings());
        for (unsigned int i=0; i<scene.renderSettingsCount(); ++i)
                renderSettingsPropertyBrowser->addRenderSettings(scene.renderSettings(i));
        for (unsigned int i=0; i<scene.cameraCount(); ++i)
                camerasPropertyBrowser->addCamera(scene.camera(i));
        for (unsigned int i=0; i<scene.objectCount(); ++i)
                objectPropertyBrowser->addObject(scene.object(i));
        for (unsigned int i=0; i<scene.volumeCount(); ++i)
                volumePropertyBrowser->addVolume(scene.volume(i));
        if (scene.backgroundCount())
                setBackground(scene.background(0));


        foreach (QtBrowserItem *it, ui->settings->topLevelItems())
                ui->settings->setExpanded(it, false);

        setUnchanged();
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

                renderSettingsPropertyBrowser->addRenderSettings(rs);
        }
        {
                using namespace redshift::scenefile;

                RenderSettings rs;
                rs.width = 800;
                rs.height = 600;
                rs.min_y = 0;
                rs.max_y = 0xFFFFF;
                rs.samplesPerPixel = 10;
                rs.surfaceIntegrator.type = SurfaceIntegrator::path;
                rs.surfaceIntegrator.numAmbientSamples = 10;
                rs.userSeed = 0;
                rs.volumeIntegrator.type = VolumeIntegrator::none;
                rs.volumeIntegrator.cutoffDistance = 50000;
                rs.volumeIntegrator.stepSize = 70;
                rs.title = "production";

                renderSettingsPropertyBrowser->addRenderSettings(rs);
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

        camerasPropertyBrowser->addCamera(cam);

        // - objects
        /*redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        o.horizonPlaneParams.height = 0;
        o.horizonPlaneParams.material.color.type = redshift::scenefile::Color::RGB;
        o.horizonPlaneParams.material.color.rgb.r = 0.6;
        o.horizonPlaneParams.material.color.rgb.g = 0.5;
        o.horizonPlaneParams.material.color.rgb.b = 0.4;
        addObject(o);*/

        // - background
        setBackground (redshift::scenefile::Background());


        foreach (QtBrowserItem *it, ui->settings->topLevelItems())
                ui->settings->setExpanded(it, false);

        setUnchanged();
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
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), 30);
        vp->setToolTip("For realism, use values between 2 and 10. But for artistic reasons, use any.");
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

        foreach (QtProperty *mooh, renderSettingsPropertyBrowser->subProperties()) {
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
                } else if("whitted_ambient" == sintegT) {
                        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::whitted_ambient;
                        rs.surfaceIntegrator.numAmbientSamples = readValue<int>("ambient-samples", si);
                } else if("path" == sintegT) {
                        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::path;
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

        foreach (QtProperty *cam, camerasPropertyBrowser->subProperties()) {

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



void MainWindow::resyncRenderSettingConfig () {
        ui->renderSettingConfig->clear();
        foreach (QtProperty *prop, renderSettingsPropertyBrowser->subProperties()) {
                ui->renderSettingConfig->addItem(prop->propertyName());
        }
        updateUi();
}



void MainWindow::resyncCameraConfig () {
        ui->cameraConfig->clear();
        foreach (QtProperty *prop, camerasPropertyBrowser->subProperties()) {
                ui->cameraConfig->addItem(prop->propertyName());
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



void MainWindow::updateUi() {
        refreshWindowTitle();
        on_settings_currentItemChanged(currentBrowserItem);
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

        ui->moveDownButton->setEnabled(false);
        ui->moveUpButton->setEnabled(false);

        const bool isCode = name == "code";

        if ((parentProp != 0)
            && (parentProp->propertyName() == "objects")
        ) {
                ui->deleteObjectButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Object);

                ui->moveDownButton->setEnabled(!isLast(parentProp, currentBrowserItem));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, currentBrowserItem));
        } else {
                ui->deleteObjectButton->setEnabled(false);
        }

        if ((parentProp != 0)
             && (parentProp->propertyName() == "volumes")
        ) {
                ui->deleteVolumeButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete_Volume);

                ui->moveDownButton->setEnabled(!isLast(parentProp, currentBrowserItem));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, currentBrowserItem));
        } else {
                ui->deleteVolumeButton->setEnabled(false);
        }


        const bool isTransform      = name  == "transform";
        const bool isSubTransform   = (parentProp != 0)
                                      && (parentProp->propertyName()
                                          == "transform");
        const bool isCamera         = (parentProp != 0)
                                      && (parentProp->propertyName() == "cameras");
        const bool isRenderSetting  = (parentProp != 0)
                                      && (parentProp->propertyName()
                                          == "render-settings");
                                      //(parentProp == renderSettingsProperty);

        currentCameraProperty        = 0;
        currentTransformProperty     = 0;
        currentRenderSettingProperty = 0;

        if (isSubTransform) {
                currentCameraProperty    = parentParentProp;
                currentTransformProperty = parentProp;
                ui->moveDownButton->setEnabled(!isLast(parentProp, currentBrowserItem));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, currentBrowserItem));
        }
        if (isTransform) {
                currentCameraProperty    = parentProp;
                currentTransformProperty = current->property();
        }
        if (isCamera) {
                currentCameraProperty    = current->property();
                ui->moveDownButton->setEnabled(!isLast(parentProp, currentBrowserItem));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, currentBrowserItem));
        }
        if (isRenderSetting) {
                currentRenderSettingProperty = current->property();
                ui->moveDownButton->setEnabled(!isLast(parentProp, currentBrowserItem));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, currentBrowserItem));
        }

        if (isCode) {
                ui->codeEditorFrameOuter->setEnabled(true);
                ui->codeEditorFrameOuter->setVisible(true);
                ui->codeEditor->setCode(codeEditManager->value(current->property()).toString());
        } else {
                ui->codeEditorFrameOuter->setEnabled(false);
                ui->codeEditorFrameOuter->setVisible(false);
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

        QStringList nameFilters;
        nameFilters << "Picogen scene (*.picogen)"
                    << "Everything (*)"
                    ;
        dialog.setNameFilters(nameFilters);

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (dialog.exec()) {
                QString name = dialog.selectedFiles()[0];

                // Check extension.
                if (name.endsWith(".picogen", Qt::CaseInsensitive)) {}
                else if (name.lastIndexOf('.')>=0){
                        QMessageBox::information(this,
                           "Unsupported filename extension",
                           "Please choose a filename with one of the following extensions:\n"
                           " * .picogen\n"
                        );
                        goto again;
                } else {
                        name += ".picogen";
                }

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
                QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
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
                QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
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

        // I somewhat find the OS' own file dialog to be disturbing
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setWindowTitle("Select a file to load");

        QStringList nameFilters;
        nameFilters << "Picogen scene (*.picogen)"
                    << "Everything (*)"
                    ;
        dialog.setNameFilters(nameFilters);

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (!dialog.exec()) {
                return;
        }

        const QString name = dialog.selectedFiles()[0];

        loadScene (name);
}



void MainWindow::loadScene (QString const &name) {
        redshift::scenefile::Scene scene;
        redshift::scenefile::Scene oldScene = *createScene();

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
                QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
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
                if (changed) {
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



void MainWindow::contextMenuEvent(QContextMenuEvent */*event*/) {
}



void MainWindow::on_actionDelete_Render_Setting_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        renderSettingsPropertyBrowser->remove(currentRenderSettingProperty);
        resyncRenderSettingConfig();
}
void MainWindow::on_actionNew_Render_Setting_triggered() {
        setChanged();
        redshift::scenefile::RenderSettings rs;
        rs.title = "new-setting";
        renderSettingsPropertyBrowser->addRenderSettings (rs);
}



void MainWindow::on_actionAdd_Homogeneous_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::homogeneous;
        volumePropertyBrowser->addVolume (v);
}
void MainWindow::on_actionAdd_Exponential_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::exponential;
        volumePropertyBrowser->addVolume (v);
}
void MainWindow::on_actionDelete_Volume_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        volumePropertyBrowser->remove(currentBrowserItem->property());
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

        camerasPropertyBrowser->addCamera(cam);
        resyncCameraConfig();
}
void MainWindow::on_actionDelete_Camera_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        camerasPropertyBrowser->remove(currentCameraProperty);
        resyncCameraConfig();
}



void MainWindow::on_actionNew_Sub_Transform_triggered() {
        setChanged();
        // We assume that newTransform can only clicked when the current-item
        // is a transform.
        camerasPropertyBrowser->addTransform (
                      currentTransformProperty,
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
        objectPropertyBrowser->addObject (o);
}
void MainWindow::on_actionAdd_Horizon_Plane_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        objectPropertyBrowser->addObject (o);
}
void MainWindow::on_actionAdd_Lazy_Quadtree_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::lazy_quadtree;
        objectPropertyBrowser->addObject (o);
}
void MainWindow::on_actionDelete_Object_triggered() {
        setChanged();
        // assumed to signal everything needed for clean up
        objectPropertyBrowser->remove(currentBrowserItem->property());
}



void MainWindow::on_actionShow_Command_Pile_triggered(bool checked) {
        ui->commandPileGroupBox->setEnabled(checked);
        ui->commandPileGroupBox->setVisible(checked);
}
void MainWindow::on_actionShow_Picohelp_Browser_triggered(bool checked) {
        ui->helpFrameOuter->setEnabled(checked);
        ui->helpFrameOuter->setVisible(checked);
}
void MainWindow::on_actionActionShow_Contextual_Picohelp_triggered() {

}
void MainWindow::helpBrowserVisibilityRequested() {
        on_actionShow_Picohelp_Browser_triggered(true);
}



void MainWindow::on_actionNew_Scene_triggered() {
        QStringList l;
        QProcess::startDetached(QApplication::applicationFilePath(), l);
}



void MainWindow::on_actionClose_triggered() {
        close();
}



void MainWindow::closeEvent(QCloseEvent *event) {
        if (changed) {
                const int r = QMessageBox::question(
                        this, "Unsaved data",
                        "You have unsaved changes. Save now?",
                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                        QMessageBox::Cancel);
                if (QMessageBox::Yes == r) {
                        on_actionSave_as_triggered();
                        event->accept();
                } else if (QMessageBox::No == r) {
                        event->accept();
                } else {
                        event->ignore();
                }
        } else {
                event->accept();
        }
}



void MainWindow::on_actionMaximize_triggered() {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
}



void MainWindow::on_actionShow_compiler_triggered() {
        QMessageBox::information(
                this,
                "Compiler",
                QString("renderer: ") + redshift::getCompilationInfo() + "\n"
                + QString("SimplexGui: ") + PICOGEN_COMPILATION_INFO
        );
}




bool MainWindow::eventFilter(QObject *o, QEvent *e) {

        if (e->type() == QEvent::KeyPress
         && ((QKeyEvent*)(e))->key() == Qt::Key_F1
        ) {
                const bool isOnPropertyBrowser = (QObject*)ui->settings == o;
                const bool isBrowserItemSelected = 0 != currentBrowserItem;
                if (isOnPropertyBrowser) {
                        if (isBrowserItemSelected) {
                                const QtProperty *p =
                                        currentBrowserItem->property();
                                const QString name =
                                        p->propertyName();
                                const QtBrowserItem *parent = currentBrowserItem
                                                              ->parent();
                                const QtBrowserItem *grandparent =
                                                              parent ?
                                                              parent->parent() :
                                                              0;
                                const QString pname =
                                        parent ?
                                        parent->property()->propertyName() :
                                        "";
                                const QString ppname =
                                        grandparent ?
                                        grandparent->property()->propertyName() :
                                        "";
                                QtBrowserItem *it = currentBrowserItem;
                                while (it->parent() != 0) it = it->parent();
                                const QString outer = it->property()->propertyName();

                                if (ui->picohelp->gotoSimplexGuiPropertyEdit(
                                        name, pname, ppname, outer
                                )) {
                                        return true;
                                }
                        }
                }

                QVariant helpFile = o->property("picohelp");
                if (helpFile.isValid()) {
                        ui->picohelp->gotoArticle(helpFile.toString());
                        return true;
                }
        }
        return false;
}



void MainWindow::on_picohelp_sceneFileClicked (QString const &path) {
        OpenSceneFile f(path, changed, this);
        f.exec();

        // TODO: unset path
        switch (f.result()) {
        case OpenSceneFile::Cancel:
                // do nothing
                break;
        case OpenSceneFile::OpenInNewInstance: {
                QStringList l;
                l << "-e"; // <-- open existing as if new
                l << path;
                QProcess::startDetached(QApplication::applicationFilePath(), l);
        } break;
        case OpenSceneFile::OpenInCurrentInstance:
                loadScene(path);
                saveFilename = "";
                setUnchanged();
                refreshWindowTitle();
                break;
        };

}



void MainWindow::on_moveUpButton_clicked() {
        QtBrowserItem *tmp = currentBrowserItem;
        moveUp (ui->settings, currentBrowserItem->parent()->property(), currentBrowserItem);
        ui->settings->setCurrentItem(tmp);
        on_settings_currentItemChanged(tmp);
}
void MainWindow::on_moveDownButton_clicked() {
        QtBrowserItem *tmp = currentBrowserItem;
        moveDown (ui->settings, currentBrowserItem->parent()->property(), currentBrowserItem);
        ui->settings->setCurrentItem(tmp);
        on_settings_currentItemChanged(tmp);
}
