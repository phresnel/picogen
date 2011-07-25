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

#ifndef SAMPLING_HH_INCLUDED_20110725
#define SAMPLING_HH_INCLUDED_20110725

#include "random.h"
#include "real.h"
#include <tuple>

namespace picogen { namespace cracker {

        // The R postfix is for "rejection sampled"
        inline std::tuple<real,real> uniformDiskR (Random &rand) {
                real x,y;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                } while (x*x+y*y > 1);
                return std::make_tuple(x,y);
        }
        inline std::tuple<real,real,real> uniformSphereR (Random &rand) {
                real x,y,z;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                        z = 2*rand()-1;
                } while (x*x+y*y+z*z > 1);
                return std::make_tuple(x,y,z);
        }
        inline std::tuple<real,real,real> cosineHemisphereR (Random &rand) {
                using namespace std;
                const tuple<real,real> r2 = uniformDiskR(rand);
                const real x = get<0>(r2);
                const real z = get<1>(r2);
                return make_tuple(x, sqrt(1-x*x-z*z), z);
        }
} }

#endif // SAMPLING_HH_INCLUDED_20110725
