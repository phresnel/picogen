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

#ifndef CAMERA_LIST_HH_INCLUDED_20090406
#define CAMERA_LIST_HH_INCLUDED_20090406

#include "ui_camera-list.h"
#include "../include/camerasettings.hh"


class CameraListImpl : public QWidget, private Ui::CameraList
{
        Q_OBJECT

public:
        CameraListImpl(QWidget* parent=0);
        virtual ~CameraListImpl() {}        

private slots:
        void on_newCamera_clicked();
        void on_openSettings_clicked();
        
        void on_list_itemChanged (QListWidgetItem * item);
        void on_list_currentItemChanged (QListWidgetItem *current, 
                                         QListWidgetItem *previous);
        
signals:
        void openCameraSettings(QString const &);
        void renameCamera (QString const &oldName, QString const &newName);

private:
        QString currentItemOldText;
};


#endif // CAMERA_LIST_HH_INCLUDED_20090406
