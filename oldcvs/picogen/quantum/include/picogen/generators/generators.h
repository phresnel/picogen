/***************************************************************************
 *            generators.h
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


#ifndef _GENERATORS_H
#define _GENERATORS_H

namespace picogen {
    namespace generators {
        namespace rng {
            class IRNG {
                public:
                    virtual void seed (::picogen::uint32 seed) = 0;
                    virtual ::picogen::uint32 rand() = 0;
                    virtual ::picogen::real randf() = 0; // [0..1]
            };
        } // namespace rng
    } // namespace generators
} // namespace picogen

#include <picogen/generators/rng/MersenneTwister.h>

#endif
