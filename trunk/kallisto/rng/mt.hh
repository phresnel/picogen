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


#ifndef RANDOM_HH_INCLUDED_20090725
#define RANDOM_HH_INCLUDED_20090725

#include <boost/random.hpp>

namespace kallisto { namespace random {
        


// as name says
template <typename T> struct make_int_if_float {
        typedef T type;
};
template <> struct make_int_if_float<float> {
        typedef int32_t type;
};
template <> struct make_int_if_float<double> {
        typedef int64_t type;
};



STATIC_ASSERT((kallisto::traits::types_equal<make_int_if_float<float>::type,int32_t>::value));
STATIC_ASSERT((kallisto::traits::types_equal<make_int_if_float<double>::type,int64_t>::value));
STATIC_ASSERT((kallisto::traits::types_equal<make_int_if_float<int>::type,int>::value));
STATIC_ASSERT(!(kallisto::traits::types_equal<make_int_if_float<int>::type,char>::value));
STATIC_ASSERT((sizeof(make_int_if_float<float>::type)==sizeof(int32_t)));
STATIC_ASSERT((sizeof(make_int_if_float<double>::type)==sizeof(int64_t)));



// wrappers
// note: mt19937 is typedefed as mersenne_twister<uint32_t,...>, hence we can
//       only use uint32 as the seed at the moment
template <
        typename T, 
        typename make_int_if_float<T>::type min,
        typename make_int_if_float<T>::type max,
        bool isFloat=kallisto::traits::is_float<T>::value
> class MersenneTwister;

template <typename T, T min, T max>
class MersenneTwister<T,min,max,false> {
public:
        STATIC_ASSERT((int_cmp_le<T, traits::integer_limits<T>::min, min>::value));
        STATIC_ASSERT((int_cmp_le<T, max, traits::integer_limits<T>::max>::value));
        STATIC_ASSERT((int_cmp_le<T, min, max>::value));

        MersenneTwister (uint32_t seed)
        : rng(seed)
        , dist (min, max)
        , rand_ (rng, dist) {
        }
        
        MersenneTwister ()
        : rng()
        , dist (min, max)
        , rand_ (rng, dist) {
        }
        
        void seed (uint32_t seed) {
                rng.seed (seed);
        }
        
        T rand () {
                return rand_();
        }
        
        operator T () {
                return rand();
        }

private:        
        boost::mt19937 rng;
        typedef boost::uniform_int<T> dist_t;

        dist_t dist;
        boost::variate_generator<
                boost::mt19937&, 
                dist_t
        > rand_;
};


template <
        typename T, 
        typename make_int_if_float<T>::type min,
        typename make_int_if_float<T>::type max
>
class MersenneTwister<T,min,max,true> {        
        typedef T float_t;

        // boost.random seems to have problems with uint64_t ...
        //typedef typename 
        //  kallisto::traits::equal_sized_uint<float_t>::type uint32_t uint_t;
        //typedef typename make_int_if_float<T>::type uint_t;
        typedef typename make_int_if_float<T>::type uint_t;
        typedef typename 
          kallisto::traits::bigger_float<float_t>::type bigger_float_t;        
        
public:

        STATIC_ASSERT(min <= max);

        MersenneTwister (uint32_t seed)
        : rng(seed)
        , maxf(std::numeric_limits<uint_t>::max())
        , dist (0, maxf)
        , rand_ (rng, dist) {
        }
        MersenneTwister ()
        : rng()
        , maxf(std::numeric_limits<uint_t>::max())
        , dist (0, maxf)
        , rand_ (rng, dist) {
        }
        
        void seed (uint_t seed) {
                rng.seed (seed);
        }
        
        float_t rand () {
                return static_cast<T>(min)
                        + (rand_() / maxf)
                        * static_cast<T>(max-min);
        }
        
        operator float_t () {
                return rand();
        }

private:
        boost::mt19937 rng;
        const bigger_float_t maxf;
        typedef boost::uniform_int<uint_t> dist_t;

        dist_t dist;
        boost::variate_generator<
                boost::mt19937&, 
                dist_t
        > rand_;
};

} }

#endif // RANDOM_HH_INCLUDED_20090725
