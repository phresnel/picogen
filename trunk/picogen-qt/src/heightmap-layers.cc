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

#include <cstdlib>

#include <iostream>

#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QResizeEvent>
#include <QMdiSubWindow>

#include "../include/heightmap-layers.hh"
#include "../include/quatsch-editor.hh"

HeightmapLayersImpl::HeightmapLayersImpl(QMdiArea *mdiArea_)
: mdiArea (mdiArea_)
{
        setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);
        
        
        QHeaderView *horizontalHeader = table->horizontalHeader();
        horizontalHeader->setResizeMode(QHeaderView::ResizeToContents);
        horizontalHeader->setResizeMode(Indices::RightMostData, QHeaderView::Stretch);
        //horizontalHeader->setStretchLastSection (true);
        horizontalHeader->hide();
        
        QHeaderView *verticalHeader = table->verticalHeader();        
        verticalHeader->setResizeMode(QHeaderView::Fixed);
        verticalHeader->setDefaultSectionSize (36);
        verticalHeader->hide();
        
        verticalHeader->setStyleSheet (
                 "QHeaderView::section { \n"
                 "    background-color: #666666 ; \n" 
                 "    color: white; \n"
                 "    padding-left: 4px; \n"
                 "    border: 1px solid #6c6c6c; \n"
                 "}"
        );
        
        fillCompositionTypeComboBox (compositionType);
        
        /*table->setStyleSheet (
        "QTableView {\n"
        "  selection-background-color: qlineargradient( \n"
        "                       x1: 0, y1: 0, x2: 0.5, y2: 0.5,\n"
        "                       stop: 0 #FF92BB, stop: 1 white);\n"
        "} \n"
        );*/
}



HeightmapLayersImpl::~HeightmapLayersImpl() {
}



void HeightmapLayersImpl::fillCompositionTypeComboBox (QComboBox *box) const {
        box->setInsertPolicy (QComboBox::InsertAtBottom);
        box->addItem ("Add", QVariant(Addition));
        box->addItem ("Subtraction", QVariant(Subtraction));
        box->addItem ("Multiplication", QVariant(Multiplication));
        box->addItem ("Division", QVariant(Division));
        box->addItem ("Lerp", QVariant(Lerp));
}



void HeightmapLayersImpl::swapRows(int rowA, int rowB)
{
        syncFromView ();
        if (rowA>=0 && (size_t)rowA < composition.data.size() 
            && rowB>=0 && (size_t)rowB < composition.data.size() 
        ) {
                /*const RowData a = getRowData (rowA);
                const RowData b = getRowData (rowB);
                setRowData (rowA, b);
                setRowData (rowB, a);*/
                
                RowData tmp = composition.data [rowA];
                composition.data [rowA] = composition.data [rowB];
                composition.data [rowB] = tmp;                
                
                table->selectRow (rowB);               
                
        }
        syncToView ();
}



void HeightmapLayersImpl::moveRowUp(int row) {
        swapRows (row, row-1);
}



void HeightmapLayersImpl::moveRowDown(int row) {
        swapRows (row, row+1);
}



void HeightmapLayersImpl::on_moveUp_clicked() {
        moveRowUp (table->currentRow());
        syncToView ();
}



void HeightmapLayersImpl::on_moveDown_clicked() {
        moveRowDown (table->currentRow());
        syncToView ();
}



HeightmapLayersImpl::RowData HeightmapLayersImpl::rowFromView (int row) const {
        if (0>row)
                return RowData();

        if (0) {
                std::cout << "Row " << row << '\n';        
                std::cout << "  visible: " 
                        << ((QCheckBox*)table->cellWidget (row, 
                                        Indices::Visible))->isChecked()
                        << '\n';
                std::cout << "  locked: " 
                        << ((QCheckBox*)table->cellWidget (row, 
                                        Indices::Locked))->isChecked()
                        << '\n';
                std::cout << "  name: " 
                        << table->item (row, 
                                        Indices::Name)->text().toStdString()
                        << '\n';
        }
        
        const QComboBox *typeComboBox =
                        ((QComboBox*)table->cellWidget (row, Indices::Type));

        return RowData(
                ((QCheckBox*)table->cellWidget (row, Indices::Visible))->
                                                                isChecked(),
                ((QCheckBox*)table->cellWidget (row, Indices::Locked))->
                                                                isChecked(),
                static_cast<RowData::Type>(typeComboBox->itemData (
                        typeComboBox->currentIndex()).toInt()),
                table->item (row, Indices::Name)->text()              
        );
}



