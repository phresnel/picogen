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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startpopup.h"
#include "cosyscene/scene.hh"
#include "cosyscene/sunsky.hh"
#include "cosyscene/save_load.hh"

#include "redshift/include/redshift_file/scene.hh"

#include "renderwindow.h"
#include "stylesheetliveeditor.h"
#include "textdialog.h"

#include <iostream>
#include <ctime>
#include <QDebug>
#include <QTemporaryFile>
#include <QTime>

namespace redshift_file {
void save_scene (const redshift_file::Scene &scene_, std::ostream &fs_);
void save_scene (const redshift_file::Scene &scene, std::string const &name);
void load_scene (redshift_file::Scene &scene, std::istream &fs);
void load_scene (Scene &scene, std::string const &name);
}



namespace detail {
        class RedshiftSceneCreator : public CreateRedshiftSceneClosure {
        public:
                redshift::shared_ptr<redshift_file::Scene>
                 createPreviewScene() const
                {
                        return scene->toRedshiftScene(true);
                }
                redshift::shared_ptr<redshift_file::Scene>
                  createProductionScene() const
                {
                        return scene->toRedshiftScene(false);
                }

                void setScene (redshift::shared_ptr<cosyscene::Scene> scene) {
                        this->scene = scene;
                }

        private:
                redshift::shared_ptr<cosyscene::Scene> scene;
        };
}



//////
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
QString askForNewSaveFilename (QWidget *parent) {
        again:
        QFileDialog dialog(parent);
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
                        QMessageBox::information(parent,
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
                    QMessageBox::question(parent, "Overwrite file?",
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
                        QMessageBox::warning(parent, "Failed to save",
                             "The file \"" + file.fileName() + "\" could not be"
                             +" opened for writing. Please select another file "
                             +" or one that does not exist yet.");
                        goto again;
                }

                return name;
        }
        return "";
}

QString askForOpenFilename(QWidget *parent) {
        // I find the OS' own file dialog to be somewhat disturbing
        QFileDialog dialog(parent);
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
                return "";
        }

        return dialog.selectedFiles()[0];
}

//////

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        scene(new cosyscene::Scene()),
        redshiftSceneCreator(new detail::RedshiftSceneCreator())
{
        using redshift::shared_ptr;
        using cosyscene::Scene;

        // Inits.
        ui->setupUi(this);
        ui->terrain->setTerrain(scene->terrain());
        ui->water->setWater(scene->water());

        scene->sunSky()->toUtahSky (cosyscene::UtahSky());
        ui->sunSky->setSunSky(scene->sunSky());
        //ui->navigation->setNavigation(scene->navigation());
        ui->navigation->setScene (scene);
        ui->navigation->setCreateRedshiftClosure(redshiftSceneCreator);
        ui->renderingSetup->setTwinRenderSettings(scene->renderSettings(), true);
        ui->filmSettingsAndCamera->setFilmSettingsAndCamera (scene->filmSettings(),
                                                             scene->camera());

        connect (ui->renderingSetup, SIGNAL(productionRenderProcessRequested()),
                 SLOT(startProductionRenderProcess()));
        connect (ui->renderingSetup, SIGNAL(previewRenderProcessRequested()),
                 SLOT(startPreviewRenderProcess()));

        ui->forestCommandLink->setVisible(false);

        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->terrain, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->water, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->sunSky, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->navigation, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->renderingSetup, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->filmSettingsAndCamera, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));

        connect (ui->terrain, SIGNAL(terrainChanged()),
                 ui->navigation, SLOT(updateTerrain()));
        connect (ui->water, SIGNAL(waterChanged()),
                 ui->navigation, SLOT(updateWater()));
        redshiftSceneCreator->setScene (scene);

        // Aesthetics.
        on_terrainCommandLink_clicked();
        indicateSaved();

        StartPopup popup (this);
        popup.setDisclaimer("IMPORTANT: This is a nightly build of picogen; it might "
                            "contain bugs, glitches, incomplete features "
                            "and features that are not present in regular releases.\n\n"
                            "Scenes created with this version of picogen may be "
                            "incompatible with future versions of picogen!\n\n"
                            "Apart from that: Welcome and thank you for your interest in picogen :)");
        popup.exec();
}

