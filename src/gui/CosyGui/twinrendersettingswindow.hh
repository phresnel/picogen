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

#ifndef TWINRENDERSETTINGS_HH
#define TWINRENDERSETTINGS_HH

#include <QWidget>
#include "rendersettingswindow.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
    class TwinRenderSettingsWindow;
}



class TwinRenderSettingsWindow : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit TwinRenderSettingsWindow(QWidget *parent = 0);
        virtual ~TwinRenderSettingsWindow();

        void setTwinRenderSettings (redshift::shared_ptr<cosyscene::TwinRenderSettings>,
                                    bool blockSignals=true);

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

signals:
        void productionRenderProcessRequested();
        void previewRenderProcessRequested();
        void renderSettingsChanged();


// ================== Private ==================================================
private slots:
        void on_startPreviewRenderProcess_clicked();
        void on_startProductionRenderProcess_clicked();
        void on_previewRenderSettingsWindow_renderSettingsChanged();
        void on_renderSettingsWindow_renderSettingsChanged();

private:
        Ui::TwinRenderSettingsWindow *ui;
        redshift::shared_ptr<cosyscene::TwinRenderSettings> twinRenderSettings_;

};

#endif // TWINRENDERSETTINGS_HH
