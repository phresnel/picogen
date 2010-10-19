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

#include "stashview.hh"

#include <stdexcept>


#include <QDebug>
#include "redshift/include/sealed.hh"
SEALED(ScopedQtSignalBlock);
class ScopedQtSignalBlock : MAKE_SEALED(ScopedQtSignalBlock) {
public:
        ScopedQtSignalBlock (QObject *obj, bool block)
        : obj(obj), wasBlocked (obj->blockSignals(block))
        {
        }
        ~ScopedQtSignalBlock () {
                obj->blockSignals(wasBlocked);
        }

private:
        ScopedQtSignalBlock();
        ScopedQtSignalBlock(ScopedQtSignalBlock const&);
        ScopedQtSignalBlock& operator=(ScopedQtSignalBlock const&);

        QObject *obj;
        bool wasBlocked;
};




NavigationWindow::NavigationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationWindow)
{
        ui->setupUi(this);
        ui->stashFrame->setTitle("Navigation");
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
        const bool was = ui->yawSpin->blockSignals(true);
        ui->yawSpin->setValue(value);
        ui->yawSpin->blockSignals(was);
        updateFromViews();
}
void NavigationWindow::on_yawSpin_valueChanged(double value) {
        const bool was = ui->yawDial->blockSignals(true);
        ui->yawDial->setValue(value);
        ui->yawDial->blockSignals(was);
        updateFromViews();
}

void NavigationWindow::on_pitchSlider_valueChanged(int position) {
        const bool was = ui->pitchSpin->blockSignals(true);
        ui->pitchSpin->setValue(-position);
        ui->pitchSpin->blockSignals(was);
        updateFromViews();
}

void NavigationWindow::on_pitchSpin_valueChanged(double value) {
        const bool was = ui->pitchSlider->blockSignals(true);
        ui->pitchSlider->setValue(-value);
        ui->pitchSlider->blockSignals(was);
        updateFromViews();
}

void NavigationWindow::on_rollDial_valueChanged(int position) {
        const bool was = ui->rollSpin->blockSignals(true);
        ui->rollSpin->setValue(-position);
        ui->rollSpin->blockSignals(was);
        updateFromViews();
}

void NavigationWindow::on_rollSpin_valueChanged(double value) {
        const bool was = ui     ->rollDial->blockSignals(true);
        ui->rollDial->setValue(-value);
        ui->rollDial->blockSignals(was);
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




void NavigationWindow::setNavigation (
        redshift::shared_ptr<cosyscene::Navigation> nav,
        bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        navigation_ = nav;
        updateViews();
        this->blockSignals(prevBlocked);
}



void NavigationWindow::setNavigationByValue (
        const cosyscene::Navigation &nav,
        bool blockSignals
) {
        ScopedQtSignalBlock (this, blockSignals);
        *navigation_ = nav;
        updateViews();
}

#include <QDebug>
void NavigationWindow::updateViews() {
        /*ScopedQtSignalBlock
                yawBlocked (ui->yawSpin, true),
                pitchBlocked (ui->pitchSpin, true),
                rollBlocked (ui->rollSpin, true),
                xBlocked (ui->xSpin, true),
                yBlocked (ui->ySpin, true),
                zBlocked (ui->zSpin, true);*/

        switch (navigation_->kind()) {
        case cosyscene::Navigation::YawPitchRoll: {
                const cosyscene::YawPitchRoll&ypr = navigation_->yawPitchRoll();
                ui->yawSpin->setValue(ypr.yaw);
                ui->pitchSpin->setValue(ypr.pitch);
                ui->rollSpin->setValue(ypr.roll);
                ui->xSpin->setValue(ypr.position.x());
                ui->ySpin->setValue(ypr.position.y());
                ui->zSpin->setValue(ypr.position.z());
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



void NavigationWindow::on_stashButton_clicked() {
        navigation_->stash();
}



void NavigationWindow::on_stashRestoreButton_clicked() {
        StashView *sw = new StashView (this);
        sw->addItems(navigation_->getStash());
        if (QDialog::Accepted == sw->exec()) {
                cosyscene::Navigation newNav =
                          sw->selectedData<cosyscene::Navigation>();
                newNav.setStash(sw->itemsToStash<cosyscene::Navigation>());
                setNavigationByValue(newNav);
        }
}



void NavigationWindow::on_stashResetButton_clicked() {
        if (!navigation_->getStash().contains_data(*navigation_)) {
                switch (confirmReset (this)) {
                case ConfirmReset_Abort: return;
                case ConfirmReset_StashBeforeReset: navigation_->stash(); break;
                case ConfirmReset_Reset: break;
                }
        }
        cosyscene::Navigation t;
        t.toYawPitchRoll(cosyscene::YawPitchRoll());
        t.setStash(navigation_->getStash());
        setNavigationByValue(t);
}



#include "renderwindow.hh"
#include "cosyscene/scene.hh"
void NavigationWindow::on_refreshButton_clicked() {
        qWarning("{{{");
        redshift::shared_ptr<redshift_file::Scene> tmp = cosyscene::Scene()
                                                         .toRedshiftScene();
        ui->redshiftWidget->setSceneAndRender(tmp);
        qWarning("}}}");
}
