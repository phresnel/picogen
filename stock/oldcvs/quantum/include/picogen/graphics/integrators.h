/***************************************************************************
 *            samplers.h
 *
 *  Thu Oct 11 19:58:36 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
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


#ifndef INTEGRATORS_H__INCLUDED
#define INTEGRATORS_H__INCLUDED

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace screen {
            }
            namespace surface {
                class ISurfaceIntegrator {
                    public:
                        virtual bool integrate (::picogen::graphics::structs::sample &sample) const = 0;
                        virtual ~ISurfaceIntegrator() {};
                };
            }
        }
    }
}

#include "integrators/surface/Path.h"
#include "integrators/surface/Whitted.h"

#include "integrators/image/XYIterator.h"
// obsolete ++
#include "integrators/ray/Simple.h"
#include "integrators/ray/Preview.h"
// --

#endif /* INTEGRATORS_H__INCLUDED */
