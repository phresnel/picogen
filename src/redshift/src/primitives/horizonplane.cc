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

#include "../../include/primitives/horizonplane.hh"

namespace redshift { namespace primitive {



HorizonPlane::HorizonPlane (real_t height_, const Color &color)
: height(height_)
, color (color)
{
}



HorizonPlane::~HorizonPlane () {
}



bool HorizonPlane::doesIntersect (RayDifferential const &ray) const {
        return ((ray.direction.y<0) == (scalar_cast<real_t>(ray.position.y)>height));
}



bool HorizonPlane::doesIntersect (Ray const &ray) const {
        return ((ray.direction.y<0) == (scalar_cast<real_t>(ray.position.y)>height));
}



optional<Intersection>
 HorizonPlane::intersect(RayDifferential const &ray) const {
        if (!doesIntersect (ray))
                return false;
        const real_t d = (height - scalar_cast<real_t>(ray.position.y))
                       / ray.direction.y;

        const Point poi = ray(d);
        const Vector voi = vector_cast<Vector>(poi);

        const Vector u = Vector(1,0,0);
        const Vector v = Vector(0,0,1);

        const bool isAbove = !(scalar_cast<real_t>(ray.position.y)>height);

        return Intersection(
                shared_from_this(),
                DifferentialGeometry (
                        d,
                        poi,
                        Normal(0,1,0),
                        isAbove ? u : v, isAbove ? v : u,
                        Vector(0,0,0), Vector(0,0,0)
                )
        );
}



} }