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
#include "spectrumdisplay.hh"
#include "ui_spectrumdisplay.h"

#include "redshift/include/setup.hh"

SpectrumDisplay::SpectrumDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectrumDisplay)
{
        ui->setupUi(this);


        // Below method is extremely wasteful, better write a
        // setAmplitude(wave,amp)-method for spectrum
        using redshift::ReferenceSpectrum;
        using redshift::color::RGB;
        using redshift::color::SRGB;

        ReferenceSpectrum::real_t
                wavs[ReferenceSpectrum::num_components] = { 0.0 };
        for (int x=0; x<ReferenceSpectrum::num_components; ++x) {
                wavs[x] = redshift::SAMPLED_LAMBDA_START+x;
        }

        QImage image (ReferenceSpectrum::num_components, 32, QImage::Format_RGB32);
        for (int x=0; x<ReferenceSpectrum::num_components; ++x) {

                ReferenceSpectrum::real_t
                        amps[ReferenceSpectrum::num_components] = { 0.0 };
                amps[x] = 1;

                const ReferenceSpectrum spectrum = ReferenceSpectrum::FromSampled(
                                                                amps,
                                                                wavs,
                                                                ReferenceSpectrum::num_components);
                //const redshift::real_t y = spectrum.y();
                const RGB rgb__ = spectrum.toRGB();
                const RGB rgb_ (rgb__.R*250, rgb__.G*250, rgb__.B*250);
                const SRGB rgb = rgb_.toSRGB();
                const int
                        R_ = rgb.R * 255,
                        G_ = rgb.G * 255,
                        B_ = rgb.B * 255,
                        R = R_ < 0 ? 0 : R_ > 255 ? 255 : R_,
                        G = G_ < 0 ? 0 : G_ > 255 ? 255 : G_,
                        B = B_ < 0 ? 0 : B_ > 255 ? 255 : B_
                        ;
                const unsigned int col = QColor(R,G,B).rgb();

                for (int y=0; y<image.height(); ++y) {
                        image.setPixel(x, y, col);
                }
        }

        ui->label->setPixmap(QPixmap::fromImage(image));

        spectralImage = image;
}



#include <QPaintEvent>
void SpectrumDisplay::paintEvent(QPaintEvent *e) {
        //spectralImage.
        e->ignore();
}



SpectrumDisplay::~SpectrumDisplay() {
        delete ui;
}



void SpectrumDisplay::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}
