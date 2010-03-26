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


#ifndef SPECTRALCOLORPICKER_HH
#define SPECTRALCOLORPICKER_HH

#include <QWidget>
#include "redshift/include/setup.hh"


namespace Ui {
    class SpectralColorPicker;
}


class SpectralSlider;
class SpectralColorPicker : public QWidget {
    Q_OBJECT
public:
        SpectralColorPicker(QWidget *parent = 0);
        ~SpectralColorPicker();

protected:
        void changeEvent(QEvent *e);

private slots:
        void on_lockSampleCount_toggled(bool checked);
        void on_sampleCount_valueChanged(int );
        void amplitudeChanged (double amplitude, double wavelength);

private:
        Ui::SpectralColorPicker *ui;
        QVector<SpectralSlider*> sliders;

        void removeSpectralSliders();
        void addSpectralSliders(unsigned int i);

        typedef redshift::LongSpectrum Spectrum;
        Spectrum spectrumFromSliders () const;
};

#endif // SPECTRALCOLORPICKER_HH
