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

#ifndef SUNSKYWINDOW_HH
#define SUNSKYWINDOW_HH

#include <QWidget>

#include "redshift/include/smart_ptr.hh"
#include "redshift/include/geometry.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class SunSkyWindow;
}

namespace picogen { namespace qt4 {

class SunSkyWindow : public QWidget
{
        Q_OBJECT

// ================== Public ===================================================
public:
        explicit SunSkyWindow(QWidget *parent = 0);
        virtual ~SunSkyWindow();

        void setSunSky (redshift::shared_ptr<cosyscene::SunSky>,
                        bool blockSignals=true);

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

signals:
        void skyChanged ();


// ================== Private ==================================================
private:
        void setSunSkyByValue (cosyscene::SunSky const &, bool blockSignals=true);
        void updateViews ();

        void updateFromUtahSunSkyEditor();
        void updateToUtahSunSkyEditor();


private slots:
        // Stash ...............................................................
        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

        // Utah sky editor .....................................................
        void on_utahSkyEditor_overcastChanged                 (double);
        void on_utahSkyEditor_atmosphericEffectsEnabledChanged(bool);
        void on_utahSkyEditor_atmosphericEffectsFactorChanged (double);
        void on_utahSkyEditor_previewResolutionChanged        (int);
        void on_utahSkyEditor_diskSizeChanged                 (double);
        void on_utahSkyEditor_turbidityChanged                (double);
        void on_utahSkyEditor_previewMultiplierChanged        (double);
        void on_utahSkyEditor_sunIntensityChanged             (double);
        void on_utahSkyEditor_atmosphereIntensityChanged      (double);
        void on_utahSkyEditor_sunDirectionChanged             (redshift::Vector);

private:
        Ui::SunSkyWindow *ui;
        redshift::shared_ptr<cosyscene::SunSky> sunSky;
};

} }

#endif // SUNSKYWINDOW_HH
