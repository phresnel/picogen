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

#ifndef RENDERSETTINGS_HH
#define RENDERSETTINGS_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class RenderSettingsWindow;
}

namespace picogen { namespace qt4_gui {

class RenderSettingsWindow : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit RenderSettingsWindow(QWidget *parent = 0);
        virtual ~RenderSettingsWindow();

        void setIsPreviewSettings(bool isPreviewSettings);

        void setTitle (QString string);
        void setRenderSettings (redshift::shared_ptr<cosyscene::RenderSettings>,
                                bool blockSignals=true);
signals:
        void renderSettingsChanged();


// ================== Private ==================================================
private:
        void setRenderSettingsByValue (cosyscene::RenderSettings const &,
                                       bool blockSignals=true);
        void updateViews();

private slots:
        void on_autoResolutionButton_clicked();
        void on_ambientSamples_valueChanged(int );
        void on_surfaceIntegrator_currentIndexChanged(int index);
        void on_enableMaxQuadtreeDepthOverride_toggled(bool checked);
        void on_maxLazyQuadtreeDepthSpin_valueChanged(int);
        void on_widthSpin_valueChanged(int );
        void on_heightSpin_valueChanged(int );
        void on_seedSpin_valueChanged(int);
        void on_samplesPerPixelSpin_valueChanged(int);

        void setAutoResolutionFromAction();

        // Stashing.
        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

private:
        Ui::RenderSettingsWindow *ui;
        redshift::shared_ptr<cosyscene::RenderSettings> renderSettings_;
        bool isPreviewSettings;

        void adjustDefaultValues(cosyscene::RenderSettings &rs);
};

} }

#endif // RENDERSETTINGS_HH