void HeightmapLayersImpl::rowToView (int rowIndex, RowData data) {

     

        // Enable/Disable (i.e. visibility)
        {
                
                QCheckBox *box = new QCheckBox ();
                table->setCellWidget (rowIndex, Indices::Visible, box);                
                makeVisibilityCheckBox (box, data.visible);
        }
        
        // Lock status
        {
                QCheckBox *box = new QCheckBox ();        
                table->setCellWidget (rowIndex, Indices::Locked, box);
                makeLockingCheckBox (box, data.locked);                
        }
        
        // Type
        {
                QComboBox *box = new QComboBox ();
                table->setCellWidget (rowIndex, Indices::Type, box);               
                
                
                if (data.type < RowData::Group) {
                        box->setInsertPolicy (QComboBox::InsertAtBottom);
                        box->addItem ("1st Class", QVariant(RowData::FirstClass));
                        box->addItem ("Composition", QVariant(RowData::Composition));
                        box->addItem ("Bitmap", QVariant(RowData::Bitmap));
                        box->addItem ("Code", QVariant(RowData::Code));                                
                } else {                                
                        box->addItem ("Add", QVariant(RowData::GroupAddition));
                        box->addItem ("Subtract", QVariant(RowData::GroupSubtraction));
                        box->addItem ("Multiply", QVariant(RowData::GroupMultiplication));
                        box->addItem ("Divide", QVariant(RowData::GroupDivision));
                        box->addItem ("Lerp", QVariant(RowData::GroupLerp));
                }                        
                
                box->setSizeAdjustPolicy (QComboBox::AdjustToContents);                        
                
                for (int i=0; i<box->count(); ++i) {
                        if (data.type == box->itemData (i).toInt()) {
                                box->setCurrentIndex (i);
                        }
                }                        
                
                QTableWidgetItem *item = new QTableWidgetItem(tr(""));                        
                item->setSizeHint( box->sizeHint() );
                table->setItem (rowIndex, Indices::Type, item);
        }
  
        // Icon size preview
        {
                QTableWidgetItem *item = new QTableWidgetItem(tr(""));
                item->setIcon(QIcon(
                               QPixmap(":/redshiftLogo/images/redshift.png")));
                item->setTextAlignment(Qt::AlignVCenter);                
                table->setItem (rowIndex, Indices::Preview, item);
        }

        // Name
        {
                QTableWidgetItem *item = new QTableWidgetItem(data.name);
                item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                table->setItem (rowIndex, Indices::Name, item);
        }
        
        // This is a hack to align all cell elements correctly
        // (resizeRowsToContents() alone won't align elements; but when the 
        // window gets resized, all seems to align properly, so we force a 
        // resize here)
        const QSize oldSize (table->size());
        const QSize newSize (oldSize.width(), oldSize.height()+1);
        table->resizeRowsToContents();
        table->resize(newSize);
        table->resize(oldSize);
}



