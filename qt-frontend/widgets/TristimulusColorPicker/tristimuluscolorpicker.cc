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

#include "tristimuluscolorpicker.hh"
#include "ui_tristimuluscolorpicker.h"

TristimulusColorPicker::TristimulusColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TristimulusColorPicker),
    isUpdating(false)
{
        ui->setupUi(this);
        setColor(Qt::green);
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

QColor TristimulusColorPicker::color() const {
        return color_;
}


//------------------------------------------------------------------------------
// Update by wheel
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_triangle_colorChanged(const QColor & color) {
        setColor(color);
}


//------------------------------------------------------------------------------
// Update by RGB spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinR_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col.setRed(v);

        setColor (col);
}
void TristimulusColorPicker::on_spinG_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col.setGreen(v);

        setColor (col);
}
void TristimulusColorPicker::on_spinB_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col.setBlue(v);

        setColor (col);
}


//------------------------------------------------------------------------------
// Update by HSV spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinH_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromHsv(v, col.hsvSaturation(), col.value());

        setColor (col);
}
void TristimulusColorPicker::on_spinS_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromHsv(col.hsvHue(), v, col.value());

        setColor (col);
}
void TristimulusColorPicker::on_spinV_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromHsv(col.hsvHue(), col.hsvSaturation(), v);

        setColor (col);
}




//------------------------------------------------------------------------------
// Update by CMYK spins
//------------------------------------------------------------------------------
void TristimulusColorPicker::on_spinC_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromCmyk(v, col.magenta(), col.yellow(), col.black());

        setColor (col);
}
void TristimulusColorPicker::on_spinM_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromCmyk(col.cyan(), v, col.yellow(), col.black());

        setColor (col);
}
void TristimulusColorPicker::on_spinY_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromCmyk(col.cyan(), col.magenta(), v, col.black());

        setColor (col);
}
void TristimulusColorPicker::on_spinK_valueChanged(int v) {
        QColor col = ui->triangle->color();
        col = QColor::fromCmyk(col.cyan(), col.magenta(), col.yellow(), v);

        setColor (col);
}




void TristimulusColorPicker::setColor(QColor const &col) {
        if (isUpdating)
                return;
        isUpdating = true;

        ui->triangle->setColor(col);

        ui->spinR->setValue(col.red());
        ui->spinG->setValue(col.green());
        ui->spinB->setValue(col.blue());

        ui->spinH->setValue(col.hsvHue());
        ui->spinS->setValue(col.hsvSaturation());
        ui->spinV->setValue(col.value());

        ui->spinC->setValue(col.cyan());
        ui->spinM->setValue(col.magenta());
        ui->spinY->setValue(col.yellow());
        ui->spinK->setValue(col.black());

        color_ = col;

        emit colorChanged (color_);
        isUpdating = false;
}

