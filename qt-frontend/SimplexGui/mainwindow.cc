#include "qmessagebox.h"

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include "qtwindowlistmenu.h"
#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qttreepropertybrowser.h"
#include <QtStringPropertyManager>

#include "../SimpleInputBox/simpleinputbox.hh"
#include "../RenderWindow/renderwindow.hh"
#include "../../redshift/include/jobfile.hh"


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


        QString readValueText (QString name, QList<QtProperty*> list) {
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                return looky->valueText();
                        }
                }
                return "";
        }


        QList<QtProperty*> readSubProperties (QString name, QList<QtProperty*> list) {
                foreach (QtProperty* looky, list) {
                        if (name == looky->propertyName()) {
                                return looky->subProperties();
                        }
                }
                return QList<QtProperty*>();
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
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentBrowserItem(0)
{
        ui->setupUi(this);

        // WindowList
        QtWindowListMenu *winMenu = new QtWindowListMenu(menuBar());
        winMenu->attachToMdiArea(ui->mdiArea);
        winMenu->setTitle("Windows");
        menuBar()->addMenu(winMenu);

        // Properties
        variantManager = new QtVariantPropertyManager(this);
        rsTitleManager = new QtStringPropertyManager (this);
        connect(rsTitleManager, SIGNAL(valueChanged (QtProperty *, const QString &)),
                this, SLOT(rsTitleManager_valueChanged(QtProperty*,const QString &)));

        groupManager = new QtGroupPropertyManager(this);        
        enumManager = new QtEnumPropertyManager(this);


        transformEnumManager = new QtEnumPropertyManager(this);
        connect(transformEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(transformEnumManager_valueChanged(QtProperty*,int)));

        objectTypeEnumManager = new QtEnumPropertyManager(this);
        connect(objectTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(objectTypeEnumManager_valueChanged(QtProperty*,int)));


        comboBoxFactory = new QtEnumEditorFactory(this);
        lineEditFactory = new QtLineEditFactory(this);

        variantFactory = new QtVariantEditorFactory(this);
        ui->settings->setFactoryForManager(variantManager, variantFactory);
        ui->settings->setFactoryForManager(enumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(transformEnumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(objectTypeEnumManager, comboBoxFactory);
        ui->settings->setFactoryForManager(rsTitleManager, lineEditFactory);

        // Film Settings.
        {
                QtProperty *topItem = 0;
                QtVariantProperty *item = 0;

                topItem = groupManager->addProperty(QLatin1String("film-settings"));

                // convert-to-srgb
                item = variantManager->addProperty(
                                QVariant::Bool,
                                QLatin1String("convert-to-srgb"));
                item->setValue(true);
                topItem->addSubProperty(item);

                // color-scale
                item = variantManager->addProperty(
                                QVariant::Double,
                                QLatin1String("color-scale"));
                item->setValue(1);
                item->setAttribute(QLatin1String("singleStep"), 0.05);
                item->setAttribute(QLatin1String("decimals"), 6);
                topItem->addSubProperty(item);


                ui->settings->addProperty(topItem);
        }

        // Render Settings.
        {
                QtProperty *topItem = groupManager->addProperty("render-settings");
                renderSettingsProperty = topItem;
                ui->settings->addProperty(topItem);

                addRenderSettings("preview");
                addRenderSettings("production");
        }

        // Camera Settings.
        {
                camerasProperty = groupManager->addProperty("cameras");
                ui->settings->addProperty(camerasProperty);
                addCamera("hello-world");
        }

        // Objects.
        {
                objectsProperty = groupManager->addProperty("objects");
                ui->settings->addProperty(objectsProperty);

                addObject();
                addObject();
        }

        ui->settings->setRootIsDecorated(true);
        //ui->settings->setIndentation(32);
        ui->settings->setHeaderVisible(false);



        //menuBar()->repaint();
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::addRenderSettings (std::string const &name) {

        QtProperty *topItem = groupManager->addProperty(name.c_str());

        QtProperty *title = rsTitleManager->addProperty("title");
        rsTitleManager->setRegExp(title, QRegExp("([a-z0-9]|-|_)+", Qt::CaseInsensitive, QRegExp::RegExp));
        rsTitleManager->setValue(title, name.c_str());
        topItem->addSubProperty(title);


        QtVariantProperty *it = variantManager->addProperty(QVariant::Int, "width");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(800);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "height");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(600);
        topItem->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Int, "samples-per-pixel");
        it->setAttribute(QLatin1String("minimum"), 1);
        it->setAttribute(QLatin1String("maximum"), 0xFFFFFF);
        it->setAttribute(QLatin1String("singleStep"), 1);
        it->setValue(1);
        topItem->addSubProperty(it);


        QtProperty *volumeIntegrator = groupManager->addProperty("volume-integrator");
        topItem->addSubProperty(volumeIntegrator);

        QtProperty *integratorType = enumManager->addProperty("type");
        QStringList enumNames;
        enumNames << "none" << "emission" << "single";
        enumManager->setEnumNames(integratorType, enumNames);
        volumeIntegrator->addSubProperty(integratorType);
        it = variantManager->addProperty(QVariant::Double, "step-size");

        it->setAttribute(QLatin1String("minimum"), 1.);
        it->setAttribute(QLatin1String("maximum"), 32768.);
        it->setAttribute(QLatin1String("singleStep"), 1.);
        it->setAttribute(QLatin1String("decimals"), 1);
        it->setValue(500);
        volumeIntegrator->addSubProperty(it);

        it = variantManager->addProperty(QVariant::Double, "cutoff-distance");
        it->setAttribute(QLatin1String("minimum"), 1.);
        it->setAttribute(QLatin1String("maximum"), 32768.);
        it->setAttribute(QLatin1String("singleStep"), 1.);
        it->setAttribute(QLatin1String("decimals"), 1);
        volumeIntegrator->addSubProperty(it);

        renderSettingsProperty->addSubProperty(topItem);
}



void MainWindow::addCamera(const std::string &name) {
        QtProperty *camera = groupManager->addProperty(name.c_str());

        QtProperty *transformRoot = groupManager->addProperty("transform");

        camera->addSubProperty(transformRoot);
        camerasProperty->addSubProperty(camera);

        addTransform (transformRoot);
}



void MainWindow::addTransform (QtProperty *transformRoot) {
        QtProperty *transform = groupManager->addProperty("---");
        transformRoot->addSubProperty(transform);


        QtProperty *transformType = transformEnumManager->addProperty("type");
        transform->addSubProperty(transformType);

        QStringList enumNames;
        enumNames << "move"
                  << "move-left"
                  << "move-right"
                  << "move-up"
                  << "move-backward"
                  << "move-forward"
                  << "yaw"
                  << "pitch"
                  << "roll"
                  ;
        transformEnumManager->setEnumNames(transformType, enumNames);        
}



void MainWindow::addObject () {
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
}



void MainWindow::transformEnumManager_valueChanged(
        QtProperty* prop,
        int index
) {
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

                if (type == "move") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"right"));
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"up"));
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
                }

        }
}



