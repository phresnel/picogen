//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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



#ifndef COLORPICKER_HH
#define COLORPICKER_HH

#include <iostream>
#include <cmath>
#include <QWidget>
#include <QtOpenGL>

#include "colorwheelwidget.hh"
#include "colorplanewidget.hh"
#include "color.hh"

namespace Ui {
        class ColorPicker;
}

class ColorPicker : public QWidget {
        Q_OBJECT
public:
        ColorPicker(QWidget *parent = 0);
        ~ColorPicker();

protected:
        void changeEvent(QEvent *e);
        void paintEvent(QPaintEvent*);

private:
        Ui::ColorPicker *ui;

        enum Mode {
                wheel
        };
        Mode mode;

        color::rgbf currentColor;

        ColorWheelWidget *colorWheel;
        ColorPlaneWidget *colorPlane;

signals:
        void currentColorChanged (color::rgbf col);

private slots:
        void on_spinColorV_valueChanged(int );
        void on_spinColorS_valueChanged(int );
        void on_spinColorH_valueChanged(int );
        void setCurrentColor (color::rgbf col);
};

#endif // COLORPICKER_HH
