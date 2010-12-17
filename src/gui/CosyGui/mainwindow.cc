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

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "cosyscene/scene.hh"
#include "cosyscene/sunsky.hh"
#include "cosyscene/save_load.hh"

#include "redshift/include/redshift_file/scene.hh"

#include "renderwindow.hh"
#include "stylesheetliveeditor.hh"

#include <iostream>
#include <ctime>
#include <QDebug>
#include <QTemporaryFile>

namespace redshift_file {
void save_scene (const redshift_file::Scene &scene_, std::ostream &fs_);
void save_scene (const redshift_file::Scene &scene, std::string const &name);
void load_scene (redshift_file::Scene &scene, std::istream &fs);
void load_scene (Scene &scene, std::string const &name);
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
        redshiftSceneCreator(new RedshiftSceneCreator())
{
        using redshift::shared_ptr;
        using cosyscene::Scene;

        // Inits.
        ui->setupUi(this);
        ui->terrain->setTerrain(scene->terrain());

        scene->sunSky()->toUtahSky (cosyscene::UtahSky());
        ui->sunSky->setSunSky(scene->sunSky());
        ui->navigation->setNavigation(scene->navigation());
        ui->navigation->setCreateRedshiftClosure(redshiftSceneCreator);
        ui->renderingSetup->setTwinRenderSettings(scene->renderSettings());
        ui->filmSettings->setFilmSettings (scene->filmSettings());

        connect (ui->renderingSetup, SIGNAL(productionRenderProcessRequested()),
                 SLOT(onProductionRenderProcessRequested()));
        connect (ui->renderingSetup, SIGNAL(previewRenderProcessRequested()),
                 SLOT(onPreviewRenderProcessRequested()));

        ui->forestCommandLink->setVisible(false);

        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->terrain, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->sunSky, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->navigation, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->renderingSetup, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        connect (this, SIGNAL(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)),
                 ui->filmSettings, SLOT(sceneInvalidated(redshift::shared_ptr<cosyscene::Scene>)));
        redshiftSceneCreator->scene = scene;

        // Aesthetics.
        on_filmCommandLink_clicked();
        indicateSaved();
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
        ui->stackedWidget->setCurrentWidget(ui->filmSettings);
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

void MainWindow::indicateSaved() {
        //ui->stashFrame->setStyleSheet("QFrame{background-color:#222;}");
        //ui->statusLabel->setText("");
}

void MainWindow::indicateUnsaved(){
        //ui->stashFrame->setStyleSheet("QFrame{background-color:#a66;}");
        //ui->statusLabel->setText("unsaved changes present");
}

void MainWindow::on_actionSave_triggered() {
        const QString str = askForNewSaveFilename(this);
        if (str == "")
                return;
        cosyscene::save_scene(*this->scene, str.toStdString());
}
#include "cosyscene/navigation.hh"
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
        ui->renderWidget->setSceneAndRender(scene->toRedshiftScene(), true);
}



redshift::shared_ptr<redshift_file::Scene>
  MainWindow::RedshiftSceneCreator::createPreviewScene() const
{
        return scene->toRedshiftScene(true);
}

redshift::shared_ptr<redshift_file::Scene>
  MainWindow::RedshiftSceneCreator::createProductionScene() const
{
        return scene->toRedshiftScene(false);
}

void MainWindow::on_action_Stylesheet_triggered() {
        StylesheetLiveEditor *editor = new StylesheetLiveEditor(this, this);
        editor->show();
}

void MainWindow::onProductionRenderProcessRequested() {
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

void MainWindow::onPreviewRenderProcessRequested() {
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

        RenderWindow::CosyGuiRenderProcess(QFileInfo(tmp.fileName()).canonicalFilePath(),
                                           0, 0);
}


