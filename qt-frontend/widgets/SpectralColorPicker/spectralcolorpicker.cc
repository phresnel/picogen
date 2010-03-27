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


#include <fstream>
#include <QMessageBox>

#include "spectralcolorpicker.hh"
#include "spectralslider.hh"
#include "ui_spectralcolorpicker.h"

#include "importrawdatawizard.hh"


SpectralColorPicker::SpectralColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectralColorPicker)
{
        ui->setupUi(this);

        on_lockSampleCount_toggled(ui->lockSampleCount->checkState());
        addSpectralSliders(ui->sampleCount->value());
}



SpectralColorPicker::~SpectralColorPicker() {
        delete ui;
}



void SpectralColorPicker::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void SpectralColorPicker::amplitudeChanged (double amp, double wavelength) {
        Q_UNUSED(amp)
        Q_UNUSED(wavelength)

        using redshift::color::RGB;
        using redshift::color::SRGB;

        const Spectrum spectrum = spectrumFromSliders();
        ui->spectralCurve->setSpectrum(spectrum);
        ui->spectralCurve->update();

        const RGB rgb_ = spectrum.toRGB();
        const SRGB rgb = rgb_.toSRGB();
        const int
                R_ = rgb.R * 255,
                G_ = rgb.G * 255,
                B_ = rgb.B * 255,
                R = R_ < 0 ? 0 : R_ > 255 ? 255 : R_,
                G = G_ < 0 ? 0 : G_ > 255 ? 255 : G_,
                B = B_ < 0 ? 0 : B_ > 255 ? 255 : B_
                ;

        QImage i(Spectrum::num_components,32,QImage::Format_RGB32);
        i.fill(QColor(R,G,B).rgb());

        ui->colorPreview->setPixmap(QPixmap::fromImage(i));
}



void SpectralColorPicker::on_sampleCount_valueChanged(int n) {
        removeSpectralSliders();
        addSpectralSliders(n);
}



SpectralColorPicker::Spectrum SpectralColorPicker::spectrumFromSliders () const {
        using redshift::color::RGB;
        using redshift::color::SRGB;

        std::vector<Spectrum::real_t> amplitudes, wavelengths;
        foreach (SpectralSlider *slider, sliders) {
                amplitudes.push_back(slider->amplitude());
                wavelengths.push_back(slider->wavelength());
        }

        return Spectrum::FromSampled(
                &amplitudes[0],
                &wavelengths[0],
                amplitudes.size());
}



void SpectralColorPicker::removeSpectralSliders() {
        foreach (SpectralSlider *slider, sliders) {
                ui->slidersLayout->removeWidget(slider);
                delete slider;
        }
        sliders.clear();
}



void SpectralColorPicker::addSpectralSliders(
        unsigned int n
) {
        const unsigned int num_sliders = n;
        const double min = 400;
        const double max = 700;
        for (unsigned int i=0; i<num_sliders; ++i) {
                const double lambda = (i/(double)(num_sliders-1))*(max-min)+min;
                SpectralSlider *w = new SpectralSlider (lambda, this);
                sliders.push_back(w);
                ui->slidersLayout->addWidget(w);

                connect (w, SIGNAL(amplitudeChanged(double,double)),
                         this, SLOT(amplitudeChanged(double,double)));
        }
}



void SpectralColorPicker::on_lockSampleCount_toggled(bool checked) {
        ui->sampleCount->setEnabled(!checked);
}



void SpectralColorPicker::on_importRawDataButton_pressed() {
        ImportRawDataWizard *wiz = new ImportRawDataWizard(this);
        if (wiz->exec()) {
                QVector<QPair<double,double> > const &
                                converted = wiz->converted();
                const int count = converted.count();

                const Spectrum spec = Spectrum::FromSampled (
                                        &wiz->amplitudes()[0],
                                        &wiz->wavelengths()[0],
                                        count
                                );

                removeSpectralSliders();
                addSpectralSliders(converted.count());

                for (int i=0; i<count; ++i) {
                        sliders[i]->setWavelength (wiz->wavelengths()[i]);
                        sliders[i]->setAmplitude (wiz->amplitudes()[i]);
                }
        }
}
