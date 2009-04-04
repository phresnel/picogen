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

#include <SDL.h>

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>

#ifdef AMALGAM
#include "../../include/redshift.hh"
#include "../../include/rendertargets/sdlrendertarget.hh"
#include "../../include/rendertargets/colorrendertarget.hh"
#include "../../include/cameras/pinhole.hh"
#include "../../include/interaction/sdlcommandprocessor.hh"
#else
#include "../include/redshift.hh"
#include "../include/rendertargets/sdlrendertarget.hh"
#include "../include/rendertargets/colorrendertarget.hh"
#include "../include/cameras/pinhole.hh"
#include "../include/interaction/sdlcommandprocessor.hh"
#endif



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
                                              int completed, int total) const {                                              

                clock_t const curr = clock();
                if (curr - lastTime < (CLOCKS_PER_SEC/2))
                        return;
                lastTime = clock();
                
                real_t const finished = static_cast<real_t>(completed)
                                          / static_cast<real_t>(total);
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
                real_t const d = sqrt (u*u + v*v);
                return (sin(u*0.7)*sin(v*0.7)) - 4;
        }
};


void run() {
        using namespace redshift;
        using namespace redshift::camera;
        using namespace redshift::interaction;
        using namespace redshift::primitive;

        // TODO replace RenderTarget with Film?
        //    i mean, a "RenderTarget" might be flipable, but a Film not, or so
        int const width = 1024;
        int const height = width;
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
                        0.25
                )
        );
        
        Scene Scene (renderBuffer, camera, agg);

        RenderTarget::Ptr screenBuffer (new SdlRenderTarget(width,height));
        
        UserCommandProcessor::Ptr commandProcessor (new SdlCommandProcessor());

        ProgressReporter::Ptr reporter (
                  new RenderTargetCopyingReporter(renderBuffer, screenBuffer));        
        
        Scene.render(reporter, commandProcessor);
        copy (renderBuffer, screenBuffer);
        screenBuffer->flip(); 
        
        //commandProcessor->waitForQuit();
        
}

#ifdef PICOGENLIB
int picogen_main ()
#else
int main ()
#endif
{
        using namespace redshift;
        
        try {
                // Initialize SDL video.
                if (SDL_Init (SDL_INIT_VIDEO) < 0) {
                        std::stringstream ss;
                        ss <<  "Unable to init SDL:\n" << SDL_GetError();
                        throw std::runtime_error (ss.str());
                }
                atexit(SDL_Quit);
        
                run();

        } catch (std::runtime_error &ex) {
                std::cerr << "Caught runtime error: " 
                          << ex.what()
                          << std::endl;
        } catch (std::exception &ex) {
                std::cerr << "Caught general exception: " 
                          << ex.what()
                          << std::endl;
        } 
}
