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

#include <QMessageBox>

#include "../../redshift/include/redshift.hh"

#include "../include/picogenqt.hh"



PicogenQTImpl::PicogenQTImpl(QWidget *parent)
: QMainWindow (parent), PicogenQT() 
{
        /*setupUi(this);
        QMetaObject::connectSlotsByName(this);*/
        //setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);        
        
        setCentralWidget (mdiArea);
        
        /*{
                HeightmapDesignerImpl *hd = new HeightmapDesignerImpl;        
                QMdiSubWindow *sub = mdiArea->addSubWindow(hd);                
                sub->show();
        }
        
        {
                CameraSettingsImpl *cs = new CameraSettingsImpl;        
                QMdiSubWindow *sub = mdiArea->addSubWindow(cs);                
                sub->show();
        }*/        
        
        {
                SceneDisplayImpl *cs = new SceneDisplayImpl;        
                QMdiSubWindow *sub = mdiArea->addSubWindow(cs);                
                sub->show();
        }
}



void PicogenQTImpl::on_actionCamera_triggered () {
        CameraListImpl *cs = new CameraListImpl;        
        QMdiSubWindow *sub = mdiArea->addSubWindow(cs);
        connect(cs, SIGNAL(openCameraSettings (const QString &)),
             this, SLOT(openCameraSettings(const QString &)));
        connect(cs, SIGNAL(renameCamera (const QString &, const QString &)),
             this, SLOT(renameCamera (const QString &, const QString &)));        
        sub->show();
}



void PicogenQTImpl::on_actionSun_and_Sky_triggered () {
}



void PicogenQTImpl::openCameraSettings (QString const &id) {
        if (cameraSettings.contains (id)) {
                cameraSettings [id]->setFocus();
        } else {
                CameraSettingsImpl *cs = new CameraSettingsImpl;
                cs->renameCamera (id);
                QMdiSubWindow *sub = mdiArea->addSubWindow(cs);
                cameraSettings [id] = cs;
                sub->show();
                
                connect(cs, SIGNAL(closeCamera (const QString &)),
                        this, SLOT(closeCamera (const QString &)));
        }
}



void PicogenQTImpl::on_actionTerrain_Definition_triggered () {
        /*
        HeightmapDesignerImpl *hd = new HeightmapDesignerImpl;
        QMdiSubWindow *sub = mdiArea->addSubWindow(hd);                
        sub->show();
        */
        HeightmapLayersImpl *hd = new HeightmapLayersImpl (mdiArea);        
        QMdiSubWindow *sub = mdiArea->addSubWindow(hd);                
        sub->show();
}



void PicogenQTImpl::renameCamera (
        QString const &oldName, QString const &newName
) {
        if (cameraSettings.contains (oldName)) {
                cameraSettings [oldName]->renameCamera (newName);
                cameraSettings [newName] = cameraSettings [oldName];
                cameraSettings.remove (oldName);
        }
}



void PicogenQTImpl::closeCamera (QString const &name) {
        std::cout << "remove " << name.toStdString() << std::endl;
        cameraSettings.remove (name);
}
