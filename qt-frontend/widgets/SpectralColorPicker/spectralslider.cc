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
    ui(new Ui::SpectralSlider),
    wavelength_(wavelength)
{
        ui->setupUi(this);
        setTitle (QString::number(wavelength_));
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



void SpectralSlider::setTitle (QString const &text) {
        ui->label->setText(text);
}



QString SpectralSlider::title () const {
        return ui->label->text();
}



void SpectralSlider::setAmplitude (double value) {
        ui->doubleSpinBox->setValue(value);
        ui->verticalSlider->setValue(value * 100.f);
}



double SpectralSlider::amplitude () const {
        return ui->doubleSpinBox->value();
}


void SpectralSlider::setWavelength (double value) {
        wavelength_ = value;
        setTitle (QString::number(wavelength_));
}

double SpectralSlider::wavelength () const {
        return wavelength_;
}


void SpectralSlider::on_verticalSlider_valueChanged(int value) {
        ui->doubleSpinBox->setValue(value / 100.f);
        emit amplitudeChanged(value / 100.f, wavelength_);
}

void SpectralSlider::on_doubleSpinBox_valueChanged(double value) {
        ui->verticalSlider->setValue(value * 100.f);
        emit amplitudeChanged(value, wavelength_);
}
