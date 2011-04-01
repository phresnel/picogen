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

#include "twinrendersettingswindow.hh"
#include "ui_twinrendersettingswindow.h"

#include "cosyscene/scene.hh"
#include "cosyscene/rendersettings.hh"

TwinRenderSettingsWindow::TwinRenderSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwinRenderSettingsWindow)
{
        ui->setupUi(this);
        ui->previewRenderSettingsWindow->setIsPreviewSettings(true);
        ui->renderSettingsWindow->setIsPreviewSettings(false);
}

TwinRenderSettingsWindow::~TwinRenderSettingsWindow()
{
        delete ui;
}

void TwinRenderSettingsWindow::setTwinRenderSettings (
        redshift::shared_ptr<cosyscene::TwinRenderSettings> tw,
        bool reAdjustDefaultValues,
        bool blockSignals
) {
        const bool wasBlocked = this->blockSignals(blockSignals);

        twinRenderSettings_ = tw;
        ui->renderSettingsWindow->setRenderSettings(tw->production());        
        ui->previewRenderSettingsWindow->setRenderSettings(tw->preview());

        if (reAdjustDefaultValues) {
                ui->renderSettingsWindow->setIsPreviewSettings(false);
                ui->previewRenderSettingsWindow->setIsPreviewSettings(true);
        }

        this->blockSignals (wasBlocked);
}

void TwinRenderSettingsWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setTwinRenderSettings (scene->renderSettings());
}

void TwinRenderSettingsWindow::on_startProductionRenderProcess_clicked() {
        emit productionRenderProcessRequested();
}

void TwinRenderSettingsWindow::on_startPreviewRenderProcess_clicked() {
        emit previewRenderProcessRequested();
}

void TwinRenderSettingsWindow::on_previewRenderSettingsWindow_renderSettingsChanged() {
        emit renderSettingsChanged();
}

void TwinRenderSettingsWindow::on_renderSettingsWindow_renderSettingsChanged() {
        emit renderSettingsChanged();
}
