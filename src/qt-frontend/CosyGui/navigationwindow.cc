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

#include "cosyscene/navigation.hh"
#include "cosyscene/scene.hh"

#include "navigationwindow.hh"
#include "ui_navigationwindow.h"

#include <stdexcept>

NavigationWindow::NavigationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
}

NavigationWindow::~NavigationWindow() {
    delete ui;
}

void NavigationWindow::on_showPrecisionTab_clicked() {
        ui->showPrecisionTab->setChecked(true);
        ui->showRealTimeTab->setChecked(false);
        ui->showRenderTab->setChecked(false);
        ui->stackedWidget->setCurrentIndex(0);
}

void NavigationWindow::on_showRealTimeTab_clicked() {
        ui->showPrecisionTab->setChecked(false);
        ui->showRealTimeTab->setChecked(true);
        ui->showRenderTab->setChecked(false);
        ui->stackedWidget->setCurrentIndex(1);
}

void NavigationWindow::on_showRenderTab_clicked() {
        ui->showPrecisionTab->setChecked(false);
        ui->showRealTimeTab->setChecked(false);
        ui->showRenderTab->setChecked(true);
        ui->stackedWidget->setCurrentIndex(2);
}

void NavigationWindow::on_yawDial_sliderMoved(int value) {
        ui->yawSpin->setValue(value);
        updateFromViews();
}
void NavigationWindow::on_yawSpin_valueChanged(double value) {
        ui->yawDial->setValue(value);
        updateFromViews();
}

void NavigationWindow::on_pitchSlider_sliderMoved(int position) {
        ui->pitchSpin->setValue(position);
        updateFromViews();
}

void NavigationWindow::on_pitchSpin_valueChanged(double value) {
        ui->pitchSlider->setValue(value);
        updateFromViews();
}

void NavigationWindow::on_rollDial_sliderMoved(int position) {
        ui->rollSpin->setValue(position);
        updateFromViews();
}

void NavigationWindow::on_rollSpin_valueChanged(double value) {
        ui->rollDial->setValue(value);
        updateFromViews();
}

void NavigationWindow::on_xSpin_valueChanged(double value) {
        updateFromViews();
}

void NavigationWindow::on_ySpin_valueChanged(double value) {
        updateFromViews();
}

void NavigationWindow::on_zSpin_valueChanged(double value) {
        updateFromViews();
}




void NavigationWindow::setNavigation (redshift::shared_ptr<cosyscene::Navigation> nav,
                                bool blockSignals
) {
        bool prevBlocked;
        if (blockSignals)
                prevBlocked = this->blockSignals(true);
        navigation_ = nav;
        updateViews();
        if (blockSignals)
                this->blockSignals(prevBlocked);
}



void NavigationWindow::updateViews() {
        switch (navigation_->kind()) {
        case cosyscene::Navigation::YawPitchRoll: {
                const cosyscene::YawPitchRoll&ypr = navigation_->yawPitchRoll();
                ui->yawSpin->setValue(ypr.yaw);
                ui->pitchSpin->setValue(ypr.pitch);
                ui->rollSpin->setValue(ypr.roll);
        } break;
        case cosyscene::Navigation::None:
                throw std::runtime_error ("Navigation::updateViews() called"
                                          " for kind 'None'");
        }
}



void NavigationWindow::updateFromViews() {
        cosyscene::YawPitchRoll ypr;
        ypr.yaw = ui->yawSpin->value();
        ypr.pitch = ui->pitchSpin->value();
        ypr.roll = ui->rollSpin->value();

        ypr.position = cosyscene::Point3d(
                ui->xSpin->value(),
                ui->ySpin->value(),
                ui->zSpin->value()
        );
        navigation_->toYawPitchRoll(ypr);
}



void NavigationWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setNavigation (scene->navigation());
}




#include "renderwindow.hh"
#include "cosyscene/scene.hh"
void NavigationWindow::on_refreshButton_clicked() {
        qWarning("{{{");
        redshift::shared_ptr<redshift::scenefile::Scene> tmp = cosyscene::Scene().toRedshiftScene();
        ui->redshiftWidget->setSceneAndRender(tmp);
        qWarning("}}}");
}
