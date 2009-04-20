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

#include <QCheckBox>
#include <QMdiArea>

#include "ui_heightmap-layers.h"

// when we derive from QAbstractScrollArea, the content is displayed
class HeightmapLayersImpl : public QWidget, private Ui::HeightmapLayers
{
        Q_OBJECT

public:
        HeightmapLayersImpl(QMdiArea *mdiArea);
        virtual ~HeightmapLayersImpl();

private slots:                

        void on_newLayer_clicked();
        void on_newGroup_clicked();
        void on_deleteLayer_clicked();
        
        void on_moveUp_clicked();
        void on_moveDown_clicked();        
        void on_openDefinition_clicked();
        
        void on_showJuxCode_clicked();
 
private:

        QMdiArea *mdiArea;
        
        struct Indices {
                const static int Visible = 1;
                const static int Locked = 2;
                const static int Type = 3;
                const static int Preview = 4;
                const static int Name = 5;
                const static int RightMostData = Name;
                
                const static int LeftBorder = 0;
                const static int RightBorder = 6;
        };
        
        enum CompositionType {
                Addition,
                Subtraction,
                Multiplication,
                Division,                
                Lerp
        };
        
        struct RowData {
                enum Type {
                        // primitives
                        FirstClass,
                        Bitmap,
                        Composition,
                        Code,
                        
                        // aggregates
                        Group,
                        GroupAddition = Group,
                        GroupSubtraction,
                        GroupMultiplication,
                        GroupDivision,
                        GroupLerp
                };                
                
                unsigned int id;
                bool visible;
                bool locked;
                Type type;
                QString name;
                CompositionType groupType;                
                
                RowData (unsigned int id_, bool visible_, bool locked_, 
                        Type type_, QString name_, 
                        CompositionType groupType_=Addition)
                : id (id_), visible(visible_), locked(locked_), type(type_) 
                , name(name_), groupType(groupType_)
                {}
                
                RowData (RowData const &r)
                : id (r.id), visible (r.visible), locked (r.locked) 
                , type (r.type), name (r.name), groupType (r.groupType)
                {
                }
                
                RowData &operator = (RowData const &r) {
                        id = r.id;
                        visible = r.visible;
                        locked = r.locked;
                        type = r.type;
                        name = r.name;
                        groupType = r.groupType;
                        return *this;
                }
                
                void swap (RowData &r) {
                        RowData tmp (r);
                        r = *this;
                        *this = tmp;
                }
                
                RowData () {}
        };
        
        
        struct Composition {
                CompositionType type;
                std::vector <RowData> data;
                unsigned int generateId () const {
                       again:
                        const unsigned int id = rand();
                        for (size_t i=0; i<data.size(); ++i) {
                                if (id == data [i].id) {
                                        goto again;
                                }
                        }
                        return id;
                }
        };
        
        Composition composition;
        void syncFromView ();
        void syncToView ();

                
        RowData rowFromView (int row) const ;
        void rowToView (int row, RowData data) ;
        void swapRows(int rowA, int rowB);
        void moveRowUp(int row);
        void moveRowDown(int row);
        
        void colorizeGroups ();


        QString generateJuxCode(int &juxFunctionId, QString &callCode,
                                Composition const &comp, int recDepth=0) const;
        QString generateJuxCode (Composition const &composition, 
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
