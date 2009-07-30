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
        
        namespace traits {
                using std::numeric_limits;
        }
        

        // Integer comparison that takes care of gcc's warning ...
        // "warning: comparison of unsigned expression >= 0 is always true"
        // ... when compiled with -Wextra
        template <
                typename T,
                T min,
                T max,
                bool min_zero = min == 0,
                bool is_signed = traits::numeric_limits<T>::is_signed
        > struct int_cmp_le;
        template <typename T, T min, bool min_zero, T max>
         struct int_cmp_le <T, min, max, min_zero, true> {
                enum { value = min <= max };
        };
        template <typename T, T min, T max> 
         struct int_cmp_le <T, min, max, false, false> {
                enum { value = min <= max };
        };
        template <typename T, T min, T max> 
         struct int_cmp_le <T, min, max, true, false> {
                enum { value = true };
        };




        // Template friendly sqrt().
        inline float       sqrt (float const &rhs)       {return sqrtf (rhs);}
        inline double      sqrt (double const &rhs)      {return sqrt (rhs); }
        inline long double sqrt (long double const &rhs) {return sqrtl (rhs);}

        // Template friendly abs().
        inline int           abs (int const &rhs)          {return abs (rhs); }
        inline long int      abs (long int const &rhs)     {return labs(rhs); }
#if __STDC_VERSION__ >= 199901L
        inline long long int abs (long long int const &rhs){return llabs(rhs);}
#endif
        inline float         abs (float const &rhs)        {return fabsf(rhs);}
        inline double        abs (double const &rhs)       {return fabs(rhs); }
        inline long double   abs (long double const &rhs)  {return fabsl(rhs);}
        
        
        
        template <typename T> inline void swap (T &lhs, T &rhs) {
                const T tmp = lhs;
                lhs = rhs;
                rhs = tmp;
        }
        
        template <typename T> inline T min (T lhs, T rhs) {
                return lhs < rhs ? lhs : rhs;
        }
        
        template <typename T> inline T max (T lhs, T rhs) {
                return lhs > rhs ? lhs : rhs;
        }
}

#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>

namespace kallisto {
        using boost::optional;
        
        using boost::tuple;
        using boost::get;
        using boost::make_tuple;
}

#endif // KALLISTO_HH_INCLUDED_20090218
