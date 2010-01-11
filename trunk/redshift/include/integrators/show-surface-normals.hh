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

#ifndef DIRECTLIGHTING_HH_INCLUDED_20090110
#define DIRECTLIGHTING_HH_INCLUDED_20090110

#include "../basictypes/transport.hh"

namespace redshift {
        class DirectLighting : Integrator {
        public:
                virtual tuple<real_t,Color> Li (
                        const Scene &scene, 
                        const RayDifferential &raydiff,
                        const Sample &sample
                ) const {
                        const optional<Intersection> I (
                                                scene.intersect (raydiff));
                        if (I) {
                                //const Vector sunDir (1,1,0);
                                
                                
                                return make_tuple (1.0, 
                                        Color(
                                                I->getDistance()*0.05,
                                                I->getDistance()*0.025,
                                                I->getDistance()*0.0125
                                                /*I->getNormal().x+0.5,
                                                I->getNormal().y+0.5,
                                                I->getNormal().z+0.5*/
                                        )
                                );                
                        } else {
                                Color const col (0.5+sample.primaryRay.direction.x,
                                                 0.5+sample.primaryRay.direction.y,
                                                 0.5+sample.primaryRay.direction.z);
                                return make_tuple (1.0, col);
                        }
                }
        };
}

#endif // DIRECTLIGHTING_HH_INCLUDED_20090110
