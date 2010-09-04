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


#ifndef TERRAINWINDOW_HH_20100902
#define TERRAINWINDOW_HH_20100902

#include <QWidget>
#include "redshift/include/smart_ptr.hh"

namespace cosyscene {
        class Terrain;
}

namespace Ui {
        class TerrainWindow;
}

class TerrainWindow : public QWidget
{
        Q_OBJECT

public:
        explicit TerrainWindow(QWidget *parent = 0);
        ~TerrainWindow();

        void setTerrain (redshift::shared_ptr<cosyscene::Terrain>);

signals:
        void terrainChanged();

public slots:
        void stash_doStash();
        void stash_doRestore();

private:
        Ui::TerrainWindow *ui;
        redshift::shared_ptr<cosyscene::Terrain> terrain;

private slots:
        void on_quatschCodeEditorCLB_clicked();
        void on_quatschCodeEditor_codeChanged();
};

#endif // TERRAINWINDOW_HH