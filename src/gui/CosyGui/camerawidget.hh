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

#ifndef CAMERAWIDGET_HH
#define CAMERAWIDGET_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"

namespace Ui {
    class CameraWidget;
}

namespace cosyscene {
         class Camera;
         class Scene;
}

class CameraWidget : public QWidget
{
        Q_OBJECT

public:
        explicit CameraWidget(QWidget *parent = 0);
        ~CameraWidget();

        void setCamera (redshift::shared_ptr<cosyscene::Camera>,
                        bool blockSignals=true);

signals:
        void cameraChanged();

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

private:
        Ui::CameraWidget *ui;
        redshift::shared_ptr<cosyscene::Camera> camera_;

private:
        void updateViews(bool blockSignals=true);
        void showPinholeParameters();
        void showCylindricalParameters();
        void showCubemapFaceParameters();

private slots:
        void on_cubemapFace_currentIndexChanged(int index);
        void on_cylindricalFrontPlaneDistance_valueChanged(double);
        void on_pinholeFrontPlaneDistance_valueChanged(double);
        void on_cameraKind_currentIndexChanged(int index);
        void on_stashButton_clicked();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked();
};

#endif // CAMERAWIDGET_HH
