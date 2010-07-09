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
#include "backgrounds/pss-adapter.hh"
#include "backgrounds/preetham-shirley-smits/sunsky.hh"
#include <QPainter>

//#include <omp.h>


QtSunSkyEditor::QtSunSkyEditor(QWidget *parent)
: QWidget(parent)
, ui(new Ui::qtsunskyeditor)
{
        ui->setupUi(this);
        ui->previewScreen->setScaledContents(true);

        using namespace redshift;
        direction = normalize(Vector(1,1,1));
        ui->turbiditySpinBox->setValue(7.2f);
        ui->sunIntensitySpinBox->setValue(1.0f);
        ui->atmosphereIntensitySpinBox->setValue(1.0f);
        redraw(true);
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
        redraw(false);
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
                        this->direction = screenToHemisphereSat(u,v);
                        updatePreethamSettings();
                        //preetham.setSunDirection(d);
                        //preetham.invalidate();
                        redraw(true, false, u, v);
                }
        }
}



void QtSunSkyEditor::mousePressEvent (QMouseEvent* e) {
        mouseMoveEvent(e);
}



void QtSunSkyEditor::wheelEvent(QWheelEvent* e) {
        if (ui->previewScreen->rect().contains(e->pos())) {
                const float speed = 10.f;/* /
                        (e->modifiers()&Qt::ControlModifier)?10.f:1.f;*/
                ui->turbiditySpinBox->setValue(
                        ui->turbiditySpinBox->value()
                        + e->delta() / (speed*(8.f*15.f))
                );
                redraw(true);
        }
}



void QtSunSkyEditor::redraw (bool recalc, bool drawCross, float crossU, float crossV) {
        if (recalc) updatePreethamSettings();

        const int
                pixelSize = ui->previewResolution->value(),
                width = ui->previewScreen->width()/pixelSize,
                height = ui->previewScreen->height()/pixelSize;
        QImage img (width, height, QImage::Format_RGB32);
        QPainter painter (&img);


        for (int y=0; y<height; ++y)
        for (int x=0; x<width; ++x) {
                const rgbf col = computeColor (x/(float)width, y/(float)height);
                const int
                        r_ = (int)(256.f*col.r),
                        g_ = (int)(256.f*col.g),
                        b_ = (int)(256.f*col.b),
                        r = r_<0?0:r_>255?255:r_,
                        g = g_<0?0:g_>255?255:g_,
                        b = b_<0?0:b_>255?255:b_
                ;
                img.setPixel(x, y, QColor(r,g,b).rgb());
                //QRgb const color = QColor(r,g,b).rgb();
                //painter.fillRect(x,y,pixelSize,pixelSize, color);
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

        const Vector d = screenToHemisphereSat(u,v);
        if (0 == preetham.get() || 0 == preetham->sun())
                return rgbf(0.5,0.25,0.125);

        const Ray skyRay (Point(), d);
        Color color = preetham->sun()->color(skyRay)
                    + preetham->atmosphere()->color(skyRay);
        // not working yet
        if (ui->enableAtmosphericEffects->checkState())
                color = preetham->atmosphericEffects()->attenuate(
                        color, skyRay, redshift::constants::infinity
                );
        const Color scaledDown = color * ui->previewMultiplier->value();
        const color::RGB rgb = scaledDown.toRGB();
        return rgbf(rgb.R, rgb.G, rgb.B);
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



void QtSunSkyEditor::on_previewResolution_valueChanged(int) {
        redraw(false);
}



void QtSunSkyEditor::on_turbiditySpinBox_valueChanged(double value) {
        redraw(true);
}



void QtSunSkyEditor::on_previewMultiplier_valueChanged(double ) {
        redraw(false);
}



void QtSunSkyEditor::on_sunIntensitySpinBox_valueChanged(double value) {
        redraw(true);
}



void QtSunSkyEditor::on_atmosphereIntensitySpinBox_valueChanged(double value) {
        redraw(true);
}



void QtSunSkyEditor::on_diskSizeSpinBox_valueChanged(double) {
        redraw(true);
}



void QtSunSkyEditor::on_enableAtmosphericEffects_stateChanged(int ) {
        redraw(true);
}



void QtSunSkyEditor::on_atmosphericEffectsFactorSpinBox_valueChanged(double ) {
        redraw(true);
}



void QtSunSkyEditor::on_overcastSpinBox_valueChanged(double ) {
        redraw(true);
}



void QtSunSkyEditor::updatePreethamSettings() {
        using namespace redshift;
        shared_ptr<background::PssSunSky> pss (new background::PssSunSky(
                normalize(direction),
                ui->turbiditySpinBox->value(),
                ui->overcastSpinBox->value(),
                ui->enableAtmosphericEffects->checkState()
        ));
        preetham = redshift::shared_ptr<redshift::backgrounds::PssAdapter>
                   (new backgrounds::PssAdapter (
                        pss,
                        ui->diskSizeSpinBox->value(),
                        ui->sunIntensitySpinBox->value(),
                        ui->atmosphereIntensitySpinBox->value(),
                        ui->atmosphericEffectsFactorSpinBox->value()));
}
