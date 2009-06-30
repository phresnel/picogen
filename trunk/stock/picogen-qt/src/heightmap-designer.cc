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

#include <iostream>
#include <sstream>

#include <QMessageBox>

#include "../include/heightmap-designer.hh"

HeightmapDesignerImpl::HeightmapDesignerImpl(/*QWidget *parent*/) {
        setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);
}



HeightmapDesignerImpl::~HeightmapDesignerImpl() {

}



void HeightmapDesignerImpl::on_showJuxCode_clicked() {
        QList<QTreeWidgetItem *> items = tree->selectedItems();
        if (1 == items.size()) {                
                QMessageBox box (
                        QMessageBox::Information, 
                        "jux code",
                        generateJuxCode(items [0]).c_str()
                );
                box.exec();
        }        
}



void HeightmapDesignerImpl::on_addConstant_clicked ()  {
        addItem (CONSTANT);
}



void HeightmapDesignerImpl::on_addXParameter_clicked ()  {
        addItem (X_PARAMETER);
}



void HeightmapDesignerImpl::on_addYParameter_clicked () {
        addItem (Y_PARAMETER);
}        



void HeightmapDesignerImpl::on_addAddition_clicked () {
        addItem (ADDITION);
}



void HeightmapDesignerImpl::on_addSubtraction_clicked () {
        addItem (SUBTRACTION);
}



void HeightmapDesignerImpl::on_addMultiplication_clicked ()  {
        addItem (MULTIPLICATION);
}



void HeightmapDesignerImpl::on_addDivision_clicked () {
        addItem (DIVISION);
}



void HeightmapDesignerImpl::addItem (QTreeWidgetItem* item) {
        QList<QTreeWidgetItem *> items = tree->selectedItems();
        if (1 == items.size()) {
                items [0]->addChild (item);
                tree->setItemExpanded (items [0], true);
        } else if (0 == items.size()) {
                tree->addTopLevelItem (item);
        }
}



void HeightmapDesignerImpl::addItem (item_type_t type) {
        
        
        QTreeWidgetItem * item = 0;
        QTreeWidgetItem * parent = 0;
        
        
        bool parentIsAggregate = false;
        bool canInsert = false;
        QList<QTreeWidgetItem *> items = tree->selectedItems();
        if (1 == items.size()) {
                parent = items [0];
                parentIsAggregate = parent->type() == ADDITION
                                 || parent->type() == SUBTRACTION
                                 || parent->type() == MULTIPLICATION
                                 || parent->type() == DIVISION
                                  ;
        }
        canInsert = parentIsAggregate || 0 == parent;
        

        switch (type) {
        case CONSTANT:
                if (canInsert) {
                        item = new QTreeWidgetItem (type);
                        item->setText (0, "<const>");
                        item->setFlags ( Qt::ItemIsSelectable 
                                       | Qt::ItemIsEditable
                                       | Qt::ItemIsEnabled );
                        item->setData (0,
                                Qt::DisplayRole | Qt::EditRole, 
                                QVariant(0.0));
                }
                break;                        
        case X_PARAMETER:
                item = new QTreeWidgetItem (type);
                item->setText (0, "[x]");
                break;
        case Y_PARAMETER:
                item = new QTreeWidgetItem (type);
                item->setText (0, "[y]");
                break;
        
        case ADDITION:
                item = new QTreeWidgetItem (type);
                item->setText (0, "+");
                break;
        case SUBTRACTION:
                item = new QTreeWidgetItem (type);
                item->setText (0, "-");
                break;
        case MULTIPLICATION:
                item = new QTreeWidgetItem (type);
                item->setText (0, "*");
                break;
        case DIVISION:
                item = new QTreeWidgetItem (type);
                item->setText (0, "/");
                break;
                
        default:;
        };
        
        
        if (0 != item) {
                if (0 != parent) {
                        parent->addChild (item);
                        tree->setItemExpanded (parent, true);
                } else {
                        tree->addTopLevelItem (item);
                }
        }
}



std::string
HeightmapDesignerImpl::generateJuxCode (
        QTreeWidgetItem const *root,
        std::string indent
) const {
        std::string ret;
        
        bool isAggregate = false;
        
        switch (root->type()) {
        case CONSTANT: {
                std::stringstream ss;
                ss << root->data(0, Qt::EditRole).toDouble();
                ret += indent + ss.str().c_str();
                break;
        } break;
        case X_PARAMETER:
                ret += indent + "x";
                break;
        case Y_PARAMETER:
                ret += indent + "y";
                break;
        
        case ADDITION:
                ret += indent + "(+";
                isAggregate = true;
                break;
        case SUBTRACTION:
                ret += indent + "(-";
                isAggregate = true;
                break;
        case MULTIPLICATION:
                ret += indent + "(*";
                isAggregate = true;
                break;
        case DIVISION:                
                ret += indent + "(/";
                isAggregate = true;
                break;
                
        default:
                ret += "<unsupported or invalid type>";

        };
        
        if (isAggregate) {                
                ret += "\n";
                for (int i=0; i<root->childCount(); ++i) {
                        ret += generateJuxCode (root->child (i), 
                                                       indent + "\t") + "\n";
                }
                ret += indent + ")";
        }
        
        
        return ret;
}
