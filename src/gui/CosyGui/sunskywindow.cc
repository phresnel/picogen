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

#include "sunskywindow.hh"
#include "ui_sunskywindow.h"
#include "cosyscene/sunsky.hh"
#include "cosyscene/scene.hh"
#include "stashview.hh"
#include "redshift/include/smart_ptr.hh"

#include <QMessageBox>



SunSkyWindow::SunSkyWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SunSkyWindow)
{
        ui->setupUi(this);
        ui->stashFrame->setTitle("Sky & Atmosphere");
        ui->stashFrame->setPixmap(":/toplevel-navigation/skyatmosphere");
}



SunSkyWindow::~SunSkyWindow() {
        delete ui;
}



void SunSkyWindow::setSunSky (redshift::shared_ptr<cosyscene::SunSky> t,
                              bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        sunSky = t;
        updateViews();
        this->blockSignals(prevBlocked);
}



void SunSkyWindow::setSunSkyByValue (cosyscene::SunSky const &t,
                                     bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        *sunSky = t;
        updateViews();
        this->blockSignals(prevBlocked);

}



void SunSkyWindow::updateViews () {
        switch (sunSky->kind()) {
        case cosyscene::SunSky::UtahSky:
                updateToUtahSunSkyEditor();
                break;
        case cosyscene::SunSky::None:
                break;
        }
}



void SunSkyWindow::sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene) {
        setSunSky(scene->sunSky());
}



void SunSkyWindow::on_utahSkyEditor_overcastChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_atmosphericEffectsEnabledChanged(bool) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_atmosphericEffectsFactorChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_previewResolutionChanged(int) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_diskSizeChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_turbidityChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_previewMultiplierChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_sunIntensityChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_atmosphereIntensityChanged(double) {
        updateFromUtahSunSkyEditor();
}
void SunSkyWindow::on_utahSkyEditor_sunDirectionChanged(redshift::Vector) {
        updateFromUtahSunSkyEditor();
}



void SunSkyWindow::updateFromUtahSunSkyEditor() {
        const QtSunSkyEditor &u = *ui->utahSkyEditor;
        cosyscene::UtahSky utah;

        utah.atmosphereBrightnessFactor = u.atmosphereIntensity();
        utah.atmosphericEffects = u.atmosphericEffectsEnabled();
        utah.atmosphericFxFactor = u.atmosphericEffectsFactor();
        utah.overcast = u.overcast();
        utah.sunBrightnessFactor = u.sunIntensity();

        utah.sunDirection = cosyscene::Direction3d(u.sunDirection().x,
                                                   u.sunDirection().y,
                                                   u.sunDirection().z);

        utah.sunSizeFactor = u.diskSize();
        utah.turbidity = u.turbidity();

        sunSky->toUtahSky(utah);

        emit skyChanged();
}



void SunSkyWindow::updateToUtahSunSkyEditor() {
        QtSunSkyEditor &u = *ui->utahSkyEditor;
        const bool blocked = u.blockSignals(true);

        const cosyscene::UtahSky utah = sunSky->utahSky();

        u.setAtmosphereIntensity(utah.atmosphereBrightnessFactor);
        u.setAtmosphericEffectsEnabled(utah.atmosphericEffects);
        u.setAtmosphericEffectsFactor(utah.atmosphericFxFactor);
        u.setOvercast(utah.overcast);
        u.setSunIntensity(utah.sunBrightnessFactor);

        u.setSunDirection(redshift::Vector(utah.sunDirection.x(),
                                           utah.sunDirection.y(),
                                           utah.sunDirection.z()));

        u.setDiskSize(utah.sunSizeFactor);
        u.setTurbidity(utah.turbidity);

        u.blockSignals(blocked);
}



void SunSkyWindow::on_stashButton_clicked() {
        sunSky->stash();
}



void SunSkyWindow::on_stashRestoreButton_clicked() {
        StashView *sw = new StashView (this);
        sw->addItems(sunSky->getStash());
        if (QDialog::Accepted == sw->exec()) {
                redshift::shared_ptr<cosyscene::SunSky> newSunSky (
                        new cosyscene::SunSky(
                          sw->selectedData<cosyscene::SunSky>())
                );
                newSunSky->setStash(sw->itemsToStash<cosyscene::SunSky>());
                setSunSkyByValue(*newSunSky, true);
        }
}



void SunSkyWindow::on_stashResetButton_clicked() {
        if (!sunSky->getStash().contains_data(*sunSky)) {
                switch (confirmReset (this)) {
                case ConfirmReset_Abort: return;
                case ConfirmReset_StashBeforeReset: sunSky->stash(); break;
                case ConfirmReset_Reset: break;
                }
        }
        cosyscene::SunSky t;
        t.toUtahSky(cosyscene::UtahSky());
        t.setStash(sunSky->getStash());
        setSunSkyByValue(t, true);
}

