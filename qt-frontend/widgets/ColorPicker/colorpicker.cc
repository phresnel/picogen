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



#include "colorpicker.hh"
#include "ui_colorpicker.h"

ColorPicker::ColorPicker(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ColorPicker)
{
        ui->setupUi(this);

        on_tabWidget_currentChanged(ui->tabWidget->currentIndex());

        connect (ui->tristimulusPicker, SIGNAL(colorChanged(QColor)),
                 this, SLOT(tristimulusColorChanged(QColor)));
        connect (ui->spectralPicker, SIGNAL(colorChanged(QVector<SpectralSample> const &)),
                 this, SLOT(spectralColorChanged(QVector<SpectralSample> const &)));

        color_.spectral = ui->spectralPicker->samples();
        color_.tristimulus = ui->tristimulusPicker->color();
}

ColorPicker::~ColorPicker() {
        delete ui;
}

void ColorPicker::changeEvent(QEvent *e) {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void ColorPicker::on_okayButton_pressed() {
        setResult(QDialog::Accepted);
        close();
}

void ColorPicker::on_cancelButton_pressed() {
        setResult(QDialog::Rejected);
        close();
}

void ColorPicker::setColor (ColorPickerColor const &color) {
        this->color_ = color;

        switch (color.mode) {
        case ColorPickerColor::Tristimulus:
                ui->tabWidget->setCurrentIndex(0);
                ui->tristimulusPicker->setColor(color.tristimulus);
                break;
        case ColorPickerColor::Spectral:
                ui->tabWidget->setCurrentIndex(1);
                ui->spectralPicker->setSamples(color.spectral);
                break;
        }
}

ColorPickerColor ColorPicker::color () const {
        return color_;
}

void ColorPicker::on_tabWidget_currentChanged(int index) {
        switch (index) {
        case 0:
                color_.mode = ColorPickerColor::Tristimulus;
                break;
        case 1:
                color_.mode = ColorPickerColor::Spectral;
                break;
        }
}

void ColorPicker::tristimulusColorChanged (QColor c) {
        color_.tristimulus = c;
}

void ColorPicker::spectralColorChanged (QVector<SpectralSample> const & c) {
        color_.spectral = c;
}
