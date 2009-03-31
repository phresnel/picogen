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


/////////////////////////////
#include <iostream>
#include <SDL/SDL.h>

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include "../../redshift/include/cameras/pinhole.hh"
#include "../../redshift/include/interaction/sdlcommandprocessor.hh"

#include "../include/qimagerendertarget.hh"

namespace redshift { namespace interaction {
class RenderTargetCopyingReporter : public ProgressReporter {
public:        
        
        RenderTargetCopyingReporter (
                RenderTarget::Ptr src,
                RenderTarget::Ptr target_
        ) 
        : source (src), target (target_), lastTime (clock())
        {}
        
        
        
        void report (RenderTarget::ReadLockPtr sourcel, 
                int /*completed*/,
                int /*total*/
        ) const {

                clock_t const curr = clock();
                if (curr - lastTime < (CLOCKS_PER_SEC/2))
                        return;
                lastTime = clock();
                
                /*real_t const finished = static_cast<real_t>(completed)
                                          / static_cast<real_t>(total);*/
                /*
                if (total>0.0) {
                        cout << real_t(100)*(finished) << "%"
                             << endl;
                } else {
                        cout << completed << endl; 
                }
                */
                copy (source, sourcel, target);
                target->flip();
        }
        
        
        
        void reportDone () const {
                redshift::copy (source, target);
                target->flip();
        }
        
private:
        RenderTargetCopyingReporter();
        RenderTargetCopyingReporter(RenderTargetCopyingReporter const &);
        RenderTargetCopyingReporter &
                        operator = (RenderTargetCopyingReporter const &);
        
        RenderTarget::ConstPtr source;
        RenderTarget::Ptr target;
        
        mutable clock_t lastTime;
};

} }


class HeightFunction : public redshift::primitive::HeightFunction {
        typedef redshift::real_t real_t;
        typedef redshift::fixed_point_t fixed_point_t;
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                return (sin(u*0.7)*sin(v*0.7)) - 4;
        }
};


QImage run() {
        using namespace redshift;
        using namespace redshift::camera;
        using namespace redshift::interaction;
        using namespace redshift::primitive;

        // TODO replace RenderTarget with Film?
        //    i mean, a "RenderTarget" might be flipable, but a Film not, or so
        int const width = 1024;
        int const height = 1024;
        RenderTarget::Ptr renderBuffer (new ColorRenderTarget(width,height));        
        shared_ptr<Camera> camera (new Pinhole(renderBuffer));
        shared_ptr<primitive::Primitive> agg (
                /*new primitive::ClosedSphere(
                        Point(scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(25)),
                        10.0)*/
                new Heightmap (
                        shared_ptr<primitive::HeightFunction>(
                                new ::HeightFunction()
                        ),
                        15.0
                )
        );
        
        Scene Scene (renderBuffer, camera, agg);

        RenderTarget::Ptr screenBuffer (new QImageRenderTarget(width,height));
        
        UserCommandProcessor::Ptr commandProcessor (new SdlCommandProcessor());

        ProgressReporter::Ptr reporter (
                  new RenderTargetCopyingReporter(renderBuffer, screenBuffer));        
        
        Scene.render(reporter, commandProcessor);
        copy (renderBuffer, screenBuffer);
        screenBuffer->flip(); 
        
        return *((QImageRenderTarget*)&*screenBuffer);
        //commandProcessor->waitForQuit();        
}



void MyThread::run() {
        qimage = ::run();              
        //exec();
}
/////////////////////////////


int picogen_main ();

PicogenQTImpl::PicogenQTImpl(QWidget *parent)
: QMainWindow (parent), PicogenQT() 
{
        setupUi(this);
        QMetaObject::connectSlotsByName(this);
        
        setCentralWidget (mdiArea);
        
        {
                HeightmapDesignerImpl *hd = new HeightmapDesignerImpl;        
                QMdiSubWindow *sub = mdiArea->addSubWindow(hd);                
                sub->show();
        }
        
        {
                CameraSettingsImpl *cs = new CameraSettingsImpl;        
                QMdiSubWindow *sub = mdiArea->addSubWindow(cs);                
                sub->show();
        }        
        
        
        /*MyThread* thread = new MyThread (imageLabel);
        thread->start();*/
        //my_thread.start();
        
        QObject::connect(&my_thread, SIGNAL (finished()), this, SLOT(thread_finished()));
        
        /*QPixmap pix = QPixmap::fromImage (run ());
        imageLabel->resize (pix.width(), pix.height());
        imageLabel->setPixmap (pix);*/        
                
        //picogen_main();        
}

void PicogenQTImpl::thread_finished() {
        QPixmap pix = QPixmap::fromImage (my_thread.qimage);
        imageLabel->resize (pix.width(), pix.height());
        imageLabel->setPixmap (pix);
}

/*
void PicogenQTImpl::on_edRed_textChanged(QString value) {
}

void PicogenQTImpl::on_edGreen_textChanged(QString value) {
        exit(0);
}

void PicogenQTImpl::on_edBlue_textChanged(QString value) {
}*/