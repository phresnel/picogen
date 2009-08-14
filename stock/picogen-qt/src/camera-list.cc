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

#include "../../redshift/include/redshift.hh"

#include "../include/camera-list.hh"



CameraListImpl::CameraListImpl(QWidget *parent)
: QWidget (parent), CameraList() 
{
        setupUi(this);        
}



void CameraListImpl::on_newCamera_clicked() {
        QListWidgetItem *item = new QListWidgetItem (
                               "Camera " + QString().setNum(list->count(),10));
        
        item->setFlags (Qt::ItemIsSelectable
                       |Qt::ItemIsEditable
                       |Qt::ItemIsEnabled);

        list->addItem (item);
}



void CameraListImpl::on_openSettings_clicked() {
        if (0 != list->currentItem())
                emit openCameraSettings (list->currentItem()->text());
}



void CameraListImpl::on_list_itemChanged (QListWidgetItem * item) {
        item->setText (item->text().simplified());
        emit renameCamera (currentItemOldText, item->text());
        currentItemOldText = item->text();
}



void CameraListImpl::on_list_currentItemChanged (
        QListWidgetItem *current, QListWidgetItem * /*previous*/
) {
        currentItemOldText = current->text();
}
