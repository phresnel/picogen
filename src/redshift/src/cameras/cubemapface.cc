//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
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

#include "../../include/cameras/cubemapface.hh"

namespace redshift { namespace camera {



namespace {
        Transform cubeRot (CubeMapFace::Face face) {
                switch (face) {
                case CubeMapFace::front:
                        return Transform::identity();
                case CubeMapFace::right:
                        return Transform::rotationY(constants::pi*.5);
                case CubeMapFace::back:
                        return Transform::rotationY(constants::pi);
                case CubeMapFace::left:
                        return Transform::rotationY(-constants::pi*.5);
                case CubeMapFace::top:
                        return Transform::rotationY(-constants::pi*.5)
                             * Transform::rotationX(-constants::pi*.5);
                case CubeMapFace::bottom:
                        return Transform::rotationY(-constants::pi*.5)
                             * Transform::rotationX(constants::pi*.5);
                };
                throw std::runtime_error("unknown CubeMapFace");
        }
}



CubeMapFace::CubeMapFace (
        shared_ptr<RenderTarget> film_,
        Face face,
        Transform const & xform
)
: Camera(xform * cubeRot(face))
, film(film_)
, invFilmWidth(1/static_cast<real_t>(film->getWidth()))
, invFilmHeight(1/static_cast<real_t>(film->getHeight()))
{
}



CubeMapFace::~CubeMapFace() {
}



bool CubeMapFace::hasCommonCenter () const {
        return true;
}



Point CubeMapFace::getCommonCenter () const {
        return vector_cast<Point>(transform.position());
}



//#include <iostream>
tuple<real_t,RayDifferential> CubeMapFace::generateRay (Sample const &sample) const{
        RayDifferential ray;

        ray.direction.x = (-.5 + sample.imageCoordinates.u * invFilmWidth);
        ray.direction.y =  .5 - sample.imageCoordinates.v * invFilmHeight;
        ray.direction.z = .5;

        ray.direction = normalize (ray.direction);
        ray.position = Point(0,0,0);

        return make_tuple (1.0, transform*ray);
}



} } // namespace redshift::CubeMapFace
