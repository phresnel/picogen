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

#ifndef QTSUNSKYEDITOR_HH
#define QTSUNSKYEDITOR_HH

#include "geometry.hh"
#include "optional.hh"
#include "smart_ptr.hh"

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include "ui_qtsunskyeditor.h"


namespace Ui {
        class qtsunskyeditor;
}
namespace redshift {
        namespace backgrounds {
                class PssAdapter;
        }
}



class QtSunSkyEditor : public QWidget {
        Q_OBJECT
public:
        QtSunSkyEditor(QWidget *parent = 0);
        ~QtSunSkyEditor();

        double overcast() const;
        bool atmosphericEffectsEnabled() const;
        double atmosphericEffectsFactor() const;
        int previewResolution() const;
        double diskSize() const;
        double turbidity() const;
        double previewMultiplier() const;
        double sunIntensity() const;
        double atmosphereIntensity() const;
        redshift::Vector sunDirection() const;

        void setOvercast(double );
        void setAtmosphericEffectsEnabled(bool);
        void setAtmosphericEffectsFactor(double );
        void setPreviewResolution(int );
        void setDiskSize(double );
        void setTurbidity(double );
        void setPreviewMultiplier(double );
        void setSunIntensity(double );
        void setAtmosphereIntensity(double );
        void setSunDirection (redshift::Vector);

signals:
        void overcastChanged(double );
        void atmosphericEffectsEnabledChanged(bool);
        void atmosphericEffectsFactorChanged(double );
        void previewResolutionChanged(int );
        void diskSizeChanged(double );
        void turbidityChanged(double );
        void previewMultiplierChanged(double );
        void sunIntensityChanged(double );
        void atmosphereIntensityChanged(double );
        void sunDirectionChanged (redshift::Vector);

protected:
        void changeEvent(QEvent *e);
        void resizeEvent (QResizeEvent*);

        void wheelEvent(QWheelEvent*);
        bool eventFilter(QObject *object, QEvent *event);


private:
        void redraw(bool recalc, bool drawCross=false, float crossU=0, float crossV=0);
        void updatePreethamSettings();
        void updatePreethamSettingsAndRedraw();

        redshift::Vector direction;


        struct rgbf {
                rgbf(){}
                rgbf(float r, float g, float b) : r(r), g(g), b(b) {}
                float r,g,b;
        };
        rgbf computeColor (float u, float v) const;

        redshift::optional<redshift::Vector>
                screenToHemisphere (float u, float v) const;
        redshift::Vector screenToHemisphereSat (float u, float v) const;


        Ui::qtsunskyeditor *ui;
        redshift::shared_ptr<redshift::backgrounds::PssAdapter> preetham;

private slots:
        void on_overcastSpinBox_valueChanged(double );
        void on_enableAtmosphericEffects_stateChanged(int );
        void on_atmosphericEffectsFactorSpinBox_valueChanged(double );
        void on_previewResolution_valueChanged(int );
        void on_diskSizeSpinBox_valueChanged(double );
        void on_turbiditySpinBox_valueChanged(double );
        void on_previewMultiplier_valueChanged(double );
        void on_sunIntensitySpinBox_valueChanged(double );
        void on_atmosphereIntensitySpinBox_valueChanged(double );
};

#endif // QTSUNSKYEDITOR_HH
