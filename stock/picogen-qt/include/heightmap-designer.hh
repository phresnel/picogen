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

#ifndef HEIGHTMAP_DESIGNER_HH_INCLUDED_20090330
#define HEIGHTMAP_DESIGNER_HH_INCLUDED_20090330

//#include <QScrollArea>
//#include <QTextEdit>
#include <string>
#include "ui_heightmap-designer.h"

class HeightmapDesignerImpl : public QWidget, private Ui::HeightmapDesigner
{
        Q_OBJECT

public:
        HeightmapDesignerImpl(/*QWidget* parent=0*/);
        virtual ~HeightmapDesignerImpl();

private slots:

        void on_showJuxCode_clicked ();

        void on_addConstant_clicked ();
        void on_addXParameter_clicked ();
        void on_addYParameter_clicked ();

        void on_addAddition_clicked ();
        void on_addSubtraction_clicked ();
        void on_addMultiplication_clicked ();
        void on_addDivision_clicked ();
 
private:
        enum item_type_t {
                X_PARAMETER,
                Y_PARAMETER,
                CONSTANT,
                
                ADDITION,
                SUBTRACTION,
                MULTIPLICATION,
                DIVISION
        };
        void addItem (item_type_t type) ;
        void addItem (QTreeWidgetItem*) ;
        
        std::string generateJuxCode (QTreeWidgetItem const *root, 
                                                std::string indent="") const ;
};


#endif // HEIGHTMAP_DESIGNER_HH_INCLUDED_20090330
