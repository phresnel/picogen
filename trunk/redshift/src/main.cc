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
#else
#include "../include/redshift.hh"
#include "../include/rendertargets/sdlrendertarget.hh"
#include "../include/rendertargets/colorrendertarget.hh"
#include "../include/cameras/pinhole.hh"
#endif


// TODO: write a pure console reporter
// TODO: reporter should react on user entry (esp. <escape>)
class MyProgressReporter : public redshift::ProgressReporter {
public:
        
        
        
        MyProgressReporter (
                redshift::RenderTarget::Ptr src,
                redshift::RenderTarget::Ptr target_
        ) : source (src), target (target_)
        {}
        
        
        
        void report (int completed, int total) const {
                using redshift::real_t;
                using std::cout; using std::endl;
                if (0 == completed % (target->getWidth()*16)) {
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
                        redshift::copy (source, target);
                        target->flip();
                }
        }
        
        
        
        void reportDone () const {
                redshift::copy (source, target);
                target->flip();
        }
        
        
        
        bool doQuit () const {
                return false;
        }
        
        
        
private:
        MyProgressReporter();
        MyProgressReporter(MyProgressReporter const &);
        MyProgressReporter & operator = (MyProgressReporter const &);

        redshift::RenderTarget::Ptr source;
        redshift::RenderTarget::Ptr target;
};



void run() {
        using namespace redshift;
        using namespace redshift::camera;

        // TODO replace RenderTarget with Film?
        //    i mean, a "RenderTarget" might be flipable, but a Film not, or so
        int const width = 512;
        int const height = 512;
        RenderTarget::Ptr renderBuffer (new ColorRenderTarget(width,height));        
        shared_ptr<Camera> camera (new Pinhole(renderBuffer));
        shared_ptr<primitive::Primitive> agg (
                new primitive::ClosedSphere(
                        Point(scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(25)),
                        10.0)
        );
        
        Scene Scene (renderBuffer, camera, agg);

        RenderTarget::Ptr screenBuffer (new SdlRenderTarget(width,height));
        ProgressReporter::Ptr reporter (
                        new MyProgressReporter(renderBuffer, screenBuffer));
        Scene.render(reporter);
        copy (renderBuffer, screenBuffer);
        screenBuffer->flip();
        
        SDL_WM_SetCaption ("redshift-sdl.", "redshift-sdl.");
        bool done = false;
        while (!done) {
                // message processing loop
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        // check for messages
                        switch (event.type) {
                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                                if (SDLK_ESCAPE != event.key.keysym.sym)
                                        break;
                                // Fall through
                        case SDL_QUIT:
                                done = true;
                                break;
                                                
                        }
                }                       
        }
}



int main () {
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
