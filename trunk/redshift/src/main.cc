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

#ifdef __MINGW32__
 #include <SDL/SDL.h>
#else
 #include <SDL.h>
#endif
#include <omp.h>

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
        : source (src), target (target_)
        {}



        void report (RenderTarget::ReadLockPtr sourcel,
                                              int completed, int total) const {
                if (lastTime() < 0.5)
                        return;
                lastTime.restart();

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

        mutable StopWatch lastTime;
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
#include "../../quatsch/configurable-functions/noise2ddef.hh"
#include "../../quatsch/configurable-functions/layerednoise2ddef.hh"

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
                return (*fun) (p);
        }


        QuatschHeightFunction (const std::string code)
        : functionSet(addfuns())
        , fun (Compiler::compile (
                "x;y",
                //"(* 0.5 (* (sin (* x 2.0)) (sin(* y 2.0)) ))",
                /*"(- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{4} persistence{0.54} levelEvaluationFunction{(abs h)}] "
                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                ")))",*/
                code,
                //"(* x 0.1)",
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

        redshift::StopWatch stopWatch;

        // TODO replace RenderTarget with Film?
        //    i mean, a "RenderTarget" might be flipable, but a Film not, or so
        int const width = 1680*2;
        int const height = width/3;

        const real_t Y = 0;
        RenderTarget::Ptr renderBuffer (new ColorRenderTarget(width,height));
        shared_ptr<Camera> camera (new Pinhole(renderBuffer, vector_cast<Point>(Vector(-320,100,-4380))));

        shared_ptr<redshift::HeightFunction> heightFunction;
        shared_ptr<redshift::HeightFunction> distortHeightFunction;
        try {
                heightFunction = shared_ptr<redshift::HeightFunction> (
                        new ::redshift::QuatschHeightFunction(
                                //"(+ -150 (* 500 (^ (- 1 (abs ([LayeredNoise2d filter{cosine} seed{13} frequency{0.001} layercount{8} persistence{0.45} levelEvaluationFunction{(abs h)}] x y))) 2 )))"
                /* benchmark */
//"(* 800 ([LayeredNoise2d filter{cosine} seed{13} frequency{0.0005} layercount{12} persistence{0.45} levelEvaluationFunction{(abs h)}] x y))"
// dA: "(* 2400 ([LayeredNoise2d filter{cosine} seed{54} frequency{0.0005} layercount{14} persistence{0.5}] x y))"
"(+ (* 3 ([LayeredNoise2d filter{cosine} seed{54} frequency{0.15} layercount{9} persistence{0.5}] x y)) (* 700 ([LayeredNoise2d filter{cosine} seed{54} frequency{0.0015} layercount{7} persistence{0.4}] x y)))"

//"(+ -1100 (* 2200 (- 1 (abs ([LayeredNoise2d filter{cosine} seed{4} frequency{0.00025} layercount{8} persistence{0.5} levelEvaluationFunction{(abs h)}] (+ 100000 x) (+ 100000 y))))))"
//                "(* 3 (sin (* 0.01 x)) (sin (* 0.01 y)))"
                ));
                distortHeightFunction = shared_ptr<redshift::HeightFunction> (
                        new ::redshift::QuatschHeightFunction(
                                "(* 0.05 ([LayeredNoise2d filter{cosine} seed{13} frequency{0.02} layercount{10} persistence{0.63}] x y))"
                ));
                /*for (int i=0; i<50; ++i) {
                        std::cout << (*distortHeightFunction)(rand()/(RAND_MAX+1.f),rand()/(RAND_MAX+1.f)) << std::endl;
                }*/
        } catch (...) { // TODO (!!!)
        }

        primitive::List *list = new List;
        /*list->add (shared_ptr<primitive::Primitive> (new ClosedSphere (vector_cast<Point>(PointF(50,15+Y,-5420)), 10)));
        list->add (shared_ptr<primitive::Primitive> (new ClosedSphere (vector_cast<Point>(PointF(50,35+Y,-5420)), 10)));
        list->add (shared_ptr<primitive::Primitive> (new ClosedSphere (vector_cast<Point>(PointF(50,55+Y,-5420)), 10)));
        list->add (shared_ptr<primitive::Primitive> (new ClosedSphere (vector_cast<Point>(PointF(50,75+Y,-5420)), 10)));
        list->add (shared_ptr<primitive::Primitive> (new ClosedSphere (vector_cast<Point>(PointF(50,95+Y,-5420)), 10)));*/
        list->add (shared_ptr<primitive::Primitive> (new LazyQuadtree (heightFunction, 10000, distortHeightFunction)));
        list->add (shared_ptr<primitive::Primitive> (new HorizonPlane (-50, distortHeightFunction)));
        shared_ptr<primitive::Primitive> agg (list);

        shared_ptr<background::Preetham> preetham (new background::Preetham());
        preetham->setSunDirection(Vector(0,1,3));
        preetham->setTurbidity(2.9f);
        preetham->setSunColor(redshift::Color(2.0,1.0,0.5)*2.3);
        preetham->setColorFilter(redshift::Color(1.3,1.0,0.8)*0.04);
        preetham->enableFogHack (true, 0.000125f, 150000);
        preetham->invalidate();

        Scene Scene (
                renderBuffer,
                camera,
                agg,
                shared_ptr<Background> (new backgrounds::PreethamAdapter (preetham)),
                //shared_ptr<Background>(new backgrounds::Monochrome(Color::fromRgb(1,1,1)))
                //shared_ptr<Background>(new backgrounds::VisualiseDirection())
                shared_ptr<Integrator> (new DirectLighting()) //*/ shared_ptr<Integrator> (new ShowSurfaceNormals())
        );

        RenderTarget::Ptr screenBuffer (new SdlRenderTarget(width,height));

        UserCommandProcessor::Ptr commandProcessor (new SdlCommandProcessor());

        ProgressReporter::Ptr reporter (
                  new RenderTargetCopyingReporter(renderBuffer, screenBuffer));

        Scene.render(reporter, commandProcessor);
        copy (renderBuffer, screenBuffer);
        screenBuffer->flip();

        stopWatch.stop();
        std::stringstream ss;
        ss << "pico:t=" << stopWatch() << "sec/" << (stopWatch()/60.f) << "min";
        SDL_WM_SetCaption(ss.str().c_str(), ss.str().c_str());

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
