//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#include "camerawidget.hh"
#include "ui_camerawidget.h"

#include "stashview.hh"
#include "scopedblocksignals.hh"

#include "cosyscene/camera.hh"
#include "cosyscene/scene.hh"

CameraWidget::CameraWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CameraWidget),
        camera_(new cosyscene::Camera())
{
        ui->setupUi(this);
        ui->stashFrame->setTitle("Camera");
}

CameraWidget::~CameraWidget() {
        delete ui;
}

void CameraWidget::setCamera (
        redshift::shared_ptr<cosyscene::Camera> c,
        bool blockSignals
) {
        ScopedQtSignalBlock block(this, blockSignals);
        camera_ = c;
        updateViews();
}

void CameraWidget::updateViews() {
        ScopedQtSignalBlock blockKind(ui->cameraKind, true);

        switch (camera_->kind()) {
        case cosyscene::Camera::pinhole:
                ui->cameraKind->setCurrentIndex(0);
                showPinholeParameters();
                break;
        case cosyscene::Camera::cylindrical:
                ui->cameraKind->setCurrentIndex(1);
                showCylindricalParameters();
                break;
        case cosyscene::Camera::cubemap_face:
                ui->cameraKind->setCurrentIndex(2);
                showCubemapFaceParameters();
                break;
        }
}

void CameraWidget::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setCamera (scene->camera());
}

void CameraWidget::on_cameraKind_currentIndexChanged(int index) {
        switch (index) {
        case 0: camera_->toPinholeCamera();
                showPinholeParameters();
                break;
        case 1: camera_->toCylindricalCamera();
                showCylindricalParameters();
                break;
        case 2: camera_->toCubemapFaceCamera();
                showCubemapFaceParameters();
                break;
        }
}

void CameraWidget::showPinholeParameters() {
        ui->stackedWidget->setCurrentWidget(ui->pinholeWidget);
        ui->pinholeFrontPlaneDistance->setValue(camera_->pinholeCamera()
                                                .frontPlaneDistance());
}

void CameraWidget::showCylindricalParameters() {
        ui->stackedWidget->setCurrentWidget(ui->cylindricalWidget);
        ui->cylindricalFrontPlaneDistance->setValue(camera_->cylindricalCamera()
                                                    .frontPlaneDistance());
}

void CameraWidget::showCubemapFaceParameters() {
        ui->stackedWidget->setCurrentWidget(ui->cubemapFaceWidget);
        switch (camera_->cubemapFaceCamera().face()) {
        case cosyscene::CubemapFaceCamera::front:
                ui->cubemapFace->setCurrentIndex(0);
                break;
        case cosyscene::CubemapFaceCamera::back:
                ui->cubemapFace->setCurrentIndex(1);
                break;
        case cosyscene::CubemapFaceCamera::left:
                ui->cubemapFace->setCurrentIndex(2);
                break;
        case cosyscene::CubemapFaceCamera::right:
                ui->cubemapFace->setCurrentIndex(3);
                break;
        case cosyscene::CubemapFaceCamera::bottom:
                ui->cubemapFace->setCurrentIndex(4);
                break;
        case cosyscene::CubemapFaceCamera::top:
                ui->cubemapFace->setCurrentIndex(5);
                break;
        }
}

void CameraWidget::on_pinholeFrontPlaneDistance_valueChanged(double) {
        cosyscene::PinholeCamera cam = camera_->pinholeCamera();
        cam.setFrontPlaceDistance(ui->pinholeFrontPlaneDistance->value());
        camera_->toPinholeCamera(cam);
}

void CameraWidget::on_cylindricalFrontPlaneDistance_valueChanged(double) {
        cosyscene::CylindricalCamera cam = camera_->cylindricalCamera();
        cam.setFrontPlaceDistance(ui->cylindricalFrontPlaneDistance->value());
        camera_->toCylindricalCamera(cam);
}

void CameraWidget::on_cubemapFace_currentIndexChanged(int index) {
        typedef cosyscene::CubemapFaceCamera CFC;
        CFC cam = camera_->cubemapFaceCamera();
        switch (index) {
        case 0: cam.setFace(CFC::front); break;
        case 1: cam.setFace(CFC::back); break;
        case 2: cam.setFace(CFC::left); break;
        case 3: cam.setFace(CFC::right); break;
        case 4: cam.setFace(CFC::bottom); break;
        case 5: cam.setFace(CFC::top); break;
        }
        camera_->toCubemapFaceCamera(cam);
}



void CameraWidget::on_stashButton_clicked() {
        camera_->stash();
}
void CameraWidget::on_stashRestoreButton_clicked() {
        StashView *sw = new StashView (this);
        sw->addItems(camera_->getStash());
        if (QDialog::Accepted == sw->exec()) {
                redshift::shared_ptr<cosyscene::Camera> newCamera (
                  new cosyscene::Camera(
                    sw->selectedData<cosyscene::Camera>())
                );
                newCamera->setStash(
                  sw->itemsToStash<cosyscene::Camera>());

                ScopedQtSignalBlock block (this, true);
                *camera_ = *newCamera;
                updateViews();
        }
}
void CameraWidget::on_stashResetButton_clicked() {
        if (!camera_->getStash().contains_data(*camera_)) {
                switch (confirmReset (this)) {
                case ConfirmReset_Abort: return;
                case ConfirmReset_StashBeforeReset: camera_->stash(); break;
                case ConfirmReset_Reset: break;
                }
        }
        cosyscene::Camera t;
        t.setStash(camera_->getStash());

        ScopedQtSignalBlock block (this, true);
        *camera_ = t;
        updateViews();
}
