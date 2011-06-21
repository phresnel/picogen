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

#ifndef TERRAINFORMATION_HH
#define TERRAINFORMATION_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class TerrainFormation;
}

namespace picogen { namespace qt4 {

class TerrainFormation : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit TerrainFormation(QWidget *parent = 0);
        virtual ~TerrainFormation();

        void setFormation (
                redshift::shared_ptr<cosyscene::TerrainFormation> t,
                bool blockSignals
        );

signals:
        void formationChanged();


// ================== Private ==================================================
private:
        void showTerrainKindSelection();
        void showQuatschEditor       ();
        void showQuatschPresetEditor ();

        void setFormationByValue (cosyscene::TerrainFormation const &,
                                  bool blockSignals);
        void updateViews();

private slots:
        // Tabs.
        void on_heightmapCLB_clicked        ();
        void on_parametricPresetCLB_clicked ();
        void on_quatschCodeEditorCLB_clicked();

        // Quatsch editor.
        void on_quatschCodeEditor_codeChanged       ();
        void on_quatschPresetEditor_formationChanged();

        // Stashing.
        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

private:
        Ui::TerrainFormation *ui;
        redshift::shared_ptr<cosyscene::TerrainFormation> formation_;
};

} }

#endif // TERRAINFORMATION_HH
