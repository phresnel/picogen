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

#ifndef SHOWSURFACENORMALS_HH_INCLUDED_20090110
#define SHOWSURFACENORMALS_HH_INCLUDED_20090110

#include "../basictypes/transport.hh"

namespace redshift {
        class ShowSurfaceNormals : public Integrator {
        public:
                virtual tuple<real_t,Color,real_t> Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample, Random& /*rand*/
                ) const {
                        const optional<Intersection> I (
                                                scene.intersect (raydiff));
                        if (I) {
                                //const Vector sunDir (1,1,0);


                                return make_tuple (1.0,
                                        Color::FromRGB(
                                                I->getShadingNormal().x+0.5,
                                                I->getShadingNormal().y+0.5,
                                                I->getShadingNormal().z+0.5
                                        ),
                                        I->getDistance()
                                );
                        } else {
                                Color const col = Color::FromRGB (
                                                0.5+sample.primaryRay.direction.x,
                                                0.5+sample.primaryRay.direction.y,
                                                0.5+sample.primaryRay.direction.z);
                                return make_tuple (1.0, col, constants::infinity);
                        }
                }
        };
}

#endif // SHOWSURFACENORMALS_HH_INCLUDED_20090110
