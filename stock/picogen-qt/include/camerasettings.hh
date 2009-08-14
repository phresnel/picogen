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

#ifndef CAMERASETTINGS_HH_INCLUDED_20090323
#define CAMERASETTINGS_HH_INCLUDED_20090323

#include <QScrollArea>
#include <QTextEdit>
#include "ui_camerasettings.h"


class CameraSettingsImpl : public QWidget, private Ui::CameraSettings
{
        Q_OBJECT

public:
        CameraSettingsImpl(/*QWidget* parent=0*/);
        virtual ~CameraSettingsImpl();

        void renameCamera (QString const &newName);               

private slots:

signals:
        void closeCamera (QString const &cameraName);
 
private:
        QString name;
};


#endif // HEIGHTFIELDPROJECT_H_INCLUDED_20090323
