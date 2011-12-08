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

#ifndef QUATSCH_HEIGHT_FUNCTION_HH_INCLUDED_20111208
#define QUATSCH_HEIGHT_FUNCTION_HH_INCLUDED_20111208

/*
#include "height-function.hh"
#include "distribution-function.hh"
*/
#include <functional>

#include "crystal_math.h"

#include "quatsch/quatsch.hh"
#include "quatsch/frontend/jux.hh"
#include "quatsch/backend/est/backend.hh"

#include "quatsch/configurable-functions/noise2ddef.hh"
#include "quatsch/configurable-functions/layerednoise2ddef.hh"
#include "quatsch/configurable-functions/heightmapdef.hh"
#include "quatsch/configurable-functions/libnoise-support-def.hh"

namespace crystal { namespace quatsch {

class height_function {
private:
        // quatsch
        typedef ::quatsch::backend::est::Backend <real,
                                                  const real *> backend_t;
        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;
        typedef ::quatsch::frontend::jux::Compiler <backend_t> Compiler;
        typedef Compiler::ConfigurableFunctionsMap FunctionSet;

        static Compiler::ConfigurableFunctionsMap addfuns() {
                //using namespace redshift;
                using namespace ::quatsch;


                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: Noise2d <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr layeredNoise2dDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LayeredNoise2d <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr heightmapDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: Heightmap <Function, Compiler>,
                                Function
                        >
                );

                // libnoise
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoisePerlinDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoisePerlin <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseBillowDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseBillow <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseCylindersDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseCylinders <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseRidgedMultiDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseRidgedMulti <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseSpheresDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseSpheres <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseVoronoiDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseVoronoi <Function, Compiler>,
                                Function
                        >
                );
                ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr libnoiseTurbulenceDesc (
                        new CreateConfigurableFunction <
                                configurable_functions :: LibnoiseTurbulence <Function, Compiler>,
                                Function
                        >
                );

                Compiler::ConfigurableFunctionsMap addfuns;
                addfuns.addSymbol ("Noise2d", noiseDesc);
                addfuns.addSymbol ("ValueNoise2d", layeredNoise2dDesc);
                addfuns.addSymbol ("Heightmap", heightmapDesc);

                // libnoise integration
                addfuns.addSymbol ("LibnoisePerlin", libnoisePerlinDesc);
                addfuns.addSymbol ("LibnoiseBillow", libnoiseBillowDesc);
                addfuns.addSymbol ("LibnoiseCylinders", libnoiseCylindersDesc);
                addfuns.addSymbol ("LibnoiseRidgedMulti", libnoiseRidgedMultiDesc);
                addfuns.addSymbol ("LibnoiseSpheres", libnoiseSpheresDesc);
                addfuns.addSymbol ("LibnoiseVoronoi", libnoiseVoronoiDesc);
                //addfuns.addSymbol ("LibnoiseTurbulence", libnoiseTurbulenceDesc); //TODO: requires a source module (i.e. we should write a libnoise-module which accepts quatsch-code)

                // deprecated
                //addfuns.addSymbol ("LayeredNoise2d", layeredNoise2dDesc);
                return addfuns;
        }

        FunctionSet functionSet;
        Compiler::FunctionPtr fun;

public:
        /*typename BASE::return_type operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                const real_t p [] = { u, v };
                return typename BASE::return_type((*fun) (p));
        }*/

        real operator() (real x, real z) const
        {
                const real p [] = { x, z };
                return (*fun) (p);
        }


        height_function (const std::string code)
        : functionSet(addfuns())
        {
                std::stringstream errors;
                fun = Compiler::compile (
                        "x;y",
                        //"(* 0.5 (* (sin (* x 2.0)) (sin(* y 2.0)) ))",
                        /*"(- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{4} persistence{0.54} levelEvaluationFunction{(abs h)}] "
                        "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                        "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                        ")))",*/
                        code,
                        //"(* x 0.1)",
                        functionSet,
                        errors);
        }

        height_function (const std::string code, std::stringstream &errors)
        : functionSet(addfuns())
        {
                fun = Compiler::compile (
                        "x;y",
                        //"(* 0.5 (* (sin (* x 2.0)) (sin(* y 2.0)) ))",
                        /*"(- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{4} persistence{0.54} levelEvaluationFunction{(abs h)}] "
                        "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                        "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                        ")))",*/
                        code,
                        //"(* x 0.1)",
                        functionSet,
                        errors);
        }
};

typedef height_function QuatschHeightFunction;
typedef height_function QuatschDistributionFunction;

} }

#endif // QUATSCH_HEIGHT_FUNCTION_HH_INCLUDED_20111208
