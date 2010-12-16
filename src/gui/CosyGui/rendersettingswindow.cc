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
#include "scopedblocksignals.hh"

#include <QMessageBox>

RenderSettingsWindow::RenderSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderSettingsWindow)
{
        ui->setupUi(this);

        using cosyscene::SurfaceIntegrator;
        ScopedQtSignalBlock siblocked(ui->surfaceIntegrator, true);
        ui->surfaceIntegrator->addItem("<none> (objects will be invisible)",
                                       QVariant(SurfaceIntegrator::none));
        ui->surfaceIntegrator->addItem("Whitted Style",
                                       QVariant(SurfaceIntegrator::whitted));
        ui->surfaceIntegrator->addItem("Whitted Style + Ambient Lighting",
                                       QVariant(SurfaceIntegrator::whitted_ambient));
        ui->surfaceIntegrator->addItem("Path Tracing",
                                       QVariant(SurfaceIntegrator::path));
        ui->surfaceIntegrator->addItem("debugging: Distance",
                                       QVariant(SurfaceIntegrator::debug_distance));
        ui->surfaceIntegrator->addItem("debugging: Surface Normals",
                                       QVariant(SurfaceIntegrator::debug_normals));
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

        const unsigned int maxLQTDepth = renderSettings_->maxLazyQuadtreeDepth();
        ui->maxLazyQuadtreeDepthSpin->setValue(maxLQTDepth);
        ui->maxLazyQuadtreeDepthSpin->setEnabled(maxLQTDepth != 0);
        ui->enableMaxQuadtreeDepthOverride->setChecked(maxLQTDepth != 0);

        const cosyscene::SurfaceIntegrator si = renderSettings_->surfaceIntegrator();
        ui->surfaceIntegrator->setCurrentIndex(ui->surfaceIntegrator->
                                               findData(QVariant(si.kind())));
        if (si.kind() == cosyscene::SurfaceIntegrator::whitted_ambient) {
                ui->ambientSamples->setVisible(true);
                ui->ambientSamplesLabel->setVisible(true);
                ui->ambientSamples->setValue(si.whittedAmbientIntegrator().numAmbientSamples());
        } else {
                ui->ambientSamples->setVisible(false);
                ui->ambientSamplesLabel->setVisible(false);
        }
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

void RenderSettingsWindow::on_maxLazyQuadtreeDepthSpin_editingFinished() {
        renderSettings_->setMaxLazyQuadtreeDepth (
                        ui->maxLazyQuadtreeDepthSpin->value());
}

void RenderSettingsWindow::on_enableMaxQuadtreeDepthOverride_toggled(bool checked) {
        if (checked) {
                ui->maxLazyQuadtreeDepthSpin->setEnabled(true);
                renderSettings_->setMaxLazyQuadtreeDepth (
                                ui->maxLazyQuadtreeDepthSpin->value());
        } else {
                ui->maxLazyQuadtreeDepthSpin->setEnabled(false);
                renderSettings_->setMaxLazyQuadtreeDepth (0);
        }
}

void RenderSettingsWindow::on_surfaceIntegrator_currentIndexChanged(int index) {
        typedef cosyscene::WhittedAmbientIntegrator WAI;

        cosyscene::SurfaceIntegrator si = renderSettings_->surfaceIntegrator();
        const QVariant v = ui->surfaceIntegrator->itemData(index);
        const cosyscene::SurfaceIntegrator::Kind kind =
                static_cast<cosyscene::SurfaceIntegrator::Kind>(v.toInt());

        if (kind == cosyscene::SurfaceIntegrator::whitted_ambient) {
                ui->ambientSamples->setVisible(true);
                ui->ambientSamplesLabel->setVisible(true);
                si.toWhittedAmbientIntegrator(WAI(ui->ambientSamples->value()));
        } else {
                ui->ambientSamples->setVisible(false);
                ui->ambientSamplesLabel->setVisible(false);
                si.to(kind);
        }

        renderSettings_->setSurfaceIntegrator(si);
}

void RenderSettingsWindow::on_ambientSamples_valueChanged(int val){
        using cosyscene::SurfaceIntegrator;
        typedef cosyscene::WhittedAmbientIntegrator WAI;

        SurfaceIntegrator si = renderSettings_->surfaceIntegrator();
        if (si.kind() == cosyscene::SurfaceIntegrator::whitted_ambient) {
                WAI wai = si.whittedAmbientIntegrator();
                wai.setNumAmbientSamples(val);
                si.toWhittedAmbientIntegrator(wai);
        }
        renderSettings_->setSurfaceIntegrator(si);
}
