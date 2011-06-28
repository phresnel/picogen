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

#include "../../include/primitives/waterplane.hh"
#include "../../include/basictypes/height-function.hh"
#include "../../include/basictypes/intersection.hh"

namespace picogen { namespace redshift { namespace primitive {



WaterPlane::WaterPlane (real_t height_, shared_ptr<HeightFunction const> heightFunction, const Color &color)
: fun (heightFunction)
, height(height_)
, color (color)
{
}



WaterPlane::~WaterPlane () {
}



bool WaterPlane::doesIntersect (Ray const &ray) const {
        return ((ray.direction.y<0) == (scalar_cast<real_t>(ray.position.y)>height));
}



optional<Intersection>
 WaterPlane::intersect(Ray const &ray) const {
        if (!doesIntersect (ray))
                return false;
        const real_t d = (height - scalar_cast<real_t>(ray.position.y))
                       / ray.direction.y;

        const Point poi = ray(d);
        const Vector voi = vector_cast<Vector>(poi);

        // For some reason I fail to see in this dull moment, I had to flip
        // u x v with v x u.
        const real_t s = 0.001f;
        const real_t h =  (*fun)(voi.x,voi.z);
        const Vector u = normalize (Vector(s, (*fun)(voi.x+s,voi.z) - h, 0));
        const Vector v = normalize (Vector(0, (*fun)(voi.x,voi.z+s) - h, s));
        //const Normal N = Normal(0,scalar_cast<real_t>(ray.position.y)>height?1:-1,0);//vector_cast<Normal>(cross (u,v));
        /*const Normal N =
                scalar_cast<real_t>(ray.position.y)>height
                ? vector_cast<Normal>(cross (v,u))
                : vector_cast<Normal>(cross (u,v))
        ;*/

        const bool isAbove = !(scalar_cast<real_t>(ray.position.y)>height);

        return Intersection(
                *this,
                DifferentialGeometry (
                        poi,
                        Normal(0,1,0),
                        isAbove ? u : v, isAbove ? v : u,
                        Vector(0,0,0), Vector(0,0,0)
                )
        );
}



} } }
