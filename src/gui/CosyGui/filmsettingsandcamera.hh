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

#ifndef FILMSETTINGSANDCAMERA_HH
#define FILMSETTINGSANDCAMERA_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
    class FilmSettingsAndCamera;
}



class FilmSettingsAndCamera : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit FilmSettingsAndCamera(QWidget *parent = 0);
        virtual ~FilmSettingsAndCamera();

        void setFilmSettingsAndCamera(
            redshift::shared_ptr<cosyscene::FilmSettings>,
            redshift::shared_ptr<cosyscene::Camera>,
            bool blockSignals = true
        );

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

signals:
        void filmSettingsChanged();
        void cameraChanged();

// ================== Private ==================================================
private slots:
        void on_filmSettings_filmSettingsChanged();
        void on_camera_cameraChanged();

private:
        Ui::FilmSettingsAndCamera *ui;
};

#endif // FILMSETTINGSANDCAMERA_HH
