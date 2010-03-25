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
    ui(new Ui::SpectralCurve)
{
    ui->setupUi(this);
    spectralImage = createSpectralImage();
}



SpectralCurve::~SpectralCurve()
{
    delete ui;
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
                const double y0 = height()-height()*spectrum[s];
                const double x0 = (s/(double)(size-1)) * width();

                if (s==0) {
                        pp.moveTo(x0, y0);
                }
                pp.lineTo(x0, y0);

                /*const double s1 = s+1<size ? s+1 : s;
                const double y1 = height()-height()*spectrum[s1];
                const double x1 = ((s1)/(double)(size-1)) * width();

                const double s2 = s1+1<size ? s1+1 : s1;
                const double y2 = height()-height()*spectrum[s2];
                const double x2 = ((s2)/(double)(size-1)) * width();*/
                //pp.cubicTo(x0, y0, x1, y1, x2, y2);
        }
        p.drawPath(pp);

        e->accept();
}



void SpectralCurve::setSpectrum (redshift::LongSpectrum const &spectrum) {
        this->spectrum = spectrum;
}
