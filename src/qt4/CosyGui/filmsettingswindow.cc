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

#include "filmsettingswindow.h"
#include "ui_filmsettingswindow.h"
#include "stashview.h"

#include "scopedblocksignals.h"

#include "cosyscene/filmsettings.hh"
#include "cosyscene/scene.hh"

namespace picogen { namespace qt4 {

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
        const bool blocked = ui->brightness->blockSignals(signalsBlocked());
        ui->brightness->setValue(filmSettings_->brightnessFactor().toDouble());
        ui->brightness->blockSignals(blocked);
}

void FilmSettingsWindow::on_brightness_valueChanged(double value) {
        filmSettings_->setBrightnessFactor(value);
        emit filmSettingsChanged();
}

void FilmSettingsWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setFilmSettings (scene->filmSettings());
}



void FilmSettingsWindow::on_stashButton_clicked() {
        if (StashView::StashDialog (this, filmSettings_)) {
                emit filmSettingsChanged();
        }
}
void FilmSettingsWindow::on_stashRestoreButton_clicked() {
        if (StashView::RestoreDialog (this, filmSettings_)) {
                updateViews();
                emit filmSettingsChanged();
        }
}
void FilmSettingsWindow::on_stashResetButton_clicked() {
        if (StashView::ResetDialog(this, filmSettings_)) {
                updateViews();
                emit filmSettingsChanged();
        }
}

} }

#include "filmsettingswindow.moc"
