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

#ifndef MAINWINDOW_HH_20100902
#define MAINWINDOW_HH_20100902

#include <QMainWindow>
#include "redshift/include/smart_ptr.hh"
#include "createredshiftsceneclosure.h"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class MainWindow;
}

namespace detail {
        class RedshiftSceneCreator;
}



class MainWindow : public QMainWindow
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

public slots:
        void startProductionRenderProcess();
        void startPreviewRenderProcess();


signals:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);



// ================== Private ==================================================
private:
        void switchOffCommandLinks ();
        void indicateSaved();
        void indicateUnsaved();

private slots:
        // Debug ...............................................................
        void on_actionShow_redshift_file_used_for_last_rendering_triggered();
        void on_actionShow_redshift_file_triggered();
        void on_action_Stylesheet_triggered();

        // Tab Selection .......................................................
        void on_waterCommandLink_clicked();
        void on_renderingSetupCommandLink_clicked();
        void on_renderCommandLink_clicked();
        void on_sunskyCommandLink_clicked();
        void on_forestCommandLink_clicked();
        void on_terrainCommandLink_clicked();
        void on_navigationCommandLink_clicked();
        void on_filmCommandLink_clicked();

        // Saving/Loading ......................................................
        void on_actionLoad_triggered();
        void on_actionSave_triggered();

        // Scene changes .......................................................
        void on_terrain_terrainChanged();
        void on_sunSky_skyChanged();
        void on_navigation_navigationChanged();
        void on_water_waterChanged();
        void on_filmSettingsAndCamera_filmSettingsChanged();
        void on_filmSettingsAndCamera_cameraChanged();
        void on_renderingSetup_renderSettingsChanged();


private:
        Ui::MainWindow *ui;
        redshift::shared_ptr<cosyscene::Scene> scene;

        redshift::shared_ptr<detail::RedshiftSceneCreator> redshiftSceneCreator;

        std::string lastRedshiftJob; // for debugging purposes
};

#endif // MAINWINDOW_HH_20100902
