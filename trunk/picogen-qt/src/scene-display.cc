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

#include "../include/scene-display.hh"

/////////////////////////////
#include <iostream>

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include "../../redshift/include/cameras/pinhole.hh"
//#include "../../redshift/include/interaction/sdlcommandprocessor.hh"

#include "../include/qimagerendertarget.hh"



Reporter::Reporter (
        SceneDisplayThread &sceneDisplayThread_,
        redshift::RenderTarget::Ptr src,
        redshift::RenderTarget::Ptr target_
)
: lastReportTime (clock()), renderBuffer (src), screenBuffer (target_)
, sceneDisplayThread(sceneDisplayThread_)
{}



void Reporter::report (redshift::RenderTarget::ReadLockPtr sourcel,
        int /*completed*/,
        int /*total*/
) const {

        const clock_t curr = clock();
        if (curr - lastReportTime < (CLOCKS_PER_SEC/2))
                return;
        lastReportTime = clock();

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
        copy (renderBuffer, sourcel, screenBuffer);
        screenBuffer->flip();
        sceneDisplayThread.reportPartialImage();
}



void Reporter::reportDone () const {
        redshift::copy (renderBuffer, screenBuffer);
        screenBuffer->flip();
        sceneDisplayThread.reportFullImage();
}




#ifdef AMALGAM
#include "../../../quatsch/quatsch.hh"
#include "../../../quatsch/frontend/jux.hh"
#include "../../../quatsch/backend/est/backend.hh"
#else
#include "../../quatsch/quatsch.hh"
#include "../../quatsch/frontend/jux.hh"
#include "../../quatsch/backend/est/backend.hh"
#endif

namespace redshift {
class QuatschHeightFunction : public redshift::primitive::HeightFunction {
private:
        // quatsch
        typedef quatsch::backend::est::Backend <real_t, const real_t *> backend_t;

        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;

        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;

        struct FunctionSet {
                Compiler::ConfigurableFunctionsMap cfm;
                operator Compiler::ConfigurableFunctionsMap () {
                        return cfm;
                }
        };

        FunctionSet functionSet;
        Compiler::FunctionPtr fun;
        std::stringstream errors;

public:
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                const real_t p [] = { u, v };
                return (*fun) (p);
        }


        QuatschHeightFunction ()
        : fun (Compiler::compile (
                "x;y",
                "(- (* (sin (* x 0.7)) (sin(* y 0.7)) ) 4)",
                functionSet,
                errors))
        {
        }
};
} // namespace redshift

class HeightFunction : public redshift::primitive::HeightFunction {
        typedef redshift::real_t real_t;
        typedef redshift::fixed_point_t fixed_point_t;
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                return (sin(u*0.7)*sin(v*0.7)) - 4;
        }
};



#include <iostream>
using namespace std;



class DummySubject : public SceneDisplaySubject {
public:
        void render (int /*width*/, int /*height*/,
                     redshift::RenderTarget::Ptr renderBuffer,
                     redshift::RenderTarget::Ptr screenBuffer,
                     redshift::shared_ptr<redshift::interaction::
                                          ProgressReporter>  reporter
        ) {
                using namespace redshift;
                using namespace redshift::camera;
                using namespace redshift::interaction;
                using namespace redshift::primitive;

                // TODO replace RenderTarget with Film?
                //    i mean, a "RenderTarget" might be flipable, but a Film not, or so

                shared_ptr<Camera> camera (new Pinhole(renderBuffer));

                shared_ptr<primitive::HeightFunction> heightFunction;
                try {
                        heightFunction = shared_ptr<primitive::HeightFunction> (
                                              new ::redshift::QuatschHeightFunction());
                } catch (...) { // TODO (!!!)
                }

                shared_ptr<primitive::Primitive> agg (
                        /*new primitive::ClosedSphere(
                                Point(scalar_cast<fixed_point_t>(0),
                                        scalar_cast<fixed_point_t>(0),
                                        scalar_cast<fixed_point_t>(25)),
                                10.0)*/
                        new Heightmap (heightFunction, 0.25)
                );

                Scene Scene (renderBuffer, camera, agg);


                UserCommandProcessor::Ptr commandProcessor (new PassiveCommandProcessor());

                Scene.render(reporter, commandProcessor);


                copy (renderBuffer, screenBuffer);
                screenBuffer->flip();

                //commandProcessor->waitForQuit();
                //emit fullImage (*((QImageRenderTarget*)&*screenBuffer));
        }
};



