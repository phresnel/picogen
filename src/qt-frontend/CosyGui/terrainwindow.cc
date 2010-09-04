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

#include "cosyscene/scene.hh"
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



void TerrainWindow::showQuatschEditor() {
        ui->stackedWidget->setCurrentWidget(ui->quatschCodeEditorPage);
}



void TerrainWindow::showTerrainKindSelection() {
        ui->stackedWidget->setCurrentWidget(ui->selectKind);
}



void TerrainWindow::setTerrain (redshift::shared_ptr<cosyscene::Terrain> t,
                                bool stopSignals
) {
        bool prevBlocked;
        if (stopSignals)
                prevBlocked = this->blockSignals(true);

        terrain = t;
        switch (t->kind()) {
        case cosyscene::Terrain::QuatschSource:
                ui->quatschCodeEditor->setCode(QString::fromStdString(
                                t->quatschSource().code()));
                showQuatschEditor();
                break;
        default:
                showTerrainKindSelection();
                break;
        }

        if (stopSignals)
                this->blockSignals(prevBlocked);
}



void TerrainWindow::on_quatschCodeEditorCLB_clicked() {
        showQuatschEditor();
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



void TerrainWindow::sceneInvalidated(
        redshift::shared_ptr<cosyscene::Scene> scene
) {
        setTerrain (scene->terrain());
}
