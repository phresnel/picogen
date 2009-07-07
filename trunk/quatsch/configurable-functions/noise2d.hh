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

#include <map>
#include <vector>
#include <string>

#include <boost/shared_array.hpp>
//#include <picogen/picogen.h>

namespace quatsch {  namespace configurable_functions {
            
        template <typename FUNCTION, typename COMPILER>
        class Noise2d : public FUNCTION {
        private:
                typedef FUNCTION function_t;            
                typedef typename function_t::FunctionPtr  FunctionPtr;
                typedef typename function_t::scalar_t     scalar_t;
                typedef typename function_t::parameters_t parameters_t;

                typedef COMPILER compiler_t;

                FunctionPtr ufun;
                FunctionPtr vfun;

                enum filter_t {
                nearest,
                bilinear,
                cosine
                };
                filter_t filter;

                //unsigned int noiseDepth;
                boost::shared_array <unsigned int> offsetLut;
                boost::shared_array <scalar_t>     rngLut;
                unsigned int offsetLutMask;
                unsigned int offsetLutSize;
                scalar_t frequency;

                scalar_t operator () (scalar_t x, scalar_t y, 
                        scalar_t domainScale, scalar_t rangeScale, 
                                                 unsigned int depth ) const;

        public:
            
                static ::std::string const & name () {
                        static const ::std::string name ("Noise2d"); 
                        return name;
                }
            
                static unsigned int parameterCount () {
                        return 2;
                }

                Noise2d (::std::map<std::string,std::string> &parameters, 
                                        FunctionPtr ufun, FunctionPtr vfun);

                Noise2d (::std::map<std::string,std::string>&static_parameters,
                              ::std::vector <FunctionPtr> runtime_parameters
                ) {
                        
                        ::std::cout << Noise2d::name ()
                                    << " created with "
                                    << runtime_parameters.size() 
                                    << " arguments."
                                    << std::endl;

                        typename compiler_t::ConfigurableFunctionsMap addfuns;
                        if (static_parameters ["inner"] == "inner")
                                ::std::cout << "!!inner" << ::std::endl;
                        if (static_parameters ["outer"] == "outer")
                        ::std::cout << "!!outer" << ::std::endl;
                }
            
                virtual ~Noise2d();
                virtual scalar_t operator () (const parameters_t &) const;
        };
} }
