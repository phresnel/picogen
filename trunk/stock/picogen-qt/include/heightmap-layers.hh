//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef HEIGHTMAP_LAYERS_HH_INCLUDED_20090408
#define HEIGHTMAP_LAYERS_HH_INCLUDED_20090408

#include <iostream>
#include <map>

#include <QCheckBox>
#include <QMdiArea>

#include "../../redshift/include/setup.hh"

#include "ui_heightmap-layers.h"
#include "heightmap-display.hh"

#include "heightmap/codegen.hh"

#include "scene.hh"



class HeightmapLayersImpl
: public QWidget, private Ui::HeightmapLayers, public scene::SyncToView
{
        Q_OBJECT

public:
        HeightmapLayersImpl(QMdiArea *mdiArea, redshift::shared_ptr<scene::heightmap::Composition> composition);
        virtual ~HeightmapLayersImpl();

        void syncFromView ();
        void syncToView ();

        redshift::shared_ptr<const scene::heightmap::Composition> getComposition() const;


private slots:

        void on_newLayer_clicked();
        void on_newGroup_clicked();
        void on_deleteLayer_clicked();

        void on_moveUp_clicked();
        void on_moveDown_clicked();
        void on_openDefinition_clicked();

        void on_showJuxCode_clicked();
        void on_updatePreview_clicked ();


        void storeRowParameters(int rowId, scene::heightmap::RowParametersMerger const &merge);
        void closingDefinitionWindow(int rowId);

        void slot_syncFromView ();


signals:
        //void setRowParameters(int rowId, RowParameters const &params);

private:

        QMdiArea *mdiArea;

        struct Indices {
                const static int Visible = 1;
                const static int Locked = 2;
                const static int Type = 3;
                const static int Preview = 4;
                const static int Name = 5;
                const static int Id = 6;
                const static int RightMostData = Id;

                const static int LeftBorder = 0;
                const static int RightBorder = 7;
        };
public:

protected:
        virtual void changeEvent(QEvent *e);

private:

        SceneDisplayImpl *display;

        std::map<int, QWidget*> openDefinitionWindows;

        redshift::shared_ptr<scene::heightmap::Composition> composition;
        //shared_ptr<scene::Scene*> scene;

        scene::heightmap::RowData rowFromView (int row) const ;
        void rowToView (int row, scene::heightmap::RowData data) ;
        void swapRows(int rowA, int rowB);
        void moveRowUp(int row);
        void moveRowDown(int row);

        void colorizeGroups ();


        QString generateJuxCode(int &juxFunctionId, QString &callCode,
                           redshift::shared_ptr<const scene::heightmap::Composition> composition,
                           int recDepth=0) const;
        redshift::tuple<QString, QString> generateJuxCode (
                 redshift::shared_ptr<const scene::heightmap::Composition> composition,
                 redshift::shared_ptr<heightmap_codegen::NamespaceMaker> nm,
                 int indent,
                 size_t &rowIndex,
                 bool startsInGroup = false
        ) const;
        QString getJuxIndendationString (int indent) const ;

        void makeVisibilityCheckBox (QCheckBox *box, bool checked) const ;
        void makeLockingCheckBox (QCheckBox *box, bool checked) const ;

        void fillCompositionTypeComboBox (QComboBox *box) const ;
};


#endif // HEIGHTMAP_LAYERS_HH_INCLUDED_20090408
