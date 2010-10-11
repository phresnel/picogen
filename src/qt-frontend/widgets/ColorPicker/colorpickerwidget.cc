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



#include "colorpickerwidget.hh"
#include "ui_colorpickerwidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPickerWidget)
{
        ui->setupUi(this);

        ui->tristimulusRadioButton->setChecked(true);

        connect (ui->tristimulusPicker, SIGNAL(colorChanged(TristimulusColor)),
                 this, SLOT(tristimulusColorChanged(TristimulusColor)));
        connect (ui->spectralPicker, SIGNAL(colorChanged(QVector<SpectralSample> const &)),
                 this, SLOT(spectralColorChanged(QVector<SpectralSample> const &)));

        color_.spectral = ui->spectralPicker->samples();
        color_.tristimulus = ui->tristimulusPicker->color();
}

ColorPickerWidget::~ColorPickerWidget() {
        delete ui;
}

void ColorPickerWidget::tristimulusColorChanged (TristimulusColor c) {
        color_.tristimulus = c;
}

void ColorPickerWidget::spectralColorChanged (QVector<SpectralSample> const & c) {
        color_.spectral = c;
}

void ColorPickerWidget::setColor (ColorPickerColor const &color) {
        this->color_ = color;

        ui->tristimulusPicker->setColor(color.tristimulus);
        ui->spectralPicker->setSamples(color.spectral);

        switch (color.mode) {
        case ColorPickerColor::Tristimulus:
                ui->stackedWidget->setCurrentIndex(0);
                break;
        case ColorPickerColor::Spectral:
                ui->stackedWidget->setCurrentIndex(1);
                break;
        }
}

ColorPickerColor ColorPickerWidget::color () const {
        return color_;
}

void ColorPickerWidget::on_tristimulusRadioButton_toggled(bool checked) {
        if (checked) {
                color_.mode = ColorPickerColor::Tristimulus;
                ui->stackedWidget->setCurrentIndex(0);
        }
}

void ColorPickerWidget::on_spectralRadioButton_toggled(bool checked) {
        if (checked) {
                color_.mode = ColorPickerColor::Spectral;
                ui->stackedWidget->setCurrentIndex(1);
        }
}
