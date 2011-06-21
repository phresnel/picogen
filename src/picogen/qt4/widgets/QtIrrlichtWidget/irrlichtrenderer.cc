//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#include "irrlichtrenderer.hh"
#include "ui_irrlichtrenderer.h"

#include <irrlicht/irrlicht.h>


#include <QResizeEvent>

// Based on http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=15112

IrrlichtRenderer::IrrlichtRenderer(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::IrrlichtRenderer),
        device(0)
{
        ui->setupUi(this);
        setAttribute (Qt::WA_OpaquePaintEvent);

        resize(width(), height());

        redraw = true;
        startTimer(250); // <-- strange thing:
        // I was rendering irrlicht within paintEvent(). This was fine when
        // I called repaint() from another event handler (like buttonClicked()),
        // but paintEvent() implicitly called via resizing did not work, not even
        // when calling onPaint() directly, so I guess some implicitly called
        // slot or event erases the screen content ... meh.
}



IrrlichtRenderer::~IrrlichtRenderer() {
        if (device) {
                device->closeDevice();
                device->drop();
        }
        delete ui;
}



void IrrlichtRenderer::resizeEvent(QResizeEvent *event) {
        if (device) {
                irr::core::dimension2d<irr::u32> size;
                size.Width = event->size().width();
                size.Height = event->size().height();
                device->getVideoDriver()->OnResize(size);

                irr::scene::ICameraSceneNode *cam =
                                device->getSceneManager()->getActiveCamera();
                if (cam)  {
                    cam->setAspectRatio( size.Height / size.Width );
                }
        }
        QWidget::resizeEvent(event);
        redraw = true;
}



void IrrlichtRenderer::timerEvent(QTimerEvent *) {
        if (!redraw) return;
        initIrrlicht();
        drawIrrlicht();
}



void IrrlichtRenderer::initIrrlicht() {
        if (device) return;
        redraw = false;

        irr::SIrrlichtCreationParameters params;
        params.WindowSize.Width  = width();
        params.WindowSize.Height = height();
        params.WindowId          = (void*)winId();
        params.DriverType        = irr::video::EDT_BURNINGSVIDEO;
        params.Doublebuffer      = false;

        device = irr::createDeviceEx(params);
}


void IrrlichtRenderer::drawIrrlicht() {
        if (device) {
                device->getVideoDriver()->beginScene(true, true,
                                                     irr::video::SColor(255,100,101,140));

                device->getSceneManager()->drawAll();
                device->getVideoDriver()->endScene();
        }
}

