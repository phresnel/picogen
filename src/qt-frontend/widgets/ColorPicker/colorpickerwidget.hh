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

#ifndef COLORPICKERWIDGET_HH
#define COLORPICKERWIDGET_HH

#include <QWidget>

#include "colorpickercolor.hh"

namespace Ui {
    class ColorPickerWidget;
}

class ColorPickerWidget : public QWidget
{
        Q_OBJECT

public:
        explicit ColorPickerWidget(QWidget *parent = 0);
        ~ColorPickerWidget();

        void setColor (ColorPickerColor const &color);
        ColorPickerColor color () const;

signals:
        void colorChanged (ColorPickerColor const &);

private:
        Ui::ColorPickerWidget *ui;
        ColorPickerColor color_;

private slots:
        void on_spectralRadioButton_toggled(bool checked);
        void on_tristimulusRadioButton_toggled(bool checked);
        void tristimulusColorChanged (TristimulusColor);
        void spectralColorChanged (QVector<SpectralSample> const & c);
};

#endif // COLORPICKERWIDGET_HH
