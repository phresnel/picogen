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

#include "filmsettingsandcamera.h"
#include "ui_filmsettingsandcamera.h"
#include "scopedblocksignals.h"

#include "cosyscene/scene.hh"

namespace picogen { namespace qt4_gui {

FilmSettingsAndCamera::FilmSettingsAndCamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilmSettingsAndCamera)
{
        ui->setupUi(this);
}

FilmSettingsAndCamera::~FilmSettingsAndCamera()
{
        delete ui;
}

void FilmSettingsAndCamera::setFilmSettingsAndCamera(
        redshift::shared_ptr<cosyscene::FilmSettings> fs,
        redshift::shared_ptr<cosyscene::Camera> c,
        bool blockSignals)
{
        ScopedQtSignalBlock block(this, blockSignals);
        ui->filmSettings->setFilmSettings(fs, blockSignals);
        ui->camera->setCamera(c, blockSignals);
}

void FilmSettingsAndCamera::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene)
{
        setFilmSettingsAndCamera(scene->filmSettings(),
                                 scene->camera());
}

void FilmSettingsAndCamera::on_filmSettings_filmSettingsChanged() {
        emit filmSettingsChanged();
}

void FilmSettingsAndCamera::on_camera_cameraChanged() {
        emit cameraChanged();
}

} }

#include "filmsettingsandcamera.moc"
