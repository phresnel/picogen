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


#include "include/qtsunskyeditor.hh"
#include <QPainter>


QtSunSkyEditor::QtSunSkyEditor(QWidget *parent)
: QWidget(parent)
, ui(new Ui::qtsunskyeditor)
{
        using namespace redshift;
        ui->setupUi(this);

        ui->turbiditySpinBox->setValue(2.2f);
        preetham.setColorFilter(Color(0.2f, 0.2f, 0.2f));
        preetham.setSunDirection(Vector(0,1,0));
        preetham.invalidate();
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



void QtSunSkyEditor::mouseMoveEvent (QMouseEvent* e) {
        using namespace redshift;

        if (ui->previewScreen->rect().contains(e->pos())) {
                QPointF P = e->posF()-ui->previewScreen->pos();

                if (e->buttons() & Qt::LeftButton) {
                        const float
                                u = P.x() / (float)ui->previewScreen->width(),
                                v = P.y() / (float)ui->previewScreen->height()
                        ;
                        const Vector d = screenToHemisphereSat(u,v);
                        preetham.setSunDirection(d);
                        preetham.invalidate();

                        redraw(true, u, v);
                }
        }
}



void QtSunSkyEditor::mousePressEvent (QMouseEvent* e) {
        mouseMoveEvent(e);
}



void QtSunSkyEditor::redraw (bool drawCross, float crossU, float crossV) {
        const int
                width = ui->previewScreen->width(),
                height = ui->previewScreen->height();
        QImage img (width, height, QImage::Format_RGB32);
        QPainter painter (&img);

        const int pixelSize = ui->previewResolution->value();
        for (int y=0; y<height; y+=pixelSize)
        for (int x=0; x<width; x+=pixelSize) {
                const rgbf col = computeColor (x/(float)width, y/(float)height);
                const int
                        r_ = (int)256.f*col.r,
                        g_ = (int)256.f*col.g,
                        b_ = (int)256.f*col.b,
                        r = r_<0?0:r_>255?255:r_,
                        g = g_<0?0:g_>255?255:g_,
                        b = b_<0?0:b_>255?255:b_
                ;
                //img.setPixel(x, y, QColor(r,g,b).rgb());
                QRgb const color = QColor(r,g,b).rgb();
                //painter.setPen(color);
                painter.fillRect(x,y,pixelSize,pixelSize, color);
        }

        if (false&&drawCross) {
                const int X = static_cast<int>(crossU * width);
                const int Y = static_cast<int>(crossV * height);
                for (int x=0; x<width; x+=2)
                        img.setPixel(x, Y, QColor(255,0,0).rgb());
                for (int y=0; y<height; y+=2)
                        img.setPixel(X, y, QColor(255,0,0).rgb());
        }
        ui->previewScreen->setPixmap(QPixmap::fromImage(img));
}



QtSunSkyEditor::rgbf
QtSunSkyEditor::computeColor (float u, float v) const {
        using namespace redshift;
        using namespace redshift::background;


        const optional<Vector> d = screenToHemisphere(u,v);
        if (!d)
                return rgbf(0,0,0);

        const Ray skyRay (Point(), *d);
        const Color color = preetham.shade (skyRay);
        return rgbf(color.r, color.g, color.b);
}



redshift::optional<redshift::Vector>
QtSunSkyEditor::screenToHemisphere (float u, float v) const {
        using namespace redshift;

        const float x = (u*2-1)*1.1;
        const float z = (v*2-1)*1.1;

        if (x*x+z*z>1.0f)
                return optional<Vector>();
        const float y = sqrt (1-x*x-z*z);
        return normalize (redshift::Vector(x,y,z));
}

redshift::Vector
QtSunSkyEditor::screenToHemisphereSat (float u, float v) const {
        using namespace redshift;

        const float x = (u*2-1)*1.1;
        const float z = (v*2-1)*1.1;

        if (x*x+z*z>1.0f) {
                return normalize (redshift::Vector(x,0,z));
        }
        const float y = sqrt (1-x*x-z*z);
        return normalize (redshift::Vector(x,y,z));
}



void QtSunSkyEditor::on_turbidityDial_sliderMoved(int position) {
        const float fac = 100.f;
        ui->turbiditySpinBox->setValue(position / fac);

        preetham.setTurbidity(ui->turbiditySpinBox->value());
        preetham.invalidate();
        redraw();
}



void QtSunSkyEditor::on_turbiditySpinBox_valueChanged(double value) {
        const float fac = 100.f;
        ui->turbidityDial->setValue(static_cast<int>(value*fac));

        preetham.setTurbidity(value);
        preetham.invalidate();
        redraw();
}
