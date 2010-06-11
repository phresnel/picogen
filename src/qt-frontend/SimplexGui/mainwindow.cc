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


// Picogen.
#include "redshift/include/jobfile.hh"

// Qt.
#include <QMessageBox>
#include <QtWindowListMenu>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>

// qt-frontend
#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "simpleinputbox.hh"
#include "renderwindow.hh"
#include "quatschsourceeditor.hh"
#include "OpenSceneFile/openscenefile.hh"
#include "propertybrowser/scenepropertybrowser.hh"
#include "propertybrowser/propertybrowser-helpers.hh"
#include "coloredit.h"



// TODO: this should be in a header
ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c) {
        using redshift::scenefile::Color;

        ColorPickerColor ret;

        switch (c.type) {
        case Color::RGB:
                ret.mode = ColorPickerColor::Tristimulus;
                ret.tristimulus = TristimulusColor::fromRgbf(
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
    nonRecurseLock(false),
    settingsContextMenu(this)
{
        setupUi();


        propertyBrowser = new ScenePropertyBrowser(this,
                                              ui->mdiArea,
                                              ui->settings,
                                              codeEditManager);

        connect(ui->codeEditor, SIGNAL(helpBrowserVisibilityRequested()),
                this, SLOT(helpBrowserVisibilityRequested()));

        if (openFilename != "") {
                try {
                        redshift::scenefile::Scene scene;
                        std::ifstream ss(initialFilename.toStdString().c_str());
                        actuarius::IArchive (ss) & actuarius::pack("scene", scene);
                        propertyBrowser->loadScene(scene);

                        saveFilename = openFilename;
                        QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
                        refreshWindowTitle();
                } catch (...) {
                        propertyBrowser->setDefaultScene();
                }
        } else {
                propertyBrowser->setDefaultScene();
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

        ui->actionShow_PicoHelp->setChecked(true);
        on_actionShow_PicoHelp_triggered (ui->actionShow_PicoHelp->isChecked());

        ui->codeEditor->setPicohelpBrowser(ui->picohelp);

        // connect signals
        connect (propertyBrowser, SIGNAL(sceneChanged()),
                 this,            SLOT(setChanged()));
        connect (propertyBrowser, SIGNAL(resyncCameraConfig()),
                 this,            SLOT(resyncCameraConfig()));
        connect (propertyBrowser, SIGNAL(resyncRenderSettingConfig()),
                 this,            SLOT(resyncRenderSettingConfig()));
        connect (propertyBrowser, SIGNAL(asUnchanged()),
                 this,            SLOT(setUnchanged()));


        resyncCameraConfig();
        resyncRenderSettingConfig();

        setUnchanged();
}



MainWindow::~MainWindow() {
        delete ui;
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

        codeEditManager = new QtVariantPropertyManager(this);
        connect(codeEditManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                this, SLOT(code_valueChanged(QtProperty*, QVariant)));


        setChanged();
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



void MainWindow::resyncRenderSettingConfig () {
        ui->renderSettingConfig->clear();
        ui->renderSettingConfig->addItems(propertyBrowser->renderSettingNames());
}



void MainWindow::resyncCameraConfig () {
        ui->cameraConfig->clear();
        ui->cameraConfig->addItems(propertyBrowser->cameraNames());
}




void MainWindow::variantManager_valueChanged (QtProperty *, QVariant) {
        setChanged();
}



void MainWindow::enumManager_valueChanged (QtProperty *, int) {
        setChanged();
}



#include <QGraphicsBlurEffect>
bool MainWindow::event(QEvent *e) {
        switch (e->type()) {
        case QEvent::WindowBlocked: {
                QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
                blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
                blur->setBlurRadius(5);
                setGraphicsEffect(blur);
                break;
        }
        case QEvent::WindowUnblocked: {
                setGraphicsEffect(0);
                break;
        }
        default:
                break;
        };
        return QMainWindow::event(e);
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
        on_settings_currentItemChanged(propertyBrowser->currentBrowserItem());
}


// FROB
void MainWindow::on_settings_currentItemChanged(QtBrowserItem * current) {
        settingsContextMenu.clear();
        ui->moveDownButton->setEnabled(false);
        ui->moveUpButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);

        ui->codeEditorFrameOuter->setEnabled(false);
        ui->codeEditorFrameOuter->setVisible(false);

        ui->newSubTransformButton->setEnabled(false);

        if (current == 0)
                return;

        settingsContextMenu.addAction(ui->actionNew_Render_Setting);
        settingsContextMenu.addAction(ui->actionNew_Camera);
        settingsContextMenu.addMenu(&objectsMenu);
        settingsContextMenu.addMenu(&volumesMenu);
        settingsContextMenu.addSeparator();

        QtProperty *parentProp =
               (current->parent()==0)
               ? 0
               : current->parent()->property();
        QtProperty *parentParentProp =
               (current->parent()==0 || current->parent()->parent()==0)
               ? 0
               : current->
                        parent()->
                        parent()->property();
               //: findParent(ui->settings->properties(),
               //             parentProp);

        const QString name = current->property()->propertyName();
        const QString parentName = (parentProp==0) ? "" : parentProp->propertyName();
        const QString parentParentName = (parentParentProp==0) ? "" : parentParentProp->propertyName();

        const bool isTransform      = name  == "transform";
        const bool isSubTransform   = parentName ==  "transform";
        const bool isCamera         = parentName == "cameras";
        const bool isRenderSetting  = parentName =="render-settings";
        const bool isVolume         = parentName == "volumes";
        const bool isObject         = parentName == "objects";
        const bool isCode           = name == "code";

        if (isCode) {
                ui->codeEditorFrameOuter->setEnabled(true);
                ui->codeEditorFrameOuter->setVisible(true);
                ui->codeEditor->setCode(codeEditManager->value(current->property()).toString());
        }

        if (propertyBrowser->currentTransformProperty() != 0) {
                ui->newSubTransformButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionNew_Sub_Transform);
        }
        if (isSubTransform) {
                ui->actionNew_Sub_Transform->setEnabled (true);
        }


        if (isSubTransform
         || isCamera
         || isRenderSetting
         || isObject
         || isVolume
        ) {
                ui->moveDownButton->setEnabled(!isLast(parentProp, current));
                ui->moveUpButton->setEnabled(!isFirst(parentProp, current));
                ui->deleteButton->setEnabled(true);
                settingsContextMenu.addAction(ui->actionDelete);
        }
}



void MainWindow::on_actionShow_redshift_job_code_triggered() {
        using namespace actuarius;
        using namespace redshift;

        try {
                const shared_ptr<scenefile::Scene> scene = propertyBrowser->createScene ();
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
        codeEditManager->setValue(propertyBrowser->currentBrowserItem()->property(),
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



void MainWindow::on_newSubTransformButton_clicked() {
        on_actionNew_Sub_Transform_triggered();
}



void MainWindow::on_newCameraButton_clicked() {
        on_actionNew_Camera_triggered();
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
        QString code = propertyBrowser->sceneToCode();
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
        QString code = propertyBrowser->sceneToCode();
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
        QString code = propertyBrowser->sceneToCode();
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



void MainWindow::on_renderButton_clicked() {
        render();
}



void MainWindow::render() {
        try {
                const int
                      renderSettings = ui->renderSettingConfig->currentIndex(),
                      camera = ui->cameraConfig->currentIndex();
                RenderWindow *rw = new RenderWindow (
                                propertyBrowser->createScene(),
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
        redshift::scenefile::Scene oldScene = *propertyBrowser->createScene();

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
                propertyBrowser->loadScene (scene);
                saveFilename = name;
                QDir::setCurrent(QFileInfo(saveFilename).absolutePath());
                refreshWindowTitle();
        } catch (std::exception const &e){
                propertyBrowser->loadScene(oldScene);
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



void MainWindow::on_actionNew_Render_Setting_triggered() {
        setChanged();
        redshift::scenefile::RenderSettings rs;
        rs.title = "new-setting";
        propertyBrowser->addRenderSettings (rs);
}



void MainWindow::on_actionAdd_Homogeneous_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::homogeneous;
        propertyBrowser->addVolume (v);
}
void MainWindow::on_actionAdd_Exponential_Volume_triggered() {
        setChanged();
        redshift::scenefile::Volume v;
        v.type = redshift::scenefile::Volume::exponential;
        propertyBrowser->addVolume (v);
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

        propertyBrowser->addCamera(cam);
        resyncCameraConfig();
}



void MainWindow::on_actionNew_Sub_Transform_triggered() {
        setChanged();
        // We assume that newTransform can only clicked when the current-item
        // is a transform.
        propertyBrowser->addTransform(
                        propertyBrowser->currentTransformProperty(),
                        redshift::scenefile::Transform());
}




void MainWindow::on_actionAdd_Water_Plane_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::water_plane;
        propertyBrowser->addObject (o);
}
void MainWindow::on_actionAdd_Horizon_Plane_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        propertyBrowser->addObject (o);
}
void MainWindow::on_actionAdd_Lazy_Quadtree_triggered() {
        setChanged();
        redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::lazy_quadtree;
        propertyBrowser->addObject (o);
}
void MainWindow::on_deleteButton_clicked() {
        on_actionDelete_triggered();
}
void MainWindow::on_actionDelete_triggered() {
        QtProperty*    curr    = propertyBrowser->currentBrowserItem()->property();
        QtBrowserItem* parenti = propertyBrowser->currentBrowserItem()->parent();
        QtProperty*    parent  = parenti ? parenti->property() : 0;

        if (parent) {
                if (QMessageBox::Yes == QMessageBox::question(
                        this,
                        "", "Delete \"" + curr->propertyName() + "\"?",
                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                        QMessageBox::Cancel
                )) {
                        parent->removeSubProperty(curr);
                        ui->settings->setCurrentItem(parenti);
                        setChanged();
                }
        }
}



void MainWindow::on_actionShow_Command_Pile_triggered(bool checked) {
        /*ui->commandPileGroupBox->setEnabled(checked);
        ui->commandPileGroupBox->setVisible(checked);*/
        if (checked) ui->commandPileGroupBox->show();
        else         ui->commandPileGroupBox->hide();
}
void MainWindow::on_actionShow_PicoHelp_triggered(bool checked) {
        if (checked) ui->helpFrameOuter->show();
        else         ui->helpFrameOuter->hide();
}
void MainWindow::helpBrowserVisibilityRequested() {
        on_actionShow_PicoHelp_triggered(true);
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
                const bool isBrowserItemSelected = 0 != propertyBrowser->currentBrowserItem();
                if (isOnPropertyBrowser) {
                        if (isBrowserItemSelected) {
                                const QtProperty *p =
                                        propertyBrowser->currentBrowserItem()->property();
                                const QString name =
                                        p->propertyName();
                                const QtBrowserItem *parent = propertyBrowser->currentBrowserItem()
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
                                QtBrowserItem *it = propertyBrowser->currentBrowserItem();
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
        QtBrowserItem *tmp = propertyBrowser->currentBrowserItem();
        moveUp (ui->settings, propertyBrowser->currentBrowserItem()->parent()->property(), propertyBrowser->currentBrowserItem());
        ui->settings->setCurrentItem(tmp);
        on_settings_currentItemChanged(tmp);
        setChanged();
}
void MainWindow::on_moveDownButton_clicked() {
        QtBrowserItem *tmp = propertyBrowser->currentBrowserItem();
        moveDown (ui->settings, propertyBrowser->currentBrowserItem()->parent()->property(), propertyBrowser->currentBrowserItem());
        ui->settings->setCurrentItem(tmp);
        on_settings_currentItemChanged(tmp);
        setChanged();
}