void MainWindow::objectTypeEnumManager_valueChanged (
        QtProperty* prop,
        int index
) {
        // parent
        //   |
        //   +-----type
        //   |
        //   +-----param*

        // We have 'type', now find 'parent'
        if (QtProperty *t = findParent(ui->settings->properties(), prop)) {
                const QStringList enumNames =
                                objectTypeEnumManager->enumNames(prop);
                const QString type = enumNames[index];

                t->setPropertyName(type);

                // Remove all properties not titled "type".
                foreach (QtProperty *nt, t->subProperties()){
                        if (nt->propertyName() != "type") {
                                t->removeSubProperty(nt);
                        }
                }

                if (type == "horizon-plane") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"height"));                        
                } else if (type == "water-plane") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"height"));
                        t->addSubProperty(variantManager->addProperty(QVariant::String,"code"));
                } else if (type == "lazy-quadtree") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Color,"color"));
                        //t->addSubProperty(variantManager->addProperty(QVariant::V,"color"));
                }
        }
}



void MainWindow::rsTitleManager_valueChanged (
        QtProperty * property,
        const QString & value
) {
        if (QtProperty *par = findParent (ui->settings->properties(), property)) {
                par->setPropertyName(value);
        }
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
        currentBrowserItem = current;
        if (current->property()->propertyName() == "transform") {
                ui->newTransformButton->setEnabled(true);
        } else {
                ui->newTransformButton->setEnabled(false);
        }
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
        foreach (Prop mooh, renderSettings) {
                Props subs = mooh->subProperties();

                scenefile::RenderSettings rs;
                rs.width = readValue<unsigned int>("width", subs);
                rs.height = readValue<unsigned int>("height", subs);
                rs.samplesPerPixel = readValue<unsigned int>("samples-per-pixel", subs);

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

                scene->addRenderSettings(rs);
        }
        return scene;
}



void MainWindow::on_actionShow_redshift_job_code_triggered() {        
        using namespace actuarius;
        using namespace redshift;

        const shared_ptr<scenefile::Scene> scene = createScene ();
        std::stringstream ss;
        OArchive (ss) & pack("scene", *scene);
        QMessageBox::information(this, QString("Teh codes"), ss.str().c_str());
}



void MainWindow::on_actionRender_triggered() {
        RenderWindow *rw = new RenderWindow (createScene(), this);
        ui->mdiArea->addSubWindow(rw);
        rw->show();
}



void MainWindow::on_newRsButton_pressed() {
        addRenderSettings ("new_setting");
}



void MainWindow::on_newTransformButton_pressed() {
        // We assume that newTransform can only clicked when the current-item
        // is a transform.
        addTransform (currentBrowserItem->property());
}



void MainWindow::on_newObjectButton_pressed() {
        addObject ();
}