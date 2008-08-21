//---------------------------------------------------------------------
//  Algorithmic Conjurings @ http://www.coyotegulch.com
//
//  mtwister.cpp (libcoyotl)
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

#include "mtwister.h"
using namespace libcoyotl;

//--------------------------------------------------------------------------
//  Default constructor, reading seed from/dev/urandom or the time.
mtwister::mtwister()
  : prng()
{
    init_helper();
}

//--------------------------------------------------------------------------
//  Constructor
mtwister::mtwister(uint32_t seed)
  : prng(seed)
{
    init_helper();
}

//--------------------------------------------------------------------------
//  Initializes the generator with "seed"
void mtwister::init(uint32_t seed)
{
    prng::init(seed);
    init_helper();
}
//--------------------------------------------------------------------------
//  Initializes the generator with "seed"
void mtwister::init_helper()
{
    // Save seed for historical purpose
    m_mt[0] = m_seed;

    // Set the seed using values suggested by Matsumoto & Nishimura, using
    //   a generator by Knuth. See original source for details.
    for (m_mti = 1; m_mti < N; ++m_mti)
        m_mt[m_mti] = 1812433253UL * (m_mt[m_mti - 1] ^ (m_mt[m_mti - 1] >> 30)) + m_mti;

}

//--------------------------------------------------------------------------
//   Obtain the next 32-bit integer in the psuedo-random sequence
uint32_t mtwister::get_rand()
{
    // Note: variable names match those in original example
    const uint32_t mag01[2] = { 0, MATRIX_A };
    uint32_t y;
    size_t   kk;

    // Generate N words at a time
    if (m_mti >= N)
    {
#if defined(USE_METATEMP)
        LOOP1<0>::EXEC(m_mt);
        LOOP2<227>::EXEC(m_mt);
#else
        for (kk=0; kk < N-M; kk++)
        {
            y = (m_mt[kk] & UPPER_MASK) | (m_mt[kk+1] & LOWER_MASK);
            m_mt[kk] = m_mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for ( ; kk < N-1; kk++)
        {
            y = (m_mt[kk] & UPPER_MASK) | (m_mt[kk+1] & LOWER_MASK);
            m_mt[kk] = m_mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (m_mt[N-1] & UPPER_MASK) | (m_mt[0]&LOWER_MASK);
        m_mt[N-1] = m_mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
#endif

        m_mti = 0;
    }

    // Here is where we actually calculate the number with a series of transformations
    y = m_mt[m_mti++];

    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}
