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

#include "../../include/cameras/pinhole.hh"

namespace redshift { namespace camera {



Pinhole::Pinhole (
        shared_ptr<RenderTarget> film_,
        real_t frontDist,
        Transform const & xform
)
: Camera(xform)
, film(film_)
, invFilmWidth(constants::one/static_cast<real_t>(film->getWidth()))
, invFilmHeight(constants::one/static_cast<real_t>(film->getHeight()))
, aspect (static_cast<real_t>(film->getWidth()) / film->getHeight())
, frontDist (frontDist)
{
}



Pinhole::~Pinhole() {
}



bool Pinhole::hasCommonCenter () const {
        return true;
}



Point Pinhole::getCommonCenter () const {
        return vector_cast<Point>(transform.position());
}



//#include <iostream>
tuple<real_t,RayDifferential> Pinhole::generateRay (Sample const &sample) const{
        RayDifferential ray;
        // TODO: need aspect ratio parameter
        ray.direction.x = (-0.5 + sample.imageCoordinates.u * invFilmWidth)*aspect;
        ray.direction.y =  0.5 - sample.imageCoordinates.v * invFilmHeight;
        ray.direction.z = frontDist;
        ray.direction = normalize (ray.direction);
        ray.position = Point(0,0,0);
        /*ray.minT = constants::epsilon;
        ray.maxT = constants::infinity;*/
        return make_tuple (1.0, transform*ray);
}



} } // namespace redshift::pinhole
