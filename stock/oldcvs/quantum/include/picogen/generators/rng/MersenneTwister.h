/***************************************************************************
 *            MersenneTwister.h
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


#ifndef _MERSENNETWISTER_H
#define _MERSENNETWISTER_H

#include <picogen/foreign/libcoyotl/mtwister.h>
#include <picogen/picogen.h>

namespace picogen {
    namespace generators {
        namespace rng {

            class MersenneTwister : public IRNG {
                    libcoyotl::mtwister coyoteTwister;
                public:
                    MersenneTwister ();
                    MersenneTwister (::picogen::uint32 seed);
                    ~MersenneTwister ();

                    void seed (::picogen::uint32 seed);
                    ::picogen::uint32 rand();
                    ::picogen::real randf(); // [0..1]
            };

        } // namespace rng {
    } // namespace misc {
} // namespace picogen {

#endif // #ifndef _MERSENNETWISTER_H
