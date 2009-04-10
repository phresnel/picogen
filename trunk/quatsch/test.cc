//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2009  Sebastian Mach
//
//      email: a@b.c, with a=phresnel, b=gmail, c=com
//        www: http://phresnel.org
//             http://picogen.org
//
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the  Free  Software  Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed  in the hope that it will be useful, but
//    WITHOUT  ANY  WARRANTY;   without   even  the  implied  warranty  of
//    MERCHANTABILITY  or  FITNESS FOR A PARTICULAR  PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy  of  the  GNU General Public License
//    along with this program. If not, see <http://www.gnu.org/licenses/>.


#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "quatsch.hh"
#include "frontend/jux.hh"
#include "backend/est/backend.hh"

//#include "backend/est/backenddef.hh"

#include "configurable-functions/noise2ddef.hh"
//template class quatsch::backend::est::Backend <double, const double *> ;

// names: Quatsch, Witz, Jux

// TODO: support non-float types (e.g. in "::pow (x)")
// TODO: check function signatures
// TODO: make (lerp a b f) -> (lerp f x_1 x_2 ... x_n)
// TODO: clean error messages



int main () {   
    
        typedef quatsch::backend::est::Backend <double, const double *> backend_t;

        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;

        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;    

        //::std::vector <::boost::shared_ptr <ICreateAdditionFunction <Function> > > configurableFunctions;
        //configurableFunctions.push_back 


        quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                new quatsch::CreateConfigurableFunction <
                        quatsch :: configurable_functions :: Noise2d <Function, Compiler>,  
                        Function
                >
        );
    
   
        ::std::string parameters = "alpha";
        ::std::string code = //"/* faculty function. */\n"
                //"// functions \n"
                //"//(defun pi () (pi))\n"
                //"(defun fac (x) (if (< x 2) 1 (* x (fac (- x 1)))))\n"
                //"(defun lerp (x) (if (< x 2) 1 (* x (lerp (- x 1)))))\n"
                //"(defun $ (x) (if (< x 2) 1 (* x (fac (- x 1))))) /* should throw */\n" // << TODO: implement functors for error messaging
                //"(defun something-awful x y z (+ x y z)) \n"
                //"// program\n"
                //"([Noise2d outer{outer}] ([Noise2d inner{inner}] alpha alpha) alpha)"
                "(lerp 0 1 0.25)"
                //"([Noise2d foo() boofar(0.5) ] alpha alpha)"
                //"([Noise2d foo() boofar((+ 0.5 0.1)) ] alpha alpha)"
        ;
        //::std::string code = "(def x a (+ a a)) (+ (x 5) 1)";

        Compiler::ConfigurableFunctionsMap addfuns;
        addfuns.addSymbol ("Noise2d", noiseDesc);

        Compiler::FunctionPtr fun (Compiler::compile (parameters, code, addfuns, std::cerr));
        //exit(41);
        /*::std::vector <scalar_t> params;
        params.push_back (6.0);*/
        scalar_t params [] = { 6.0 };
        ::std::cout << "r:" << (*fun) (params) << ::std::endl;
    
        while (true) {
        
                std::stringstream errors;
                try {
                        char s [1024];
                        gets (s);        
                
                        Compiler::FunctionPtr fun (Compiler::compile (parameters, s, addfuns, errors));
                
                        scalar_t r = (*fun) (params);
                        ::std::cout << "r:" << r << ::std::endl;
                        //compiler ("alpha;beta;gamma").compile (s);
                } catch (std::exception const & e) {
                        ::std::cout << "general exception caught: \n"
                                    << errors.str()
                                    << e.what()                                    
                                    << '\n';
                }/* catch (...) {
                        ::std::cout << "caught unknown exception\n";
                }*/
    };
    
}
