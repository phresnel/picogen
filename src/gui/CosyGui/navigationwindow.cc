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
#include "cosyscene/terrain.hh"
#include "cosyscene/scene.hh"
#include "cosyscene/water.hh"

#include "navigationwindow.hh"
#include "ui_navigationwindow.h"

#include "stashview.hh"

#include <stdexcept>
#include <QDebug>
#include "scopedblocksignals.hh"


#include "redshift/include/basictypes/quatsch-height-function.hh"
#include "heightfunction.hh"
namespace {
        class QuatschHeightFunction : public HeightFunction {
        public:
                QuatschHeightFunction (const std::string &code) {
                        try {
                                std::stringstream errors;
                                hh.reset(new redshift::QuatschHeightFunction(code, errors));
                                valid_ = true;
                        } catch (...) {
                                valid_ = false;
                        }
                }
                bool valid() const { return valid_; }

                virtual double height (double x, double z) const {
                        if (!valid_) return 0;
                        return (*hh)(x,z);
                }

        private:
                bool valid_;
                redshift::shared_ptr<redshift::QuatschHeightFunction> hh;
        };

}

NavigationWindow::NavigationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationWindow)
{
        ui->setupUi(this);
        ui->stashFrame->setTitle("Navigation");
        ui->subNavigationFrame->setVisible(false);
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

void NavigationWindow::setCreateRedshiftClosure (
        CreateRedshiftSceneClosure::Ptr val
) {
        createRedshiftScene = val;
}


void NavigationWindow::setScene (
        redshift::shared_ptr<cosyscene::Scene> scene,
        bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        scene_ = scene;
        navigation_ = scene->navigation();
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
                /*ui->yawSpin->setValue(ypr.yaw);
                ui->pitchSpin->setValue(ypr.pitch);
                ui->rollSpin->setValue(ypr.roll);
                ui->xSpin->setValue(ypr.position.x());
                ui->ySpin->setValue(ypr.position.y());
                ui->zSpin->setValue(ypr.position.z());*/
                ui->graphicalNavigation->setYaw(ypr.yaw);
                ui->graphicalNavigation->setPitch(ypr.pitch);
                ui->graphicalNavigation->setRoll(ypr.roll);
                ui->graphicalNavigation->setPosition(ypr.position.x(),
                                                     ypr.position.y(),
                                                     ypr.position.z());
        } break;
        case cosyscene::Navigation::None:
                throw std::runtime_error ("Navigation::updateViews() called"
                                          " for kind 'None'");
        }


        //const cosyscene::Terrain & t = *terrain_;

        if (scene_->terrain()) {
                ui->graphicalNavigation->setHeightFunction (
                       HeightFunction::Ptr(
                         new QuatschHeightFunction(scene_->finalTerrainCode()))
                );
        } else {
                ui->graphicalNavigation->setHeightFunction (
                       HeightFunction::Ptr(new QuatschHeightFunction("0")));
        }
        if (scene_->water()) {
                ui->graphicalNavigation->setWaterLevel(scene_->water()->fitting()->seaLevel());
        } else {
                ui->graphicalNavigation->setWaterLevel(-999999);
        }
}


void NavigationWindow::updateFromViews(bool refreshIfAutoRefreshEnabled) {
        cosyscene::YawPitchRoll ypr;
        ypr.yaw = ui->graphicalNavigation->yaw();
        ypr.pitch = ui->graphicalNavigation->pitch();
        ypr.roll = ui->graphicalNavigation->roll();
        ypr.position = cosyscene::Point3d(
                          ui->graphicalNavigation->position().x(),
                          ui->graphicalNavigation->position().y(),
                          ui->graphicalNavigation->position().z()
                       );
        navigation_->toYawPitchRoll(ypr);

        if (refreshIfAutoRefreshEnabled
            && ui->autoRefreshCheckBox->checkState() == Qt::Checked
        ){
                refreshPreview();
        }
}



void NavigationWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        //setNavigation (scene->navigation());
        setScene (scene);
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
        refreshPreview();
}



void NavigationWindow::refreshPreview() {
        redshift::shared_ptr<redshift_file::Scene> tmp =
                        createRedshiftScene->createPreviewScene();
        ui->redshiftWidget->setSceneAndRender(tmp);
}




void NavigationWindow::on_graphicalNavigation_positionChanged (QVector3D) {
        updateFromViews();
        qDebug() << "pos changed in gn";
}

void NavigationWindow::on_graphicalNavigation_orientationChanged (qreal, qreal, qreal) {
        updateFromViews();
        qDebug() << "orientation changed in gn";
}