void HeightmapLayersImpl::colorizeGroups () {
        srand (16); // No I am not going to use a Mersenne Twister here. lulz.
        
        QString color [2] = {
                "#333333", "#CCCCCC"
        };
        const int colorCount = sizeof color / sizeof color[0];
        int colorId = 0;
        
        for (int i=0; i<table->rowCount(); ++i) {
                const RowData::Type type = composition.data[i].type;
                if (type >= RowData::Group) {                        
                        ++colorId;
                        colorId %= colorCount;
                }                                
                
                const QString stylesheet = "QFrame { background-color: " + 
                                                color [colorId] + "}";

                QFrame *gi = new QFrame ();
                gi->setStyleSheet (stylesheet);
                table->setCellWidget (i, Indices::LeftBorder, gi);
                
                gi = new QFrame ();
                gi->setStyleSheet (stylesheet);
                table->setCellWidget (i, Indices::RightBorder, gi);
        }
}
 


void HeightmapLayersImpl::on_openDefinition_clicked() {
        const int row = table->currentRow();
        if (0>row) {
                return;
        }
        
        syncFromView();
        const RowData data = composition.data [row];//getRowData (row);
        
        switch (data.type) {
        case RowData::FirstClass:
                std::cout << "must. open. presets-dialog." << std::endl;
                break;
        case RowData::Bitmap:
                std::cout << "must. open. bitmap-dialog." << std::endl;
                break;
        case RowData::Composition: {
                std::cout << "must. open. composition dialog." << std::endl;
                HeightmapLayersImpl *hd = new HeightmapLayersImpl (mdiArea);        
                QMdiSubWindow *sub = mdiArea->addSubWindow(hd);                
                sub->show();
                } break;
        case RowData::Code: {
                QuatschEditorImpl *ed = new QuatschEditorImpl ();
                QMdiSubWindow *sub = mdiArea->addSubWindow(ed);
                sub->show();
                } break;
                
        case RowData::GroupAddition:                
                break;
        case RowData::GroupSubtraction:                
                break;
        case RowData::GroupMultiplication:                
                break;
        case RowData::GroupDivision:                
                break;
        case RowData::GroupLerp:                
                break;        
        };
}



void HeightmapLayersImpl::makeVisibilityCheckBox (
        QCheckBox *box, bool checked
) const {
        box->setChecked (checked);
        box->adjustSize();
        box->setStyleSheet (
          "QCheckBox::indicator {"
          "   width:16px;"
          "   height: 16px;"
          "}"
          "QCheckBox::indicator:unchecked {"
          "   image: url(:/visible/icons/darkglass _reworked/1239215171_stop.png)"
          "}"
          "QCheckBox::indicator:checked {"
          "   image: url(:/visible/icons/darkglass _reworked/1239214390_xeyes.png)"
          "}"
        );
}



void HeightmapLayersImpl::makeLockingCheckBox (
        QCheckBox *box, bool checked
) const {
        box->setChecked (checked);
        box->adjustSize();
        box->setStyleSheet (
               "QCheckBox::indicator {"
               "   width:16px;"
               "   height: 16px;"
               "}"
               "QCheckBox::indicator:unchecked {"
               ""
               "}"
               "QCheckBox::indicator:checked {"
               "   image: url(:/locked/icons/darkglass/1239259860_gpg.png)"
               "}"
        );        
}



void HeightmapLayersImpl::on_newLayer_clicked() {
        const int index = table->currentRow() >= 0
                        ? 1+table->currentRow()
                        : table->rowCount();
        composition.data.push_back ( 
                RowData (composition.generateId(),
                        true, false, RowData::FirstClass, tr("New Layer"))
        );
        syncToView ();
        colorizeGroups ();
}



void HeightmapLayersImpl::on_deleteLayer_clicked() {
        if (QMessageBox::Yes == QMessageBox::question (0, "Deleting Row.",
                "Do you want to delete the selected row?",
                QMessageBox::Yes |  QMessageBox::Cancel, QMessageBox::Cancel))
        {
                //table->removeRow (table->currentRow());
                composition.data.erase (
                        composition.data.begin()+table->currentRow()
                );
                syncToView ();
                colorizeGroups ();
        }        
}



void HeightmapLayersImpl::on_newGroup_clicked() {
        const int index = table->currentRow() >= 0
                        ? 1+table->currentRow()
                        : table->rowCount();
        composition.data.push_back ( 
                RowData (composition.generateId(), 
                        true, false, RowData::Group, tr("New Group"),Addition)
        );
        syncToView ();
        colorizeGroups ();
}



