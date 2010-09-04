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
#include "cosyscene/save_load.hh"

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
//////

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        scene(new cosyscene::Scene())
{
        // Inits.
        ui->setupUi(this);
        ui->terrain->setTerrain(scene->terrain());

        // Aesthetics.
        on_filmCommandLink_clicked();
        indicateSaved();
}

MainWindow::~MainWindow() {
        delete ui;
}

void MainWindow::on_filmCommandLink_clicked() {
        switchOffCommandLinks();
        ui->filmCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->filmSettings);
}

void MainWindow::on_navigationCommandLink_clicked() {
        switchOffCommandLinks();
        ui->navigationCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::switchOffCommandLinks () {
        ui->filmCommandLink->setChecked(false);
        ui->terrainCommandLink->setChecked(false);
        ui->navigationCommandLink->setChecked(false);
        ui->forestCommandLink->setChecked(false);

        disconnect(SIGNAL(stash_doStash()));
        disconnect(SIGNAL(stash_doRestore()));
}

void MainWindow::on_terrainCommandLink_clicked() {
        switchOffCommandLinks();
        ui->terrainCommandLink->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->terrain);
        connect(this, SIGNAL(stash_doStash()),
                ui->terrain, SLOT(stash_doStash()));
        connect(this, SIGNAL(stash_doRestore()),
                ui->terrain, SLOT(stash_doRestore()));
}

void MainWindow::on_forestCommandLink_clicked() {
        switchOffCommandLinks();
        ui->forestCommandLink->setChecked(true);
}

void MainWindow::on_terrain_terrainChanged() {
        indicateUnsaved();
}

void MainWindow::indicateSaved() {
        ui->stashFrame->setStyleSheet("QFrame{background-color:#222;}");
        ui->statusLabel->setText("");
}

void MainWindow::indicateUnsaved(){
        ui->stashFrame->setStyleSheet("QFrame{background-color:#a66;}");
        ui->statusLabel->setText("unsaved changes present");
}

void MainWindow::on_stashButton_clicked() {
        emit stash_doStash();
}

void MainWindow::on_restoreButton_clicked() {
        emit stash_doRestore();
}

void MainWindow::on_actionSave_triggered() {
        askForNewSaveFilename(this);
        //cosyscene::save_scene()
}
