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
#include "cosyscene/filmsettings.hh"

#include "rendersettingswindow.hh"
#include "ui_rendersettingswindow.h"
#include "scopedblocksignals.hh"
#include "stashview.hh"

#include <QMessageBox>
#include <QMenu>
#include <QDesktopWidget>

RenderSettingsWindow::RenderSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderSettingsWindow),
    renderSettings_(new cosyscene::RenderSettings())
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
        ScopedQtSignalBlock block(this, blockSignals);
        renderSettings_ = rs;
        updateViews();
}

void RenderSettingsWindow::setRenderSettingsByValue (
        cosyscene::RenderSettings const &rs,
        bool blockSignals)
{
    ScopedQtSignalBlock block(this, blockSignals);
    *renderSettings_ = rs;
    updateViews();
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

void RenderSettingsWindow::on_samplesPerPixelSpin_valueChanged(int) {
        renderSettings_->setSamplesPerPixel (ui->samplesPerPixelSpin->value());
        emit renderSettingsChanged();
}

void RenderSettingsWindow::on_seedSpin_valueChanged(int) {
        renderSettings_->setRandomSeed (ui->seedSpin->value());
        emit renderSettingsChanged();
}

void RenderSettingsWindow::setTitle (QString title) {
        ui->stashFrame->setTitle(title);
}

void RenderSettingsWindow::on_widthSpin_valueChanged(int ) {
        const double mp = (ui->widthSpin->value()*ui->heightSpin->value())
                          / 1000000.;
        ui->megapixelsDisplay->setText("(" + QString::number(mp, 'f', 1) + " MPixels)");
        renderSettings_->setWidth (ui->widthSpin->value());
        emit renderSettingsChanged();
}

void RenderSettingsWindow::on_heightSpin_valueChanged(int ) {
        const double mp = (ui->widthSpin->value()*ui->heightSpin->value())
                          / 1000000.;
        ui->megapixelsDisplay->setText("(" + QString::number(mp, 'f', 1) + " MPixels)");
        renderSettings_->setHeight (ui->heightSpin->value());
        emit renderSettingsChanged();
}

void RenderSettingsWindow::on_maxLazyQuadtreeDepthSpin_valueChanged(int) {
        renderSettings_->setMaxLazyQuadtreeDepth (
                        ui->maxLazyQuadtreeDepthSpin->value());
        emit renderSettingsChanged();
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
        emit renderSettingsChanged();
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
        emit renderSettingsChanged();
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
        emit renderSettingsChanged();
}

void RenderSettingsWindow::on_autoResolutionButton_clicked() {
        QMenu menu;

        const QDesktopWidget *desk = QApplication::desktop();
        const QRect desktopSize = desk->screenGeometry();
        const int deskW = desktopSize.width(),
                  deskH = desktopSize.height();
        const int currW = ui->widthSpin->value(),
                  currH = ui->heightSpin->value();

        const double f4_3 = 4.0 / 3.0;
        const double f16_9 = 16.0 / 9.0;
        menu.addAction("Desktop: "
                       + QString::number(deskW) + "x"
                       + QString::number(deskH),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(deskW, deskH));
        menu.addAction("Desktop 16:9: "
                       + QString::number(deskW) + "x"
                       + QString::number((int)(deskW / f16_9)),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(deskW, (int)(deskW / f16_9)));
        menu.addAction("Desktop 3:1: "
                       + QString::number(deskW) + "x"
                       + QString::number((int)(deskW / 3)),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(deskW, (int)(deskW / 3)));
        menu.addAction("Desktop 4:1: "
                       + QString::number(deskW) + "x"
                       + QString::number((int)(deskW / 4)),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(deskW, (int)(deskW / 4)));


        menu.addSeparator();
        menu.addAction("4:3 (keep width): "
                       + QString::number(currW) + "x"
                       + QString::number((int)((double)currW / f4_3)),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(currW, (int)((double)currW / f4_3)));
        menu.addAction("4:3 (keep height): "
                       + QString::number((int)(currH * f4_3)) + "x"
                       + QString::number(currH),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint((int)(currH * f4_3), currH));

        menu.addAction("16:9 (keep width): "
                       + QString::number(currW) + "x"
                       + QString::number((int)((double)currW / f16_9)),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(currW, (int)((double)currW / f16_9)));

        menu.addAction("16:9 (keep height): "
                       + QString::number((int)(currH * f16_9)) + "x"
                       + QString::number(currH),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint((int)(currH * f16_9), currH));

        menu.addSeparator();
        menu.addAction("Height*2, Width*2: "
                       + QString::number(currW*2) + "x"
                       + QString::number(currH*2),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(currW*2, currH*2));
        menu.addAction("Height/2, Width/2: "
                       + QString::number(currW/2) + "x"
                       + QString::number(currH/2),
                       this, SLOT(setAutoResolutionFromAction()))
                ->setData(QPoint(currW/2, currH/2));

        menu.exec(QCursor::pos());
}

void RenderSettingsWindow::setAutoResolutionFromAction() {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action) {
                const QPoint res = action->data().toPoint();

                const bool block = blockSignals (true);
                ui->widthSpin->setValue(res.x());
                ui->heightSpin->setValue(res.y());

                on_widthSpin_valueChanged(ui->widthSpin->value());
                on_heightSpin_valueChanged(ui->heightSpin->value());
                blockSignals (block);
                emit renderSettingsChanged();
        }
}



void RenderSettingsWindow::on_stashButton_clicked() {
        if (StashView::StashDialog (this, renderSettings_)) {
                emit renderSettingsChanged();
        }
}
void RenderSettingsWindow::on_stashRestoreButton_clicked() {
        if (StashView::RestoreDialog (this, renderSettings_)) {
                setRenderSettingsByValue(*renderSettings_);
                emit renderSettingsChanged();
        }
}
void RenderSettingsWindow::on_stashResetButton_clicked() {
        if (StashView::ResetDialog(this, renderSettings_)) {
                setRenderSettingsByValue(*renderSettings_);
                emit renderSettingsChanged();
        }
}
