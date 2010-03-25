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
#include <QPaintEvent>

#include "spectrumdisplay.hh"
#include "ui_spectrumdisplay.h"
#include "createspectralimage.hh"

SpectrumDisplay::SpectrumDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpectrumDisplay)
{
        ui->setupUi(this);

        spectralImage = createSpectralImage();
        ui->label->setPixmap(QPixmap::fromImage(spectralImage));
}



void SpectrumDisplay::paintEvent(QPaintEvent *e) {
        //spectralImage.
        e->ignore();
}



double SpectrumDisplay::scaledWidth() const {
        return ui->label->width();
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
