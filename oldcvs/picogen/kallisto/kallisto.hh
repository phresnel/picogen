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

#ifndef KALLISTO_HH_INCLUDED_20090218
#define KALLISTO_HH_INCLUDED_20090218

//#include "common.hh"

namespace kallisto {
        enum coordinate_space_t {
                CARTESIAN,
                SPHERICAL
        };


        // Template friendly sqrt.
        inline float       sqrt (float const &rhs)       {return sqrtf (rhs);}
        inline double      sqrt (double const &rhs)      {return sqrt (rhs); }
        inline long double sqrt (long double const &rhs) {return sqrtl (rhs);}


        /*template <typename T> T sqrt (T const &v) {
                return static_cast<T> (sqrt (static_cast<double>(v)));
        }
        template <> float sqrt<float> (float const &v) {
                return sqrt (v);
        }*/
}

#endif // KALLISTO_HH_INCLUDED_20090218
