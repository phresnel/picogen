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

#ifndef LIBNOISE_SUPPORT_HH_INCLUDED_20100427
#define LIBNOISE_SUPPORT_HH_INCLUDED_20100427

#include <boost/shared_array.hpp>
//#include <picogen/picogen.h>
#include "../../kallisto/common.hh"

#include "../quatsch.hh"
#include "../frontend/jux.hh"
#include "../backend/est/backend.hh"

#include <libnoise/noise.h>

#include <map>

namespace quatsch {  namespace configurable_functions {

        template <typename FUNCTION, typename COMPILER>
        class LibnoisePerlin : public FUNCTION {
        private:
                typedef FUNCTION function_t;
                typedef typename function_t::FunctionPtr  FunctionPtr;
                typedef typename function_t::scalar_t     scalar_t;
                typedef typename function_t::parameters_t parameters_t;

                typedef COMPILER compiler_t;

                FunctionPtr ufun;
                FunctionPtr vfun;

                noise::module::Perlin perlin;

        public:
                LibnoisePerlin (
                        ::std::map<std::string,std::string>&static_parameters,
                        ::std::vector <FunctionPtr> &runtime_parameters
                );

                static ::std::string const & name () {
                        static const ::std::string name ("LibnoisePerlin");
                        return name;
                }

                static unsigned int parameterCount () {
                        return 2;
                }

                virtual ~LibnoisePerlin();
                virtual scalar_t operator () (const parameters_t &) const;

        };
} }


#endif // LIBNOISE_SUPPORT_HH_INCLUDED_20100427
