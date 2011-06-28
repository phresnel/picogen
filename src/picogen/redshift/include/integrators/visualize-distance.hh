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

#ifndef VISUALIZEDISTANCE_HH_INCLUDED_20090110
#define VISUALIZEDISTANCE_HH_INCLUDED_20090110

#include "../basictypes/scene.hh"
#include "../basictypes/transport.hh"
#include "../basictypes/intersection.hh"
#include "../optional.hh"

namespace picogen { namespace redshift {
        class VisualizeDistance : public Integrator {
        public:
                DistantRadiance Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &,
                        const LiRecursion &,
                        Random&
                ) const {
                        const optional<Intersection> I (
                                scene.intersect (raydiff));
                        if (I) {
                                //const Vector sunDir (1,1,0);

                                const real_t dist = length(raydiff.position-
                                                           I->getCenter());
                                return DistantRadiance (
                                        Color::FromRGB(
                                                dist*0.05,
                                                dist*0.025,
                                                dist*0.0125,
                                                IlluminantSpectrum
                                        ),
                                        Distance(dist)
                                );
                        } else {
                                Color const col = Color::FromRGB(
                                                0.5+raydiff.direction.x,
                                                0.5+raydiff.direction.y,
                                                0.5+raydiff.direction.z,
                                                IlluminantSpectrum);
                                return DistantRadiance (
                                        col,
                                        Distance(constants::infinity));
                        }
                }
        };
} }

#endif // VISUALIZEDISTANCE_HH_INCLUDED_20090110
