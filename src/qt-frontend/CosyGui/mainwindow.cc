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
}
