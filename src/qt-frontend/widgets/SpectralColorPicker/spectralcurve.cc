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

#include <QPainter>
#include <QPaintEvent>

#include "spectralcurve.hh"
#include "ui_spectralcurve.h"
#include "createspectralimage.hh"

SpectralCurve::SpectralCurve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectralCurve),
    energyMin(0), energyMax(1)
{
    ui->setupUi(this);
    spectralImage = createSpectralImage(64);
}



SpectralCurve::~SpectralCurve()
{
    delete ui;
}



void SpectralCurve::setEnergyRange(double min, double max) {
        energyMin = min;
        energyMax = max;
}



void SpectralCurve::setMinEnergy (double e) {
        energyMin = e;
}



void SpectralCurve::setMaxEnergy (double e) {
        energyMax = e;
}



void SpectralCurve::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void SpectralCurve::paintEvent (QPaintEvent *e) {
        using redshift::ReferenceSpectrum;
        using redshift::color::RGB;
        using redshift::color::SRGB;

        QPainter p(this);
        p.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);

        p.drawImage(QRectF(0,0, width(), height()), spectralImage);

        p.setPen(QPen(QBrush(Qt::black), 2.f));
        p.drawRect(0,0,width(),height());

        p.setPen(QPen(QBrush(Qt::black), 1.f));
        QPainterPath pp(QPointF(0,0));
        const int size = spectrum.size();
        for (int s=0; s<size; ++s) {
                const double e = (spectrum[s]-energyMin)/(energyMax-energyMin);
                const double y0 = height()-height()*e;
                const double x0 = (s/(double)(size-1)) * width();

                if (s==0) {
                        pp.moveTo(x0, y0);
                }
                pp.lineTo(x0, y0);
        }
        const double e1 = (1-energyMin)/(energyMax-energyMin);
        const double e0 = (0-energyMin)/(energyMax-energyMin);
        p.drawPath(pp);

        QPainterPath mm;
        mm.moveTo(0., height()-height()*e1);
        mm.lineTo(width(), height()-height()*e1);
        mm.moveTo(0., height()-height()*e0);
        mm.lineTo(width(), height()-height()*e0);
        p.setPen(QPen(QBrush(QColor(0,0,0,150)), 2.f));
        p.drawPath(mm);

        p.setPen(QPen(QBrush(QColor(255,255,255,150)), 2.f));
        p.drawText(QPointF(0., (height()-height()*e1)+5), "1");
        p.drawText(QPointF(0., (height()-height()*e0)+5), "0");

        e->accept();
}



void SpectralCurve::setSpectrum (SpectralColorPicker::Spectrum  const &spectrum) {
        this->spectrum = spectrum;
}
