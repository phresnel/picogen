/***************************************************************************
 *            MersenneTwister.cc
 *
 *  2008-08-21 20:20:00 2008
 *  Copyright  2007  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



#include <picogen/generators/rng/MersenneTwister.h>

namespace picogen {
    namespace generators {
        namespace rng {


            MersenneTwister::MersenneTwister () : coyoteTwister () {
            }



            MersenneTwister::MersenneTwister (::picogen::uint32 seed) : coyoteTwister (seed) {
            }



            MersenneTwister::~MersenneTwister () {
            }



            void MersenneTwister::seed (uint32 seed) {
                coyoteTwister.init (seed);
            }



            ::picogen::uint32 MersenneTwister::rand() {
                return coyoteTwister.get_rand();
            }



            ::picogen::real MersenneTwister::randf() {

                //
                // Quick paste from the libcoyotl::prng.h/cc:
                //
                // [quote]
                //--------------------------------------------------------------------------
                // inline double prng::get_rand_real1()
                //  Obtain a psuedorandom real number in the range [0,1], i.e., a number
                //   greater than or equal to 0 and less than or equal to 1.
                //--------------------------------------------------------------------------
                // inline double prng::get_rand_real2()
                //   Obtain a psuedorandom real number in the range [0,1), i.e., a number
                //   greater than or equal to 0 and less than 1.
                //--------------------------------------------------------------------------
                // inline double prng::get_rand_real3()
                //   Obtain a psuedorandom real number in the range (0,1), i.e., a number
                //   greater than 0 and less than 1.
                //--------------------------------------------------------------------------
                //   Obtain a psuedorandom real number in the range [0,1), i.e., a number
                //   greater than or equal to 0 and less than 1, with 53-bit precision.
                //--------------------------------------------------------------------------
                // [/quote]
                //
                // >> I am personally solely interested in [0..1], hence...
                return coyoteTwister.get_rand_real1();
            }


        } // namespace rng {
    } // namespace misc {
} // namespace picogen {