//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2010  Sebastian Mach
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

#ifndef LIBNOISE_SUPPORT_DEF_HH_INCLUDED_20100427
#define LIBNOISE_SUPPORT_DEF_HH_INCLUDED_20100427

#include <string>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "libnoise-support.hh"

namespace quatsch {  namespace configurable_functions {

template <typename FUNCTION, typename COMPILER>
LibnoisePerlin <FUNCTION, COMPILER> :: LibnoisePerlin (
        ::std::map<std::string,std::string>& static_parameters,
        ::std::vector <FunctionPtr>& runtime_parameters
) {

        {
                using namespace quatsch::backend::est;
                if (runtime_parameters.size() < 2)
                        throw insufficient_number_of_operands_exception (2);
                if (runtime_parameters.size() > 2)
                        throw too_many_operands_exception (2);

                ufun = runtime_parameters[0];
                vfun = runtime_parameters[1];
        }


        using namespace std;
        typedef map<string,string> Map;

        //====---- - - -  -   -    -      -
        // Scan Parameters.
        //====---- - - -  -   -    -      -
        string nonExistantParameterNames (""); // To collect together parameter names that don't exist, for dumping errors in the end.


        for (Map::const_iterator it=static_parameters.begin();
             it!=static_parameters.end();
             ++it
        ) {
                const string name = it->first;
                /// \todo Add some shorter Mnenomics.
                if (name == string("frequency")) {
                        istringstream hmmm (static_parameters[name]);
                        double tmp; hmmm >> tmp; perlin.SetFrequency(tmp);
                } else if (name == string("lacunarity")) {
                        istringstream hmmm (static_parameters[name]);
                        double tmp; hmmm >> tmp; perlin.SetLacunarity(tmp);
                } else if (name == string("octave-count")) {
                        istringstream hmmm (static_parameters[name]);
                        double tmp; hmmm >> tmp; perlin.SetFrequency(tmp);
                } else if (name == string("persistence")) {
                        istringstream hmmm (static_parameters[name]);
                        double tmp; hmmm >> tmp; perlin.SetPersistence(tmp);
                } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                }
        }

        // Any parameters set that we don't know?
        if (nonExistantParameterNames != "") {
                throw general_exception ("the following parameters do not exist for 'LayeredNoise': "+nonExistantParameterNames);
        }
}


template <typename FUNCTION, typename COMPILER>
LibnoisePerlin <FUNCTION, COMPILER> :: ~LibnoisePerlin() {
}




template <typename FUNCTION, typename COMPILER>
typename LibnoisePerlin <FUNCTION, COMPILER>::scalar_t
LibnoisePerlin <FUNCTION, COMPILER>::operator () (
        const typename LibnoisePerlin <FUNCTION, COMPILER>::parameters_t& parameters
) const {
        const scalar_t u = (*ufun) (parameters);
        const scalar_t v = (*vfun) (parameters);
        return perlin.GetValue(u, v, 0.5);
}

} }

#endif // LIBNOISE_SUPPORT_DEF_HH_INCLUDED_20100427
