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

#include "stashview.hh"
#include "terrainformation.hh"
#include "ui_terrainformation.h"

#include "cosyscene/terrain.hh"


TerrainFormation::TerrainFormation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerrainFormation),
    formation_(new cosyscene::TerrainFormation())
{
        ui->setupUi(this);
        ui->importCLB->setVisible(false);
}



TerrainFormation::~TerrainFormation()
{
    delete ui;
}



void TerrainFormation::setFormation (
        redshift::shared_ptr<cosyscene::TerrainFormation> t,
        bool blockSignals
) {

        const bool prevBlocked = this->blockSignals(blockSignals);
        formation_ = t;
        updateViews();
        this->blockSignals(prevBlocked);
}



void TerrainFormation::setFormationByValue(cosyscene::TerrainFormation const &t,
                                           bool blockSignals
) {
        const bool prevBlocked = this->blockSignals(blockSignals);
        *formation_ = t;
        updateViews();
        this->blockSignals(prevBlocked);
}



void TerrainFormation::showQuatschEditor() {
        ui->stackedWidget->setCurrentWidget(ui->quatschCodeEditorPage);
}



void TerrainFormation::showQuatschPresetEditor() {
        ui->stackedWidget->setCurrentWidget(ui->quatschPresetEditorPage);
}



void TerrainFormation::showTerrainKindSelection() {
        ui->stackedWidget->setCurrentWidget(ui->selectKind);
}



void TerrainFormation::on_quatschCodeEditorCLB_clicked() {
        formation_->toQuatschSource(cosyscene::QuatschSource(
                "/* Press F1 for help :) */\n"
                "/* ... or move the caret over some entity and try out\n"
                "   the contextual help via F1 */\n"
                "([LibnoiseVoronoi \n"
                "   enable-distance{1}\n"
                "   displacement{0.1}\n"
                " ] x y)\n"
        ));
        updateViews();
}



void TerrainFormation::on_parametricPresetCLB_clicked() {
        formation_->toQuatschPreset(cosyscene::QuatschPreset(
        "(($Width :real = {[1..99999]} default=1000))\n"
        "(($Depth :real = {[1..99999]} default=1000))\n"
        "(($Height:real = {[1..99999]} default=1000))\n"
        "(($Filename:filename))\n"
        "(($Filter:enumeration={nearest, bilinear, cosine, cubic} default=3))\n"
        "\n"
        "([Heightmap filter{(($Filter))}\n"
        "            width {(($Width))}\n"
        "            depth {(($Depth))}\n"
        "            height{(($Height))}\n"
        "            filename{(($Filename))}\n"
        " ] x y)"
        ));
        updateViews();
}

void TerrainFormation::on_heightmapCLB_clicked() {
        formation_->toQuatschPreset(cosyscene::QuatschPreset(
        "(($Width :real = {[1..9999999]} default=1000))\n"
        "(($Depth :real = {[1..9999999]} default=1000))\n"
        "(($Height:real = {[1..9999999]} default=1000))\n"
        "(($Filename:filename))\n"
        "(($Filter:enumeration={nearest, bilinear, cosine, cubic} default=3))\n"
        "\n"
        "([Heightmap filter{(($Filter))}\n"
        "            width {(($Width))}\n"
        "            depth {(($Depth))}\n"
        "            height{(($Height))}\n"
        "            filename{(($Filename))}\n"
        " ] x y)"
        ));
        updateViews();
}

void TerrainFormation::on_quatschPresetEditor_formationChanged() {
        formation_->toQuatschPreset(ui->quatschPresetEditor->toCosy());
        emit formationChanged();
}



void TerrainFormation::on_quatschCodeEditor_codeChanged() {
        formation_->toQuatschSource(cosyscene::QuatschSource(
                ui->quatschCodeEditor->code().toStdString()
        ));
        emit formationChanged();
}



void TerrainFormation::updateViews() {
        switch (formation_->kind()) {
        case cosyscene::TerrainFormation::QuatschSource:
                ui->quatschCodeEditor->setCode(
                                QString::fromStdString(
                                formation_->quatschSource().code()));
                showQuatschEditor();
                break;
        case cosyscene::TerrainFormation::QuatschPreset: {
                const bool blocked = ui->quatschPresetEditor->blockSignals(true);
                cosyscene::QuatschPreset qp = formation_->quatschPreset();
                ui->quatschPresetEditor->fromCosy(qp);
                showQuatschPresetEditor();
                ui->quatschPresetEditor->blockSignals(blocked);
        } break;
        case cosyscene::TerrainFormation::None:
                showTerrainKindSelection();
                break;
        }
}



void TerrainFormation::on_stashButton_clicked() {
        formation_->stash();
}
void TerrainFormation::on_stashRestoreButton_clicked() {
        StashView *sw = new StashView (this);
        sw->addItems(formation_->getStash());
        if (QDialog::Accepted == sw->exec()) {
                redshift::shared_ptr<cosyscene::TerrainFormation> newFormation (
                  new cosyscene::TerrainFormation(
                    sw->selectedData<cosyscene::TerrainFormation>())
                );
                newFormation->setStash(
                  sw->itemsToStash<cosyscene::TerrainFormation>());
                setFormationByValue(*newFormation, true);
        }
}
void TerrainFormation::on_stashResetButton_clicked() {
        if (!formation_->getStash().contains_data(*formation_)) {
                switch (confirmReset (this)) {
                case ConfirmReset_Abort: return;
                case ConfirmReset_StashBeforeReset: formation_->stash(); break;
                case ConfirmReset_Reset: break;
                }
        }
        cosyscene::TerrainFormation t;
        t.setStash(formation_->getStash());
        setFormationByValue(t, true);
}



