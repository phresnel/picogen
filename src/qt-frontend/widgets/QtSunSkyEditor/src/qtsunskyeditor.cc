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

QtSunSkyEditor::QtSunSkyEditor(QWidget *parent)
: QWidget(parent)
, ui(new Ui::qtsunskyeditor)
{
        ui->setupUi(this);
        ui->previewScreen->setScaledContents(true);

        ui->previewScreen->installEventFilter(this);

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



bool QtSunSkyEditor::eventFilter(QObject *object, QEvent *event) {
        if (object == ui->previewScreen
            && (event->type() == QEvent::MouseMove
                || event->type() == QEvent::MouseButtonPress)
        ) {
                const QMouseEvent *e = (QMouseEvent*)event;
                const QPoint P = e->pos();
                if (e->buttons() & Qt::LeftButton) {
                        const float
                                u = P.x() / (float)ui->previewScreen->width(),
                                v = P.y() / (float)ui->previewScreen->height()
                        ;
                        setSunDirection(screenToHemisphereSat(u,v));
                }

                return true;
        }
        return false;
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



void QtSunSkyEditor::on_previewResolution_valueChanged(int res) {
        redraw(false);
        previewResolutionChanged(res);
}



void QtSunSkyEditor::on_turbiditySpinBox_valueChanged(double value) {
        redraw(true);
        emit turbidityChanged(value);
}



void QtSunSkyEditor::on_previewMultiplier_valueChanged(double value) {
        redraw(false);
        emit previewMultiplierChanged(value);
}



void QtSunSkyEditor::on_sunIntensitySpinBox_valueChanged(double value) {
        redraw(true);
        emit sunIntensityChanged(value);
}



void QtSunSkyEditor::on_atmosphereIntensitySpinBox_valueChanged(double value) {
        redraw(true);
        emit atmosphereIntensityChanged(value);
}



void QtSunSkyEditor::on_diskSizeSpinBox_valueChanged(double value) {
        redraw(true);
        emit diskSizeChanged(value);
}



void QtSunSkyEditor::on_enableAtmosphericEffects_stateChanged(int state) {
        redraw(true);
        emit atmosphericEffectsEnabledChanged(state != 0);
}



void QtSunSkyEditor::on_atmosphericEffectsFactorSpinBox_valueChanged(
        double value
) {
        redraw(true);
        emit atmosphericEffectsFactorChanged(value);
}



void QtSunSkyEditor::on_overcastSpinBox_valueChanged(double value) {
        redraw(true);
        emit overcastChanged(value);
}



void QtSunSkyEditor::on_directionXSpinBox_editingFinished() {
        updateSunDirectionFromSpinBoxes();
}
void QtSunSkyEditor::on_directionYSpinBox_editingFinished() {
        updateSunDirectionFromSpinBoxes();
}
void QtSunSkyEditor::on_directionZSpinBox_editingFinished() {
        updateSunDirectionFromSpinBoxes();
}
void QtSunSkyEditor::updateSunDirectionFromSpinBoxes() {
        using redshift::Vector;
        this->direction = normalize(Vector(ui->directionXSpinBox->value(),
                                           ui->directionYSpinBox->value(),
                                           ui->directionZSpinBox->value()));
        emit sunDirectionChanged(this->direction);
        updatePreethamSettingsAndRedraw();
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



double QtSunSkyEditor::overcast() const {
        return ui->overcastSpinBox->value();
}
bool QtSunSkyEditor::atmosphericEffectsEnabled() const {
        return ui->enableAtmosphericEffects->checkState() == Qt::Checked;
}
double QtSunSkyEditor::atmosphericEffectsFactor() const {
        return ui->atmosphericEffectsFactorSpinBox->value();
}
int QtSunSkyEditor::previewResolution() const {
        return ui->previewResolution->value();
}
double QtSunSkyEditor::diskSize() const {
        return ui->diskSizeSpinBox->value();
}
double QtSunSkyEditor::turbidity() const {
        return ui->turbiditySpinBox->value();
}
double QtSunSkyEditor::previewMultiplier() const {
        return ui->previewMultiplier->value();
}
double QtSunSkyEditor::sunIntensity() const {
        return ui->sunIntensitySpinBox->value();
}
double QtSunSkyEditor::atmosphereIntensity() const {
        return ui->atmosphereIntensitySpinBox->value();
}
redshift::Vector QtSunSkyEditor::sunDirection() const {
        return direction;
}


void QtSunSkyEditor::setOvercast(double value) {
        ui->overcastSpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setAtmosphericEffectsEnabled(bool value) {
        ui->enableAtmosphericEffects->setChecked(value?Qt::Checked:Qt::Unchecked);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setAtmosphericEffectsFactor(double value) {
        ui->atmosphericEffectsFactorSpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setPreviewResolution(int value) {
        ui->previewResolution->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setDiskSize(double value) {
        ui->diskSizeSpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setTurbidity(double value) {
        ui->turbiditySpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setPreviewMultiplier(double value) {
        ui->previewMultiplier->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setSunIntensity(double value) {
        ui->sunIntensitySpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setAtmosphereIntensity(double value) {
        ui->atmosphereIntensitySpinBox->setValue(value);
        updatePreethamSettingsAndRedraw();
}
void QtSunSkyEditor::setSunDirection (redshift::Vector value) {
        direction = value;
        const bool blocked = blockSignals(true);
        ui->directionXSpinBox->setValue(value.x);
        ui->directionYSpinBox->setValue(value.y);
        ui->directionZSpinBox->setValue(value.z);
        updatePreethamSettingsAndRedraw();
        blockSignals(blocked);
        emit sunDirectionChanged(this->direction);
}



void QtSunSkyEditor::updatePreethamSettingsAndRedraw() {
        updatePreethamSettings();
        redraw(true);
}
