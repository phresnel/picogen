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

#include "filmsettingswindow.hh"
#include "ui_filmsettingswindow.h"
#include "stashview.hh"

#include "scopedblocksignals.hh"

#include "cosyscene/filmsettings.hh"
#include "cosyscene/scene.hh"

FilmSettingsWindow::FilmSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilmSettingsWindow),
    filmSettings_(new cosyscene::FilmSettings())
{
        ui->setupUi(this);
        ui->stashFrame->setTitle("Film");
}

FilmSettingsWindow::~FilmSettingsWindow() {
        delete ui;
}

void FilmSettingsWindow::setFilmSettings (
        redshift::shared_ptr<cosyscene::FilmSettings> fs,
        bool blockSignals
) {
        ScopedQtSignalBlock block(this, blockSignals);
        filmSettings_ = fs;
        updateViews();
}

void FilmSettingsWindow::updateViews() {
        ui->brightness->setValue(filmSettings_->brightnessFactor());
}

void FilmSettingsWindow::on_brightness_valueChanged(double value) {
        filmSettings_->setBrightnessFactor(value);
}

void FilmSettingsWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setFilmSettings (scene->filmSettings());
}



void FilmSettingsWindow::on_stashButton_clicked() {
        filmSettings_->stash();
}
void FilmSettingsWindow::on_stashRestoreButton_clicked() {
        StashView *sw = new StashView (this);
        sw->addItems(filmSettings_->getStash());
        if (QDialog::Accepted == sw->exec()) {
                redshift::shared_ptr<cosyscene::FilmSettings> newFilmSettings (
                  new cosyscene::FilmSettings(
                    sw->selectedData<cosyscene::FilmSettings>())
                );
                newFilmSettings->setStash(
                  sw->itemsToStash<cosyscene::FilmSettings>());

                ScopedQtSignalBlock block (this, true);
                *filmSettings_ = *newFilmSettings;
                updateViews();
        }
}
void FilmSettingsWindow::on_stashResetButton_clicked() {
        if (!filmSettings_->getStash().contains_data(*filmSettings_)) {
                switch (confirmReset (this)) {
                case ConfirmReset_Abort: return;
                case ConfirmReset_StashBeforeReset: filmSettings_->stash(); break;
                case ConfirmReset_Reset: break;
                }
        }
        cosyscene::FilmSettings t;
        t.setStash(filmSettings_->getStash());

        ScopedQtSignalBlock block (this, true);
        *filmSettings_ = t;
        updateViews();
}
