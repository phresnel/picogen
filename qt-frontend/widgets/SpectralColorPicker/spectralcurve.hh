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

#ifndef SPECTRALCURVE_HH
#define SPECTRALCURVE_HH

#include <QWidget>

namespace Ui {
    class SpectralCurve;
}

#include "spectrumdisplay.hh"
#include "spectralcolorpicker.hh"

class SpectralCurve : public QWidget {
    Q_OBJECT
public:
    SpectralCurve(QWidget *parent = 0);
    ~SpectralCurve();

    void setSpectrum (SpectralColorPicker::Spectrum const &spectrum);
    void setEnergyRange (double min, double max);
    void setMinEnergy (double e);
    void setMaxEnergy (double e);

protected:
    void changeEvent(QEvent *e);
    void paintEvent (QPaintEvent *);

private:
    Ui::SpectralCurve *ui;
    QImage spectralImage;
    SpectralColorPicker::Spectrum spectrum;
    double energyMin, energyMax;
};

#endif // SPECTRALCURVE_HH
