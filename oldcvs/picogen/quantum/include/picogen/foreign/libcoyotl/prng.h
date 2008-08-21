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

#if !defined(LIBCOYOTL_PRNG_H)
#define LIBCOYOTL_PRNG_H

// Standard C Library
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <fcntl.h>
#include <stdint.h>

namespace libcoyotl
{
    //! An abstract definition of a peudorandom number generator
    /*!
        This class defines the generic properties of any psuedo-random number
        generator that returns a 32-bit unsigned result.
    */
    class prng
    {
    protected:
        // Working storage
        uint32_t m_seed;

    public:
        //! Default constructor, reading seed from/dev/urandom or the time.
        /*!
            The constructor initializes the prng seed from either the time
            or some stochastic source such as /dev/random or /dev/urandom.
        */
        prng();

        //! Default constructor, with optional seed.
        /*!
            Starts the PRNG with a specific seed; this is provided so that
            a given sequence of "random" values can be duplicated. Note that
            the actual sequence is predicated on the specific PRNG algorithm. 
            \param seed - Seed value used to "start" or seed the generator
        */
        prng(uint32_t seed);

        //! Initializes the generator with "seed"
        /*!
            Resets the generator using the provided seed value.
            \param seed - Seed value used to "start" or seed the generator
        */
        virtual void init(uint32_t seed);

        //! Returns the original seed value
        /*!
            Returns the seed value used to initialize this generator.
            \return The seed value used to initialize this generator
        */
        uint32_t get_seed();
        
        //!  Get the next integer
        /*!
            Returns the next uint32_t in sequence.
            \return A pseudorandom uint32_t value
        */
        virtual uint32_t get_rand() = 0;

        //! Get the next integer in the range [lo,hi]
        /*!
            Returns the next int_value between lo and hi, inclusive.
            \param lo - Minimum value of result
            \param hi - Maximum value of result
            \return A pseudorandom uint32_t value
        */
        uint32_t get_rand_range(uint32_t lo, uint32_t hi);

        //! Get the next random value as a size_t index
        /*!
            Returns the next value as a size_t "index" in the range [0,length).
            \param length - Maximum value of result
            \return A pseudorandom size_t value
        */
        size_t get_rand_index(size_t length);

        //! Get the next number in the range [0,1]
        /*!
            Returns the next real number in the range [0,1], i.e., a number
            greater than or equal to 0 and less than or equal to 1.
            Provides 32-bit precision.
            \return A pseudorandom double value
        */
        double get_rand_real1();
    
        //! Get the next number in the range [0,1)
        /*!
            Returns the next real number in the range [0,1), i.e., a number
            greater than or equal to 0 and less than 1.
            Provides 32-bit precision.
            \return A pseudorandom double value
        */
        double get_rand_real2();
    
        //! Get the next number in the range (0,1)
        /*!
            Returns the next real number in the range (0,1), i.e., a number
            greater than 0 and less than 1.
            Provides 32-bit precision.
            \return A pseudorandom double value
        */
        double get_rand_real3();
    
        //! Get the next number in the range [0,1)
        /*!
            Returns the next real number in the range [0,1), i.e., a number
            greater than or equal to 0 and less than 1.
            Provides 53-bit precision.
            \return A pseudorandom double value
        */
        double get_rand_real53();
    };

    //---------------------------------------------------------------------------
    //  Returns the original seed value
    inline uint32_t prng::get_seed()
    {
        return m_seed;
    }
        
    //---------------------------------------------------------------------------
    //   Obtain a psuedorandom integer in the range [lo,hi]
    inline uint32_t prng::get_rand_range(uint32_t lo, uint32_t hi)
    {
        // Local working storage
        double range = hi - lo + 1.0;
    
        // Use real value to caluclate range
        return lo + uint32_t(floor(range * get_rand_real2()));
    }

    //--------------------------------------------------------------------------
    //  Returns the next value as a size_t "index" in the range [0,length).
    inline size_t prng::get_rand_index(size_t length)
    {
        return size_t(double(length) * get_rand_real2());
    }

    //--------------------------------------------------------------------------
    //   Obtain a psuedorandom real number in the range [0,1], i.e., a number
    //   greater than or equal to 0 and less than or equal to 1.
    inline double prng::get_rand_real1()
    {
        // privides a granularity of approx. 2.3E-10
        return double(get_rand()) * (1.0 / 4294967295.0);
    }

    //--------------------------------------------------------------------------
    //   Obtain a psuedorandom real number in the range [0,1), i.e., a number
    //   greater than or equal to 0 and less than 1.
    inline double prng::get_rand_real2()
    {
        // privides a granularity of approx. 2.3E-10
        return double(get_rand()) * (1.0 / 4294967296.0);
    }

    //--------------------------------------------------------------------------
    //   Obtain a psuedorandom real number in the range (0,1), i.e., a number
    //   greater than 0 and less than 1.
    inline double prng::get_rand_real3()
    {
        // privides a granularity of approx. 2.3E-10
        return double((double(get_rand()) + 0.5) * (1.0 / 4294967296.0));
    }

} // end namespace libcoyotl

#endif
