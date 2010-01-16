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
#include "../../include/basictypes/height-function.hh"
#else
#include "../include/redshift.hh"
#include "../include/rendertargets/sdlrendertarget.hh"
#include "../include/rendertargets/colorrendertarget.hh"
#include "../include/cameras/pinhole.hh"
#include "../include/interaction/sdlcommandprocessor.hh"
#include "../include/basictypes/height-function.hh"
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


#ifdef AMALGAM
#include "../../../quatsch/quatsch.hh"
#include "../../../quatsch/frontend/jux.hh"
#include "../../../quatsch/backend/est/backend.hh"
#else
#include "../../quatsch/quatsch.hh"
#include "../../quatsch/frontend/jux.hh"
#include "../../quatsch/backend/est/backend.hh"
#endif
#include "quatsch/configurable-functions/noise2ddef.hh"
#include "quatsch/configurable-functions/layerednoise2ddef.hh"

namespace redshift {
class QuatschHeightFunction : public redshift::HeightFunction {
private:
        // quatsch
        typedef quatsch::backend::est::Backend <redshift::real_t, const redshift::real_t *> backend_t;   
        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;
        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;
        typedef Compiler::ConfigurableFunctionsMap FunctionSet;
        
        static Compiler::ConfigurableFunctionsMap addfuns() {
                using namespace redshift;
                
                
                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: Noise2d <Function, Compiler>,  
                                Function
                        >
                );
                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr layeredNoise2dDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: LayeredNoise2d <Function, Compiler>,  
                                Function
                        >
                );
                
                Compiler::ConfigurableFunctionsMap addfuns;
                addfuns.addSymbol ("Noise2d", noiseDesc);
                addfuns.addSymbol ("LayeredNoise2d", layeredNoise2dDesc);
                return addfuns;
        }
        
        FunctionSet functionSet;
        Compiler::FunctionPtr fun;
        std::stringstream errors;

public:
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                const real_t p [] = { u, v };
                return (*fun) (p) - 20;
        }
        
        
        QuatschHeightFunction ()
        : functionSet(addfuns())
        , fun (Compiler::compile (
                "x;y",
                //"(* (sin (* x 0.7)) (sin(* y 0.7)) )", 
                /*"(- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{4} persistence{0.54} levelEvaluationFunction{(abs h)}] "
                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                ")))"*/
                "(* 20 (- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.025} layercount{9} persistence{0.54} levelEvaluationFunction{(abs h)}] x y))))",
                functionSet,
                errors))
        {
        }                
};
} // namespace redshift


class HeightFunction : public redshift::HeightFunction {
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
        int const width = 512;
        int const height = width;
        RenderTarget::Ptr renderBuffer (new ColorRenderTarget(width,height));        
        shared_ptr<Camera> camera (new Pinhole(renderBuffer));

        shared_ptr<redshift::HeightFunction> heightFunction;
        try {
                heightFunction = shared_ptr<redshift::HeightFunction> (
                                      new ::redshift::QuatschHeightFunction());
        } catch (...) { // TODO (!!!)
        }
        
        shared_ptr<primitive::Primitive> agg (
                /*new primitive::ClosedSphere(
                        Point(scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(0),
                                scalar_cast<fixed_point_t>(25)),
                        10.0)*/
                new Heightmap (heightFunction, 1.5)
                //new BooleanField (heightFunction, 1.5)
        );

        shared_ptr<background::Preetham> preetham (new background::Preetham());
        preetham->setSunDirection(Vector(2,1,0));
        preetham->setTurbidity(2.0f);
        preetham->setSunColor(redshift::Color(.9,.7,.5)*.1);
        preetham->setColorFilter(redshift::Color(.2,.2,.2));
        preetham->enableFogHack (true, 0.005f, 150000);
        preetham->invalidate();
        
        Scene Scene (
                renderBuffer, 
                camera, 
                agg,
                shared_ptr<Background> (new backgrounds::PreethamAdapter (preetham))
                //shared_ptr<Background>(new backgrounds::Monochrome(Color::fromRgb(1,1,1)))
                //shared_ptr<Background>(new backgrounds::VisualiseDirection())
        );

        RenderTarget::Ptr screenBuffer (new SdlRenderTarget(width,height));
        
        UserCommandProcessor::Ptr commandProcessor (new SdlCommandProcessor());

        ProgressReporter::Ptr reporter (
                  new RenderTargetCopyingReporter(renderBuffer, screenBuffer));        
        
        Scene.render(reporter, commandProcessor);
        copy (renderBuffer, screenBuffer);
        screenBuffer->flip(); 

        while (!commandProcessor->userWantsToQuit())
                commandProcessor->tick();
}

#ifdef PICOGENLIB
int picogen_main ()
#else
int main (int, char*[])
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
        return 0;
}
