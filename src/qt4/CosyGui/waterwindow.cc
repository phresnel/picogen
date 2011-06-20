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

#include "waterwindow.h"
#include "ui_waterwindow.h"
#include "cosyscene/scene.hh"
#include "cosyscene/water.hh"
#include "stashview.h"
#include <QMessageBox>

WaterWindow::WaterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaterWindow),
    water(new cosyscene::Water)
{
        ui->setupUi(this);
        on_showFormationTab_clicked();
}

WaterWindow::~WaterWindow() {
        delete ui;
}


void WaterWindow::setWater (redshift::shared_ptr<cosyscene::Water> t,
                                bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        water = t;
        ui->formation->setFormation(water->formation(),
                                    blockSignals);
        ui->texturing->setMaterial(water->material(),
                                   blockSignals);
        ui->fitting->setFitting (water->fitting(),
                                 blockSignals);
        updateViews();
        this->blockSignals(prevBlocked);
}



void WaterWindow::updateViews () {
        const bool wasBlocked = blockSignals(true);
        blockSignals(wasBlocked);
}



void WaterWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setWater (scene->water());
}


void WaterWindow::on_showFormationTab_clicked() {
        ui->showFormationTab->setChecked(true);
        ui->showTexturingTab->setChecked(false);
        ui->showWorldFittingTab->setChecked(false);
        ui->waterTaskStackedWidget->setCurrentIndex(0);
}

void WaterWindow::on_showTexturingTab_clicked() {
        ui->showFormationTab->setChecked(false);
        ui->showTexturingTab->setChecked(true);
        ui->showWorldFittingTab->setChecked(false);
        ui->waterTaskStackedWidget->setCurrentIndex(1);
}

void WaterWindow::on_showWorldFittingTab_clicked() {
        ui->showFormationTab->setChecked(false);
        ui->showTexturingTab->setChecked(false);
        ui->showWorldFittingTab->setChecked(true);
        ui->waterTaskStackedWidget->setCurrentIndex(2);
}

void WaterWindow::on_fitting_fittingChanged() {
        emit waterChanged();
}
void WaterWindow::on_formation_formationChanged() {
        emit waterChanged();
}
void WaterWindow::on_texturing_materialChanged() {
        emit waterChanged();
}

#include "waterwindow.moc"
