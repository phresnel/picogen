//---------------------------------------------------------------------
//  Algorithmic Conjurings @ http://www.coyotegulch.com
//
//  mtwister.h (libcoyotl)
//
//  Mersenne Twister -- A pseudorandom Number Generator
//
//  ORIGINAL ALGORITHM COPYRIGHT
//  ============================
//  Copyright (C) 1997, 2002 Makoto Matsumoto and Takuji Nishimura.
//  Any feedback is very welcome. For any question, comments, see
//  http://www.math.keio.ac.jp/matumoto/emt.html or email
//  matumoto@math.keio.ac.jp
//---------------------------------------------------------------------
//
//  Copyright 1990-2004 Scott Robert Ladd
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the
//      Free Software Foundation, Inc.
//      59 Temple Place - Suite 330
//      Boston, MA 02111-1307, USA.
//
//-----------------------------------------------------------------------
//
//  For more information on this software package, please visit
//  Scott's web site, Coyote Gulch Productions, at:
//
//      http://www.coyotegulch.com
//
//-----------------------------------------------------------------------

#if !defined(LIBCOYOTL_MTWISTER_H)
#define LIBCOYOTL_MTWISTER_H

// note by Sebastian Mach: with gcc 4.3.1, USE_METATEMP can yield up to
// 10% faster code that simple benchmark (gcc -O3, for some reasons
// with -O0 it gets slower, ..., hmmm):
//
//
// template <int C, class T> void speedTest (T &fun) {
//     uint32_t t = 0;
//     for (unsigned int u=0; u<C; ++u) {
//         t += fun();
//     }
//     cout << "<ignore this message, it is only here so that nothing is optimized away " << t << ">";
// }
#define USE_METATEMP

#include "prng.h"

namespace libcoyotl
{
    //! Implements the Mersenne Twister, a peudorandom number generator
    /*!
        The mtwister class encapsulates the Mersenne Twister algorithm invented by
        Makoto Matsumoto and Takuji Nishimura. One of the appealing aspects of the
        Mersenne Twister is its use of binary operations (as opposed to
        time-consuming multiplication) for generating numbers. The algorithm's
        period is 2<sup>19937</sup>-1 (~10<sup>6001</sup>), as compared to a
        period of ~10<sup>8</sup> for the best variants of the linear congruential
        methods.

    */
    template <int i> class LOOP1;

    class mtwister : public prng
    {
        friend class LOOP1<0>;

    public:
        // Period parameters
        static const size_t N = 624;
        static const size_t M = 397;

        static const uint32_t MATRIX_A   = 0x9908b0dfUL;
        static const uint32_t UPPER_MASK = 0x80000000UL;
        static const uint32_t LOWER_MASK = 0x7fffffffUL;

    private:
        // Working storage
        uint32_t m_mt[N];
        size_t   m_mti;
        uint32_t m_multiplier;

    public:
        //! Default constructor, reading seed from/dev/urandom or the time.
        /*!
            The constructor initializes the prng seed from either the time
            or some stochastic source such as /dev/random or /dev/urandom.
        */
        mtwister();

        //! Default constructor, with optional seed.
        /*!
            The constructor uses an explicit value for the seed.
            \param seed - Seed value used to "start" or seed the generator
        */
        mtwister(uint32_t seed);

        //! Initializes the generator with "seed"
        /*!
            Resets the generator using the provided seed value.
            \param seed - Seed value used to "start" or seed the generator
        */
        virtual void init(uint32_t seed);

    private:
        //! Initializes the generator with "seed"
        /*!
            Initializes internal tables based on the current seed value.
            \param seed - Seed value used to "start" or seed the generator
        */
        void init_helper();

    public:
        //!  Get the next integer
        /*!
            Returns the next uint32_t in sequence.
            \return A pseudorandom uint32_t value
        */
        uint32_t get_rand();
    };

#if defined(USE_METATEMP)
    template <int i>
    class LOOP1
    {
    public:
        inline static void EXEC(uint32_t * a)
        {
            uint32_t y = (a[i] & mtwister::UPPER_MASK) | (a[i+1] & mtwister::LOWER_MASK);
            a[i] = a[i + 397] ^ (y >> 1) ^ ((y & 1) ? mtwister::MATRIX_A : 0);
            LOOP1<i+1>::EXEC(a);
        }
    };

    template<>
    class LOOP1<226>
    {
    public:
        inline static void EXEC(uint32_t * a)
        {
            uint32_t y = (a[226] & mtwister::UPPER_MASK) | (a[227] & mtwister::LOWER_MASK);
            a[226] = a[623] ^ (y >> 1) ^ ((y & 1) ? mtwister::MATRIX_A : 0);
        }
    };

    template <int i>
    class LOOP2
    {
    public:
        inline static void EXEC(uint32_t * a)
        {
            uint32_t y = (a[i] & mtwister::UPPER_MASK) | (a[i+1] & mtwister::LOWER_MASK);
            a[i] = a[i - 227] ^ (y >> 1) ^ ((y & 1) ? mtwister::MATRIX_A : 0);
            LOOP2<i+1>::EXEC(a);
        }
    };

    template<>
    class LOOP2<623>
    {
    public:
        inline static void EXEC(uint32_t * a)
        {
            uint32_t y = (a[623] & mtwister::UPPER_MASK) | (a[0] & mtwister::LOWER_MASK);
            a[623] = a[396] ^ (y >> 1) ^ ((y & 1) ? mtwister::MATRIX_A : 0);
        }
    };

#endif

} // end namespace libcoyotl

#endif