MainWindow::~MainWindow() {
        delete ui;
}

void MainWindow::switchOffCommandLinks () {
        ui->renderingSetupCommandLink->setChecked(false);
        ui->filmCommandLink->setChecked(false);
        ui->terrainCommandLink->setChecked(false);
        ui->waterCommandLink->setChecked(false);
        ui->navigationCommandLink->setChecked(false);
        ui->forestCommandLink->setChecked(false);
        ui->sunskyCommandLink->setChecked(false);
}

void MainWindow::on_waterCommandLink_clicked() {
        switchOffCommandLinks();
        ui->waterCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->water);
}

void MainWindow::on_filmCommandLink_clicked() {
        switchOffCommandLinks();
        ui->filmCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->filmSettingsAndCamera);
}

void MainWindow::on_navigationCommandLink_clicked() {
        switchOffCommandLinks();
        ui->navigationCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->navigation);
}

void MainWindow::on_terrainCommandLink_clicked() {
        switchOffCommandLinks();
        ui->terrainCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->terrain);
}

void MainWindow::on_forestCommandLink_clicked() {
        switchOffCommandLinks();
        ui->forestCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(0);
}

void MainWindow::on_sunskyCommandLink_clicked() {
        switchOffCommandLinks();
        ui->sunskyCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->sunSky);
}

void MainWindow::on_renderingSetupCommandLink_clicked() {
        switchOffCommandLinks();
        ui->renderingSetupCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->renderingSetup);
}

void MainWindow::on_terrain_terrainChanged() {
        indicateUnsaved();
}

void MainWindow::on_sunSky_skyChanged() {
        indicateUnsaved();
}

void MainWindow::on_navigation_navigationChanged() {
        indicateUnsaved();
}

void MainWindow::on_water_waterChanged() {
        indicateUnsaved();
}

void MainWindow::on_filmSettingsAndCamera_filmSettingsChanged() {
        indicateUnsaved();
}

void MainWindow::on_filmSettingsAndCamera_cameraChanged() {
        indicateUnsaved();
}

void MainWindow::on_renderingSetup_renderSettingsChanged() {
        indicateUnsaved();
}

void MainWindow::indicateSaved() {
        setWindowTitle("");
}

void MainWindow::indicateUnsaved(){
        setWindowTitle(windowTitle()+"*");
}

void MainWindow::on_actionSave_triggered() {
        const QString str = askForNewSaveFilename(this);
        if (str == "")
                return;
        cosyscene::save_scene(*this->scene, str.toStdString());
}

void MainWindow::on_actionLoad_triggered() {
        const QString str = askForOpenFilename(this);
        if (str == "")
                return;
        try {
                cosyscene::Scene scene;
                cosyscene::load_scene(scene, str.toStdString());
                *this->scene = scene;
                emit sceneInvalidated(this->scene);

        } catch (std::exception const &e) {
                std::cerr << e.what() << std::endl;
        } catch (...) {
                std::cerr << "unknown exception" << std::endl;
        }
}

void MainWindow::on_renderCommandLink_clicked() {
        // Et hop.
        redshift::shared_ptr<redshift_file::Scene>
                        scene = this->scene->toRedshiftScene();
        ui->renderWidget->setSceneAndRender(scene, true);

        std::ostringstream ss;
        redshift_file::save_scene(*scene, ss);
        lastRedshiftJob = ss.str();
}

void MainWindow::on_action_Stylesheet_triggered() {
        StylesheetLiveEditor *editor = new StylesheetLiveEditor(this, this);
        editor->show();
}

