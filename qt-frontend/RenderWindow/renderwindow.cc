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



#include "renderwindow.hh"
#include "ui_renderwindow.h"
#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/interaction/rendertarget-copying-reporter.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include <QImage>
#include "qimagerendertarget.hh"


//=============================================================================
// RenderWindowImpl
//=============================================================================
class RenderWindowImpl {
public:
        RenderWindowImpl (RenderWindow *rw);
        virtual ~RenderWindowImpl ();

private:
        RenderWindow *parent;
        redshift::shared_ptr<
                redshift::interaction::RenderTargetCopyingReporter
        > rep;

        redshift::shared_ptr<redshift::QImageRenderTarget> qrt;
        redshift::shared_ptr<redshift::ColorRenderTarget> crt;
};



RenderWindowImpl::RenderWindowImpl (RenderWindow *rw) :
        parent(rw)
{
        using namespace redshift;
        using namespace redshift::interaction;
        crt = shared_ptr<ColorRenderTarget>(new ColorRenderTarget (320, 240));
        qrt = shared_ptr<QImageRenderTarget>(new QImageRenderTarget (320, 240));
        rep = shared_ptr<RenderTargetCopyingReporter>
                (new interaction::RenderTargetCopyingReporter(crt, qrt));

}



RenderWindowImpl::~RenderWindowImpl () {
}




//=============================================================================
// RenderWindow
//=============================================================================
RenderWindow::RenderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderWindow)
{
        ui->setupUi(this);
        impl = new RenderWindowImpl(this);

        QImage map (320, 240, QImage::Format_RGB32);
        for (int y=0; y<map.height(); ++y) {
                for (int x=0; x<map.width(); ++x) {
                        map.setPixel(x, y, QColor(122,64,32).rgb());
                }
        }
        ui->pix->setPixmap(QPixmap::fromImage(map));
}

RenderWindow::~RenderWindow()
{
        delete impl;
        delete ui;
}

void RenderWindow::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}
