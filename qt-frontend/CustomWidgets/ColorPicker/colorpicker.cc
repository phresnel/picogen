//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include <QImage>
#include <QPainter>
#include <QColor>
#include <QtOpenGL>
#include "colorpicker.hh"
#include "ui_colorpicker.h"
#include <cmath>


#include <qgl.h>
#include <qapplication.h>
#include <qimage.h>
#include <iostream>





ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPicker)
{
        ui->setupUi(this);

        ui->wheelTab->addWidget(
                        colorWheel = new ColorWheelWidget (this));


        QObject::connect(colorWheel, SIGNAL(currentColorChanged(color::rgbf)),
                         this, SLOT(setCurrentColor(color::rgbf))
        );

        QObject::connect(this, SIGNAL(currentColorChanged(color::rgbf)),
                         colorWheel, SLOT(setCurrentColor(color::rgbf))
        );

        //currentColor
}



ColorPicker::~ColorPicker() {
    delete ui;
}



void ColorPicker::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void ColorPicker::setCurrentColor (color::rgbf col) {
        currentColor = col;
        color::hsvf hsv = color::rgb_to_hsv(currentColor);
        ui->spinColorH->setValue((int)hsv.h);
        ui->spinColorS->setValue((int)(hsv.s*100.0f));
        ui->spinColorV->setValue((int)(hsv.v*100.0f));
        repaint();
        emit currentColorChanged(currentColor);
}

void ColorPicker::on_spinColorH_valueChanged(int val) {
        color::hsvf hsv = color::rgb_to_hsv(currentColor);
        hsv.h = val;
        currentColor = color::hsv_to_rgb(hsv);
        setCurrentColor (currentColor);
        emit currentColorChanged(currentColor);
}

void ColorPicker::on_spinColorS_valueChanged(int val) {
        color::hsvf hsv = color::rgb_to_hsv(currentColor);
        hsv.s = val / 100.0f;
        currentColor = color::hsv_to_rgb(hsv);
        setCurrentColor (currentColor);
        emit currentColorChanged(currentColor);
}

void ColorPicker::on_spinColorV_valueChanged(int val) {
        color::hsvf hsv = color::rgb_to_hsv(currentColor);
        hsv.v = val / 100.0f;
        currentColor = color::hsv_to_rgb(hsv);
        setCurrentColor (currentColor);
        emit currentColorChanged(currentColor);
}



void ColorPicker::paintEvent(QPaintEvent*) {
        const color::hsvf currentHsv = rgb_to_hsv (currentColor);
        {
                QImage img (
                        ui->colorH->width(), ui->colorH->height(),
                        QImage::Format_RGB32);
                QPainter painter (&img);
                for (int x=0; x<ui->colorH->width(); ++x) {
                        const float h = x / float(ui->colorH->width()) * 360.0f;
                        const color::rgbf col = color::hsv_to_rgb(
                                        h, currentHsv.s, currentHsv.v);
                        painter.setPen(QColor(
                            (int)(col.r*255), (int)(col.g*255), (int)(col.b*255)
                        ));
                        painter.drawLine(
                                QPointF(x, 0),
                                QPointF(x, ui->colorH->height())
                        );

                }
                ui->colorH->setPixmap(QPixmap::fromImage(img));
        }
}