void SceneDisplayThread::run() {
        int const width = widthForNextRender;
        int const height = heightForNextRender;

        this->screenBuffer =
           redshift::RenderTarget::Ptr(
                                new redshift::QImageRenderTarget(width,height));

        this->renderBuffer =
           redshift::RenderTarget::Ptr(new redshift::ColorRenderTarget(width,height));

        this->reporter =
                redshift::shared_ptr<redshift::interaction::ProgressReporter>(
                        new Reporter (*this, renderBuffer, screenBuffer));

        if (subject.get()) {
                subject->render(width, height,
                                this->renderBuffer,
                                this->screenBuffer,
                                this->reporter
                               );
        }}



void SceneDisplayThread::reportPartialImage() {
        using namespace redshift;
        emit partialImage (*((QImageRenderTarget*)&*screenBuffer));
}



void SceneDisplayThread::reportFullImage() {
        using namespace redshift;
        emit fullImage (*((QImageRenderTarget*)&*screenBuffer));
}



void SceneDisplayThread::setDisplaySubject (
                redshift::shared_ptr<SceneDisplaySubject> subject) {
        this->subject = subject;
}






SceneDisplayImpl::SceneDisplayImpl(QWidget* parent)
: QWidget (parent), mustReRender (false)
{
        qRegisterMetaType<QImage>("QImage");
        connect(&renderThread, SIGNAL(partialImage (const QImage&)),
             this, SLOT(updateDisplay(const QImage&)));
        connect(&renderThread, SIGNAL(fullImage (const QImage &)),
             this, SLOT(updateDisplay(const QImage &)));
        connect(&renderThread, SIGNAL(fullImage (const QImage &)),
             this, SLOT(fullImage()));


        //setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);

        renderThread.widthForNextRender = width();
        renderThread.heightForNextRender = height();
        //renderThread.start();

        renderThread.setDisplaySubject(
                        redshift::shared_ptr<SceneDisplaySubject> (
                                new DummySubject ()));
}



SceneDisplayImpl::~SceneDisplayImpl() {
}



void SceneDisplayImpl::setSubject (
        redshift::shared_ptr<SceneDisplaySubject> subject
) {
        renderThread.setDisplaySubject(subject);
}



void SceneDisplayImpl::updateDisplay(QImage const &image) {
        QPixmap pix = QPixmap::fromImage (image);
        imageLabel->setPixmap (pix);
        imageLabel->resize (pix.width(), pix.height());
        //setMinimumSize (pix.width(), pix.height());
        update();
}



void SceneDisplayImpl::fullImage() {
        if (mustReRender) {
                mustReRender = false;
                renderThread.widthForNextRender = width();
                renderThread.heightForNextRender = height();
                renderThread.start();
        }
}



void SceneDisplayImpl::resizeEvent(QResizeEvent *event) {
/*
        renderThread.widthForNextRender = event->size().width();
        renderThread.heightForNextRender = event->size().height();
        if (renderThread.isRunning()) {
                mustReRender = true;
        } else {
                mustReRender = false;
                renderThread.start();
        }*/
        QWidget::resizeEvent (event);
}



void SceneDisplayImpl::paintEvent (QPaintEvent * event) {
        if (true/*lastRenderedWidth != this->width()
            || lastRenderedHeight != this->height()*/
        ) {
                lastRenderedWidth =
                 renderThread.widthForNextRender = this->width();
                lastRenderedHeight =
                 renderThread.heightForNextRender = this->height();

                if (renderThread.isRunning()) {
                        mustReRender = true;
                } else {
                        mustReRender = false;
                        renderThread.start();
                }
        }
        QWidget::paintEvent (event);
}