void MainWindow::startProductionRenderProcess() {
        QTemporaryFile tmp ("XXXXXX.picogen");
        tmp.setAutoRemove(false);

        if (!tmp.open()) {
                return;
        }

        // Documentation says as long as the file-object is not destroyed (i.e.
        // goes out of scope) it safe to use the filename it generates.
        // anways ...
        tmp.close();

        redshift::shared_ptr<redshift_file::Scene> scene =
                                redshiftSceneCreator->createProductionScene();
        redshift_file::save_scene(*scene, tmp.fileName().toStdString());
        RenderWindow::CosyGuiRenderProcess(tmp.fileName(), 0, 0);

        std::ostringstream ss;
        redshift_file::save_scene(*scene, ss);
        lastRedshiftJob = ss.str();

        // Would yield deadlock as no other process might remove the file
        // as long as tmp is alive.
        /*
        // Spinlock that waits until the sub-process killed the file.
        std::clock_t b = std::clock();
        while (QFile::exists(tmp.fileName())) {
                QApplication::processEvents();
                if ((std::clock() - b)>(CLOCKS_PER_SEC*10)) {
                        int res = QMessageBox::warning(this, "Warning",
                          "The freshly started render-process seems to be in "
                          "trouble or does not live at all, as it takes very "
                          "long to delete the file \"" +
                          tmp.fileName() + "\", which is used to communicate "
                          "with the render process.\n\n"
                          "Delete now? ",
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::Yes
                        );
                        if (res == QMessageBox::No) {
                                b = std::clock();
                        } else {
                                tmp.remove();
                                break;
                        }
                }
        }*/
}

void MainWindow::startPreviewRenderProcess() {
        QTemporaryFile tmp ("XXXXXX.picogen");
        tmp.setAutoRemove(false);

        if (!tmp.open()) {
                return;
        }

        // Documentation says as long as the file-object is not destroyed (i.e.
        // goes out of scope) it safe to use the filename it generates.
        // anways ...
        tmp.close();

        redshift::shared_ptr<redshift_file::Scene> scene =
                                redshiftSceneCreator->createPreviewScene();

        tmp.open();
        redshift_file::save_scene(*scene, tmp.fileName().toStdString());

        std::ostringstream ss;
        redshift_file::save_scene(*scene, ss);
        lastRedshiftJob = ss.str();

        RenderWindow::CosyGuiRenderProcess(QFileInfo(tmp.fileName()).canonicalFilePath(),
                                           0, 0);
}


void MainWindow::on_actionShow_redshift_file_triggered() {
        using redshift_file::save_scene;

        QMessageBox bb;
        const QPushButton
            *preview = bb.addButton("Preview", QMessageBox::ActionRole),
            *production = bb.addButton("Production", QMessageBox::ActionRole);

        bb.addButton(QMessageBox::Cancel);
        bb.setText("Preview or Production?");
        bb.exec();

        std::ostringstream ss;
        if (bb.clickedButton() == preview) {
                save_scene(*redshiftSceneCreator->createPreviewScene(), ss);
        } else if (bb.clickedButton() == production) {
                save_scene(*redshiftSceneCreator->createProductionScene(), ss);
        } else {
                return;
        }

        TextDialog d(true, this);
        d.setText(ss.str());
        d.exec();
}

void MainWindow::on_actionShow_redshift_file_used_for_last_rendering_triggered() {
        TextDialog d(true, this);
        d.setText(lastRedshiftJob);
        d.exec();
}


void compactCommandLink (QCommandLinkButton *clb) {
        clb->setIconSize(clb->iconSize()/2);
        clb->setStyleSheet("font-size:7pt;");
}
void uncompactCommandLink (QCommandLinkButton *clb) {
        clb->setIconSize(clb->iconSize()*2);
        clb->setStyleSheet("");
}
void compactCommandLink(QCommandLinkButton *clb, bool compact) {
        if (compact) compactCommandLink(clb);
        else uncompactCommandLink(clb);
}

void MainWindow::on_actionCompact_left_pane_triggered(bool checked) {
        compactCommandLink(ui->terrainCommandLink, checked);
        compactCommandLink(ui->renderCommandLink, checked);
        compactCommandLink(ui->terrainCommandLink, checked);
        compactCommandLink(ui->waterCommandLink, checked);
        compactCommandLink(ui->navigationCommandLink, checked);
        compactCommandLink(ui->forestCommandLink, checked);
        compactCommandLink(ui->sunskyCommandLink, checked);
        compactCommandLink(ui->filmCommandLink, checked);
        compactCommandLink(ui->renderingSetupCommandLink, checked);
}


#include "mainwindow.moc"
