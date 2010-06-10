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

#include "redshift/include/setup.hh"

#include <iostream>
#include "tristimuluscolorpicker.hh"
#include "ui_tristimuluscolorpicker.h"

#include "redshift/include/setup.hh"

TristimulusColorPicker::TristimulusColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TristimulusColorPicker),
    isUpdating(false)
{
        ui->setupUi(this);
        setColor(TristimulusColor::fromRgbf(-10, -9, -8));
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
        TristimulusColor scaledColor;
        scaledColor.setRedF  (color_.redF()  *range + min);
        scaledColor.setGreenF(color_.greenF()*range + min);
        scaledColor.setBlueF (color_.blueF() *range + min);
        return scaledColor;
}


//------------------------------------------------------------------------------
// Update by wheel
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_triangle_colorChanged(const QColor & color) {
        setColor_(TristimulusColor(color));
}


//------------------------------------------------------------------------------
// Update by RGB spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinR_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setRedF(v/255);
        setColor_ (col);
}
void TristimulusColorPicker::on_spinG_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setGreenF(v/255);
        setColor_ (col);
}
void TristimulusColorPicker::on_spinB_valueChanged(double v) {
        TristimulusColor col = color_;
        col.setBlueF(v/255);
        setColor_ (col);
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
        setColor_ (TristimulusColor(col));

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
        setColor_ (TristimulusColor(col));

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




void TristimulusColorPicker::setColor(TristimulusColor const &from) {
        TristimulusColor col_;
        double min, max;
        from.makeConvertibleToQColor(col_, min, max);
        ui->spinMin->setValue(min);
        ui->spinMax->setValue(max);
        setColor_(col_);
}



void TristimulusColorPicker::setColor_(const TristimulusColor &from) {

        if (isUpdating)
                return;
        isUpdating = true;

        const bool prevBlockSignals = blockSignals(true);

        const QColor col = from.toQColor();

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


        color_ = from;
        drawColorPreview();

        blockSignals(prevBlockSignals);

        emit colorChanged (color());
        isUpdating = false;
}



void TristimulusColorPicker::drawColorPreview() {
        QImage img(ui->colorPreview->width(),32,QImage::Format_RGB32);
        QImage scaledImg(ui->scaledColorPreview->width(),32,QImage::Format_RGB32);
        img.fill(color_.toQColor().rgb());

        double sr = color_.redF() * range + min;
        double sg = color_.greenF() * range + min;
        double sb = color_.blueF() * range + min;

        const redshift::ReferenceSpectrum ref =
                        redshift::ReferenceSpectrum::
                        FromRGB(sr,sg,sb,redshift::ReflectanceSpectrum);
        const redshift::color::RGB redRGB = ref.toRGB();

        sr = redRGB.R;
        sg = redRGB.G;
        sb = redRGB.B;

        scaledImg.fill(QColor::fromRgbF(
                        sr<0 ? 0 : sr>1 ? 1 : sr,
                        sg<0 ? 0 : sg>1 ? 1 : sg,
                        sb<0 ? 0 : sb>1 ? 1 : sb).rgb());
        const QRgb black = 0U;
        for (int i=0; i<img.height(); ++i) {
                img.setPixel(0, i, black);
                img.setPixel(img.width()-1, i, black);
                scaledImg.setPixel(0, i, black);
                scaledImg.setPixel(img.width()-1, i, black);
        }
        for (int i=0; i<img.width(); ++i) {
                img.setPixel(i, 0, black);
                img.setPixel(i, img.height()-1, black);
                scaledImg.setPixel(i, 0, black);
                scaledImg.setPixel(i, img.height()-1, black);
        }
        ui->colorPreview->setPixmap(QPixmap::fromImage(img));
        ui->scaledColorPreview->setPixmap(QPixmap::fromImage(scaledImg));
}



void TristimulusColorPicker::resizeEvent(QResizeEvent *) {
        drawColorPreview();
}



void TristimulusColorPicker::on_spinMin_valueChanged(double ) {
        min = ui->spinMin->value();
        max = ui->spinMax->value();
        range = max - min;
        drawColorPreview();
        emit colorChanged(color());
}
void TristimulusColorPicker::on_spinMax_valueChanged(double ) {
        on_spinMin_valueChanged(double());
}
