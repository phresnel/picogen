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

#include "../../include/cameras/cylindrical.hh"
#include "../../include/basictypes/sample.hh"
#include "../../include/constants.hh"
#include "../../include/rendertargets/rendertarget.hh"

namespace picogen { namespace redshift { namespace camera {



Cylindrical::Cylindrical (
        unsigned int filmWidth, unsigned int filmHeight,
        real_t frontDist,
        Transform const & xform
)
: Camera(xform)
, invFilmWidth(constants::one/static_cast<real_t>(filmWidth))
, invFilmHeight(constants::one/static_cast<real_t>(filmHeight))
, aspect (static_cast<real_t>(filmWidth) / filmHeight)
, frontDist (frontDist)
{
        // hacky: do approximate pinhole-camera by "intersection" left/right-
        //        most rays with front-plane and then taking the angle
        const real_t
                x_(-0.5 * aspect), z_(frontDist), // <--pinhole for u=0, v=0
                l (1 / (x_*x_ + z_*z_)),
                /*x(x_*l),*/ z(z_*l) // we don't need x for dot product
        ;

        angleRange = std::acos(z) * 2; // "reduced" dot product and then angle
}



Cylindrical::~Cylindrical() {
}



bool Cylindrical::hasCommonCenter () const {
        return true;
}



Point Cylindrical::getCommonCenter () const {
        return vector_cast<Point>(transform.position());
}



//#include <iostream>
tuple<real_t,RayDifferential> Cylindrical::generateRay (Sample const &sample) const{
        RayDifferential ray;
        // TODO: need aspect ratio parameter
        ray.direction.y =  0.5 - sample.imageCoordinates.v * invFilmHeight;

         //ray.direction.x = (-0.5 + sample.imageCoordinates.u * invFilmWidth)*aspect;
         //ray.direction.z = frontDist;
        const real_t u = -0.5 + sample.imageCoordinates.u * invFilmWidth;
        const real_t alpha = u*angleRange;
        ray.direction.x = frontDist * sin(alpha);
        ray.direction.z = frontDist * cos(alpha);

        ray.direction = normalize (ray.direction);
        ray.position = Point(0,0,0);
        /*ray.minT = constants::epsilon;
        ray.maxT = constants::infinity;*/
        return make_tuple (1.0, transform*ray);
}



} } }
