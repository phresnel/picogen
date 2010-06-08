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

#include <iostream>
#include "tristimuluscolorpicker.hh"
#include "ui_tristimuluscolorpicker.h"

TristimulusColorPicker::TristimulusColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TristimulusColorPicker),
    isUpdating(false)
{
        ui->setupUi(this);
        setColor(TristimulusColor::fromRgbf(0,1,0));
}

TristimulusColorPicker::~TristimulusColorPicker() {
        delete ui;
}

void TristimulusColorPicker::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

TristimulusColor TristimulusColorPicker::color() const {
        return color_;
}


//------------------------------------------------------------------------------
// Update by wheel
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_triangle_colorChanged(const QColor & color) {
        setColor(TristimulusColor(color));
}


//------------------------------------------------------------------------------
// Update by RGB spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinR_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setRedF(v/255);
        setColor (col);
}
void TristimulusColorPicker::on_spinG_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setGreenF(v/255);
        setColor (col);
}
void TristimulusColorPicker::on_spinB_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setBlueF(v/255);
        setColor (col);
}


//------------------------------------------------------------------------------
// Update by HSV spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinH_valueChanged(double) {
        // 0) Save box values. They might change during setColor().
        const double
                h = ui->spinH->value(),
                s = ui->spinS->value(),
                v = ui->spinV->value();

        // 1) Build up QColor from boxes.
        QColor col = QColor::fromHsvF(h/360, s/255, v/255);
        setColor (TristimulusColor(col));

        // 2) Block signals or we'll never end.
        const bool
                H = ui->spinH->blockSignals(true),
                S = ui->spinS->blockSignals(true),
                V = ui->spinV->blockSignals(true),
                T = ui->triangle->blockSignals(true);

        // 3) setColor() might have changed the box values, so restore them,
        //    and also restore the previous blocking state.
        ui->spinH->setValue(h);
        ui->spinS->setValue(s);
        ui->spinV->setValue(v);
        ui->triangle->setColor(col);

        ui->spinH->blockSignals(H);
        ui->spinS->blockSignals(S);
        ui->spinV->blockSignals(V);
        ui->triangle->blockSignals(T);
}
void TristimulusColorPicker::on_spinS_valueChanged(double v) {
        on_spinH_valueChanged(double());
}
void TristimulusColorPicker::on_spinV_valueChanged(double v) {
        on_spinH_valueChanged(double());
}




//------------------------------------------------------------------------------
// Update by CMYK spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinC_valueChanged(double) {
        // 0) Save box values. They might change during setColor().
        const double
                c = ui->spinC->value(),
                m = ui->spinM->value(),
                y = ui->spinY->value(),
                k = ui->spinK->value();

        // 1) Build up QColor from boxes.
        QColor col = QColor::fromCmykF(c/255, m/255, y/255, k/255);
        setColor (TristimulusColor(col));

        // 2) Block signals or we'll never end.
        const bool
                C = ui->spinC->blockSignals(true),
                M = ui->spinM->blockSignals(true),
                Y = ui->spinY->blockSignals(true),
                K = ui->spinK->blockSignals(true),
                T = ui->triangle->blockSignals(true);

        // 3) setColor() might have changed the box values, so restore them,
        //    and also restore the previous blocking state.
        ui->spinC->setValue(c);
        ui->spinM->setValue(m);
        ui->spinY->setValue(y);
        ui->spinK->setValue(k);
        ui->triangle->setColor(col);

        ui->spinC->blockSignals(C);
        ui->spinM->blockSignals(M);
        ui->spinY->blockSignals(Y);
        ui->spinK->blockSignals(K);
        ui->triangle->blockSignals(T);
}
void TristimulusColorPicker::on_spinM_valueChanged(double) {
        on_spinC_valueChanged(double());
}
void TristimulusColorPicker::on_spinY_valueChanged(double) {
        on_spinC_valueChanged(double());
}
void TristimulusColorPicker::on_spinK_valueChanged(double) {
        on_spinC_valueChanged(double());
}




void TristimulusColorPicker::setColor(TristimulusColor const &col_) {
        if (isUpdating)
                return;
        isUpdating = true;

        const bool prevBlockSignals = blockSignals(true);
        const QColor col = col_.toQColor();


        bool prev = ui->triangle->blockSignals(true);
        ui->triangle->setColor(col);
        ui->triangle->blockSignals(prev);

        // RGB
        ui->spinR->blockSignals(true);
        ui->spinR->setValue(col.redF() * 255);
        ui->spinR->blockSignals(prev);

        prev = ui->spinG->blockSignals(true);
        ui->spinG->setValue(col.greenF() * 255);
        ui->spinG->blockSignals(prev);

        prev = ui->spinB->blockSignals(true);
        ui->spinB->setValue(col.blueF() * 255);
        ui->spinB->blockSignals(prev);

        // HSV
        prev = ui->spinH->blockSignals(true);
        ui->spinH->setValue(col.hsvHueF() * 360);
        ui->spinH->blockSignals(prev);

        prev = ui->spinS->blockSignals(true);
        ui->spinS->setValue(col.hsvSaturationF() * 255);
        ui->spinS->blockSignals(prev);

        prev = ui->spinV->blockSignals(true);
        ui->spinV->setValue(col.valueF() * 255);
        ui->spinV->blockSignals(prev);

        // CMYK
        prev = ui->spinC->blockSignals(true);
        ui->spinC->setValue(col.cyanF() * 255);
        ui->spinC->blockSignals(prev);

        prev = ui->spinM->blockSignals(true);
        ui->spinM->setValue(col.magentaF() * 255);
        ui->spinM->blockSignals(prev);

        prev = ui->spinY->blockSignals(true);
        ui->spinY->setValue(col.yellowF() * 255);
        ui->spinY->blockSignals(prev);

        prev = ui->spinK->blockSignals(true);
        ui->spinK->setValue(col.blackF() * 255);
        ui->spinK->blockSignals(prev);


        color_ = col_;

        blockSignals(prevBlockSignals);
        emit colorChanged (color_);
        isUpdating = false;
}

