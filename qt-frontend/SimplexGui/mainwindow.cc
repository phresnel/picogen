#include "qmessagebox.h"

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include "qtwindowlistmenu.h"
#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qttreepropertybrowser.h"
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
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        // WindowList
        QtWindowListMenu *winMenu = new QtWindowListMenu(menuBar());
        winMenu->attachToMdiArea(ui->mdiArea);
        winMenu->setTitle("Windows");
        menuBar()->addMenu(winMenu);

        // Properties
        variantManager = new QtVariantPropertyManager(this);
        groupManager = new QtGroupPropertyManager(this);
        enumManager = new QtEnumPropertyManager(this);
        comboBoxFactory = new QtEnumEditorFactory(this);

        variantFactory = new QtVariantEditorFactory(this);
        ui->settings->setFactoryForManager(variantManager, variantFactory);
        ui->settings->setFactoryForManager(enumManager, comboBoxFactory);


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
                QtProperty *topItem = 0;

                topItem = groupManager->addProperty("render-settings");
                renderSettingsProperty = topItem;
                ui->settings->addProperty(topItem);

                addRenderSettings("preview");
                addRenderSettings("production");
        }

        ui->settings->setRootIsDecorated(true);
        //ui->settings->setIndentation(32);
        ui->settings->setHeaderVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRenderSettings (std::string const &name) {

        QtProperty *topItem = groupManager->addProperty(name.c_str());

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

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_actionShow_redshift_job_code_triggered()
{
        typedef QList<QtProperty*> Props;
        typedef QtProperty* Prop;
        typedef QtVariantProperty* VProp;


        Props topProps = ui->settings->properties();

        QString ret;
        using namespace redshift;
        scenefile::Scene scene;

        // FilmSettings.
        const Props filmSettings = readSubProperties("film-settings", topProps);
        scenefile::FilmSettings fs;
        fs.colorscale = readValue<double>("color-scale", filmSettings);
        fs.convertToSrgb = readValue<bool>("convert-to-srgb", filmSettings);
        scene.setFilmSettings(fs);

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

                scene.addRenderSettings(rs);
        }

        using namespace actuarius;
        std::stringstream ss;
        OArchive (ss) & pack("scene", scene);
        QMessageBox::information(this, QString("Teh codes"), ss.str().c_str());
}

void MainWindow::on_actionRender_triggered() {
        RenderWindow *rw = new RenderWindow (this);
        ui->mdiArea->addSubWindow(rw);
        rw->show();
}

void MainWindow::on_pushButton_pressed() {
        addRenderSettings("new setting");
}

void MainWindow::on_pushButton_2_pressed() {
        SimpleInputBox *sib = new SimpleInputBox(this);
        sib->exec();
        QMessageBox::information(this, "", sib->value());
}
