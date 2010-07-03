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

#ifndef SAMPLING_HH_INCLUDED_20100701
#define SAMPLING_HH_INCLUDED_20100701

#include "geometry.hh"
#include "random.hh"
#include "constants.hh"
#include "tuple.hh"

namespace redshift {

        // The R postfix is for "rejection sampled"
        inline tuple<real_t,real_t> uniformDiskR (Random &rand) {
                real_t x,y;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                } while (x*x+y*y > 1);
                return make_tuple(x,y);
        }
        inline tuple<real_t,real_t,real_t> uniformSphereR (Random &rand) {
                real_t x,y,z;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                        z = 2*rand()-1;
                } while (x*x+y*y+z*z > 1);
                return make_tuple(x,y,z);
        }
        inline tuple<real_t,real_t,real_t> cosineHemisphereR (Random &rand) {
                const tuple<real_t,real_t> r2 = uniformDiskR(rand);
                const real_t x = get<0>(r2);
                const real_t z = get<1>(r2);
                return make_tuple(x, std::sqrt(1-x*x-z*z), z);
        }
}

#endif // SAMPLING_HH_INCLUDED_20100701
