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


#include "spectralslider.hh"
#include "ui_spectralslider.h"

SpectralSlider::SpectralSlider(double wavelength, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectralSlider)
{
        ui->setupUi(this);
        setWavelength(wavelength);
        setAmplitude(0.5);
}



SpectralSlider::~SpectralSlider() {
        delete ui;
}



void SpectralSlider::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void SpectralSlider::setAmplitude (double value) {
        ui->amplitude->setValue(value);
        ui->verticalSlider->setValue(value * 100.f);
}



double SpectralSlider::amplitude () const {
        return ui->amplitude->value();
}


void SpectralSlider::setWavelength (double value) {
        ui->wavelength->setValue(value);
}

double SpectralSlider::wavelength () const {
        return ui->wavelength->value();
}


void SpectralSlider::on_verticalSlider_valueChanged(int value) {
        ui->amplitude->setValue(value / 100.f);
        emit valueChanged(ui->amplitude->value(), ui->wavelength->value());
}

void SpectralSlider::on_amplitude_valueChanged(double) {
        emit valueChanged(ui->amplitude->value(), ui->wavelength->value());
}

void SpectralSlider::on_wavelength_valueChanged(double) {
        emit valueChanged(ui->amplitude->value(), ui->wavelength->value());
}

void SpectralSlider::minimumChanged (double val) {
        ui->amplitude->setMinimum(val);
        ui->verticalSlider->setMinimum(val * 100.);
}

void SpectralSlider::maximumChanged (double val) {
        ui->amplitude->setMaximum(val);
        ui->verticalSlider->setMaximum(val * 100.);
}
