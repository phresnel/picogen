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

#include "cosyscene/rendersettings.hh"

#include "rendersettingswindow.hh"
#include "ui_rendersettingswindow.h"

RenderSettingsWindow::RenderSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderSettingsWindow)
{
        ui->setupUi(this);
}

RenderSettingsWindow::~RenderSettingsWindow() {
        delete ui;
}

void RenderSettingsWindow::setRenderSettings (
        redshift::shared_ptr<cosyscene::RenderSettings> rs,
        bool blockSignals
) {
        const bool prev = this->blockSignals(blockSignals);
        renderSettings_ = rs;
        updateViews();
        this->blockSignals(prev);
}

void RenderSettingsWindow::updateViews() {
        ui->widthSpin->setValue(renderSettings_->width());
        ui->heightSpin->setValue(renderSettings_->height());
        ui->seedSpin->setValue(renderSettings_->randomSeed());
        ui->samplesPerPixelSpin->setValue(renderSettings_->samplesPerPixel());
}

void RenderSettingsWindow::on_widthSpin_editingFinished() {
        renderSettings_->setWidth (ui->widthSpin->value());
}

void RenderSettingsWindow::on_heightSpin_editingFinished() {
        renderSettings_->setHeight (ui->heightSpin->value());
}

void RenderSettingsWindow::on_samplesPerPixelSpin_editingFinished() {
        renderSettings_->setSamplesPerPixel (ui->samplesPerPixelSpin->value());
}

void RenderSettingsWindow::on_seedSpin_editingFinished() {
        renderSettings_->setRandomSeed (ui->seedSpin->value());
}

void RenderSettingsWindow::setTitle (QString title) {
        ui->stashFrame->setTitle(title);
}

void RenderSettingsWindow::on_widthSpin_valueChanged(int ) {
        const double mp = (ui->widthSpin->value()*ui->heightSpin->value())
                          / 1000000.;
        ui->megapixelsDisplay->setText("(" + QString::number(mp, 'f', 1) + " MPixels)");
}

void RenderSettingsWindow::on_heightSpin_valueChanged(int ) {
        const double mp = (ui->widthSpin->value()*ui->heightSpin->value())
                          / 1000000.;
        ui->megapixelsDisplay->setText("(" + QString::number(mp, 'f', 1) + " MPixels)");
}
