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

#include "kallisto/common.hh"
#include <QImage>
#include "include/qtsunskyeditor.hh"
#include "ui_qtsunskyeditor.h"
#include "redshift.hh"



QtSunSkyEditor::QtSunSkyEditor(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::qtsunskyeditor)
{
        ui->setupUi(this);

        redraw();
}



QtSunSkyEditor::~QtSunSkyEditor() {
        delete ui;
}



void QtSunSkyEditor::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void QtSunSkyEditor::resizeEvent (QResizeEvent*) {
        redraw();
}



void QtSunSkyEditor::redraw () {
        const int
                width = ui->previewScreen->width(),
                height = ui->previewScreen->height();
        QImage img (width, height, QImage::Format_RGB32);
        for (int y=0; y<height; ++y)
        for (int x=0; x<width; ++x) {
                const rgbf col = computeColor (x/(float)width, y/(float)height);
                const int
                        r_ = (int)256.f*col.r,
                        g_ = (int)256.f*col.g,
                        b_ = (int)256.f*col.b,
                        r = r_<0?0:r_>255?255:r_,
                        g = g_<0?0:g_>255?255:g_,
                        b = b_<0?0:b_>255?255:b_
                ;
                img.setPixel(x, y, QColor(r,g,b).rgb());
        }
        ui->previewScreen->setPixmap(QPixmap::fromImage(img));
}



QtSunSkyEditor::rgbf
QtSunSkyEditor::computeColor (float u, float v) const {
        using namespace redshift;
        using namespace redshift::background;
        Preetham preetham;
        preetham.setTurbidity(2.2f);
        preetham.setColorFilter(Color(0.2f, 0.2f, 0.2f));
        preetham.setSunDirection(Vector(1,1,0));
        preetham.invalidate();

        const float x = (u*2-1)*1.1;
        const float z = (v*2-1)*1.1;

        if (x*x+z*z>1.0f)
                return rgbf(0,0,0);
        const float y = sqrt (1-x*x-z*z);

        const Ray skyRay (
                Point(),
                normalize(Vector(x,y,z))
        );
        const Color color = preetham.shade (skyRay);
        return rgbf(color.r, color.g, color.b);
}
