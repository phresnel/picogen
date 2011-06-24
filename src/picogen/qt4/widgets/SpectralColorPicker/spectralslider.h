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


#ifndef SPECTRALSLIDER_HH
#define SPECTRALSLIDER_HH

#include <QWidget>

namespace Ui {
    class SpectralSlider;
}

namespace picogen { namespace qt4 {

class SpectralSlider : public QWidget {
        Q_OBJECT
public:
        SpectralSlider(double wavelength, QWidget *parent = 0);
        ~SpectralSlider();

        void setAmplitude (double);
        double amplitude () const;

        void setWavelength (double);
        double wavelength () const;

signals:
        void valueChanged (double amplitude, double wavelength);

public slots:
        void minimumChanged (double val);
        void maximumChanged (double val);

protected:
        void changeEvent(QEvent *e);

private:
        Ui::SpectralSlider *ui;

private slots:
        void on_wavelength_valueChanged(double );
        void on_amplitude_valueChanged(double );
        void on_verticalSlider_valueChanged(int value);
};

} }

#endif // SPECTRALSLIDER_HH