void HeightmapLayersImpl::on_showJuxCode_clicked() {
        int juxFunctionId = 0;
        QString callCode;
        syncFromView();
        const QString code = generateJuxCode(juxFunctionId, callCode, 
                                             composition, 0);
        QMessageBox box (
                QMessageBox::Information, 
                "jux code",
                code + "\n" + callCode                
        );
        box.exec();
}



QString HeightmapLayersImpl::getJuxIndendationString (int indent) const {
        QString ret = "";
        for (int i=0; i<indent; ++i)
                ret += "    ";
        return ret;
}



QString HeightmapLayersImpl::generateJuxCode (
        Composition const &composition, 
        int indent_, 
        size_t &rowIndex,
        bool startsInGroup
) const {

        const QString indent = getJuxIndendationString (indent_);
        QString code;
        
        while (rowIndex<composition.data.size()) {
                switch (composition.data[rowIndex].type) {
                // primitives
                case RowData::FirstClass: 
                        code += indent + "(first-class x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Bitmap:
                        code += indent + "([bitmap filename()] x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Composition:
                        code += indent + "([bitmap filename()] x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Code:
                        code += indent + "(code code code)\n";
                        ++ rowIndex;
                        break;
                
                // aggregates
                case RowData::GroupAddition:
                        if (startsInGroup) 
                                return code;
                        code += indent + "( +\n";
                        goto rest;   
                        
                case RowData::GroupSubtraction:
                        if (startsInGroup) 
                                return code;
                        code += indent + "( -\n";
                        goto rest;
                        
                case RowData::GroupMultiplication:
                        if (startsInGroup) 
                                return code;
                        code += indent + "( *\n";
                        goto rest;
                        
                case RowData::GroupDivision:
                        if (startsInGroup) 
                                return code;
                        code += indent + "( /\n";
                        goto rest;
                        
                case RowData::GroupLerp:
                        if (startsInGroup) 
                                return code;
                        code += indent + "( lerp X\n";
                        goto rest;
                        
                rest:
                        code += generateJuxCode (composition, 1+indent_, 
                                                        ++rowIndex, true);
                        code += indent + ")\n";
                        break;
                };                
        }
        return code;
}



QString HeightmapLayersImpl::generateJuxCode(
        int &juxFunctionId, QString &callCode, 
        Composition const &composition, int recDepth
) const {

        int indent_ = 0;
        QString indent = getJuxIndendationString (indent_);
        QString code = indent + "// Auto generated\n";
        
        // Set top level type.
        code += indent + "( ";
        switch (composition.type) {
        case Addition:
                code += indent + "+ \n";
                break;
        case Subtraction:
                code += indent + "- \n";
                break;
        case Multiplication:
                code += indent + "* \n";
                break;
        case Division:
                code += indent + "/ \n";
                break;
        case Lerp:
                code += indent + "lerp X \n";
                break;
        };
        
        for (size_t i=0; i<composition.data.size(); ++i) {
                code += generateJuxCode (composition, 1+indent_, i);
        }
        
        code += ")";

        return code;
}



void HeightmapLayersImpl::syncFromView () {
        if ((size_t)table->rowCount() != composition.data.size()) {
                composition.data.resize (table->rowCount());
        }
        
        composition.type = (CompositionType)compositionType->
                        itemData(compositionType->currentIndex()).toInt();
        
        for (int i=0; i<table->rowCount(); ++i) {
                composition.data [i] = rowFromView (i);
        }        
}



void HeightmapLayersImpl::syncToView () {
        if ((size_t)table->rowCount() != composition.data.size()) {
                table->setRowCount (composition.data.size());
        }
        for (size_t i=0; i<composition.data.size(); ++i) {
                rowToView (i, composition.data [i]);
        }
        colorizeGroups ();
}
