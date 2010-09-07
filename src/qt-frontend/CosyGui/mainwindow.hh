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

namespace cosyscene {
        class Scene;
}

namespace Ui {
        class MainWindow;
}
class FilmSettings;

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

private:
        Ui::MainWindow *ui;
        redshift::shared_ptr<cosyscene::Scene> scene;

        void switchOffCommandLinks ();

        void indicateSaved();
        void indicateUnsaved();

signals:
        void stash_doStash();
        void stash_doRestore();
        void reset();

        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

private slots:
        void on_stashResetButton_clicked();
        void on_actionLoad_triggered();
        void on_actionSave_triggered();
        void on_restoreButton_clicked();
        void on_stashButton_clicked();
        void on_terrain_terrainChanged();
        void on_forestCommandLink_clicked();
        void on_terrainCommandLink_clicked();
        void on_navigationCommandLink_clicked();
        void on_filmCommandLink_clicked();
};

#endif // MAINWINDOW_HH_20100902
