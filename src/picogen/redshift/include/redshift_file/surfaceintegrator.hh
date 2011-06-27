//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef SURFACEINTEGRATOR_HH_20101013
#define SURFACEINTEGRATOR_HH_20101013

#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

namespace picogen { namespace redshift_file {
        struct SurfaceIntegrator {
                enum Type {
                        none,
                        whitted,
                        whitted_ambient,
                        path,
                        debug_distance,
                        debug_normals
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                unsigned int numAmbientSamples;

                SurfaceIntegrator ()
                : type(whitted_ambient), numAmbientSamples(10)
                {}                

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);
        };
} }

#endif // SURFACEINTEGRATOR_HH_20101013
