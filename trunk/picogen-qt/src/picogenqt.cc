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

#include "../include/picogenqt.hh"
#include "../include/camerasettings.hh"
#include "../include/heightmap-designer.hh"
#include "../include/scene-display.hh"




int picogen_main ();

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
