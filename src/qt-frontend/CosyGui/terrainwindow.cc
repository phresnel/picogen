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


#include "terrainwindow.hh"
#include "ui_terrainwindow.h"

#include "cosyscene/terrain.hh"



TerrainWindow::TerrainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerrainWindow),
    terrain(new cosyscene::Terrain)
{
        ui->setupUi(this);
}



TerrainWindow::~TerrainWindow() {
        delete ui;
}



void TerrainWindow::setTerrain (redshift::shared_ptr<cosyscene::Terrain> t) {
        terrain = t;
}



void TerrainWindow::on_quatschCodeEditorCLB_clicked() {
        ui->stackedWidget->setCurrentWidget(ui->quatschCodeEditorPage);
        terrain->toQuatschSource (cosyscene::QuatschSource(
                ui->quatschCodeEditor->code().toStdString()
        ));
}



void TerrainWindow::on_quatschCodeEditor_codeChanged() {
        terrain->toQuatschSource (cosyscene::QuatschSource(
                ui->quatschCodeEditor->code().toStdString()
        ));
        emit terrainChanged();
}



void TerrainWindow::stash_doStash() {
        terrain->stash();
}



void TerrainWindow::stash_doRestore() {
}
