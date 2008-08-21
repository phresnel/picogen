//---------------------------------------------------------------------
//  Algorithmic Conjurings @ http://www.coyotegulch.com
//
//  prng.h (libcoyotl)
//
//  A Framework for Pseudorandom Number Generators
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

#include "prng.h"
using namespace libcoyotl;

#include <unistd.h>

//---------------------------------------------------------------------------
//  Default constructor, reading seed from/dev/urandom or the time.
prng::prng()
  : m_seed(0)
{
    // first try to read /dev/urandom
	int fd = open ("/dev/urandom", O_RDONLY);

    if (fd != -1)
    {
        read(fd, &m_seed, 4);
        close(fd);
    }
    else
    {
        // the following is *NOT* cryptographically-secure
        m_seed = uint32_t(time(NULL));
    }
}

//---------------------------------------------------------------------------
//  Default constructor, with optional seed.
prng::prng(uint32_t seed)
  : m_seed(seed)
{
    // nada
}

//---------------------------------------------------------------------------
//  Initializes the generator with "seed"
void prng::init(uint32_t seed)
{
    m_seed = seed;
}


//--------------------------------------------------------------------------
//   Obtain a psuedorandom real number in the range [0,1), i.e., a number
//   greater than or equal to 0 and less than 1, with 53-bit precision.
double prng::get_rand_real53()
{
    // privides a granularity of approx. 1.1E-16
    uint32_t a = get_rand() >> 5;
    uint32_t b = get_rand() >> 6;
    return double(a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}
