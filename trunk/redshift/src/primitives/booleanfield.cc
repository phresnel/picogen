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

#include "../../include/primitives/booleanfield.hh"


#include "quatsch/quatsch.hh"
#include "quatsch/frontend/jux.hh"
#include "quatsch/backend/est/backend.hh"

#include "quatsch/configurable-functions/noise2ddef.hh"
#include "quatsch/configurable-functions/layerednoise2ddef.hh"


namespace redshift { namespace primitive {



#include "booleanfield-impl.cc"



BooleanField::BooleanField (shared_ptr<HeightFunction const> fun, real_t detail) 
: impl (new impl::BooleanFieldImpl(500))
{
}



BooleanField::~BooleanField () {
        delete impl;
}



bool BooleanField::doesIntersect (RayDifferential const &ray_) const {
        const Vector pos = vector_cast<Vector> (ray_.position);
        const Vector dir = vector_cast<Vector> (ray_.direction);
        impl::Ray ray = {
                {pos.x, pos.y, pos.z},
                {dir.x, dir.y, dir.z}
        };
        return impl->intersect (ray)>=0.f;
}



bool BooleanField::doesIntersect (Ray const &ray_) const {
        const Vector pos = vector_cast<Vector> (ray_.position);
        const Vector dir = vector_cast<Vector> (ray_.direction);
        impl::Ray ray = {
                {pos.x, pos.y, pos.z},
                {dir.x, dir.y, dir.z}
        };
        return impl->intersect (ray)>=0.f;
}



optional<Intersection>
 BooleanField::intersect(RayDifferential const &ray_) const {

        /*
        tuple<bool const,DifferentialGeometry const>
                       i(sphereData.intersect(ray));


        const bool                  & does (get<0>(i));
        const DifferentialGeometry  & dg   (get<1>(i));
        

        Intersection iret = Intersection(
                shared_from_this(),
                dg);

        return make_tuple (does, iret);
        */
         /*
        optional<DifferentialGeometry> 
                       i(sphereData.intersect(ray));
        if (i) { 
                return Intersection (shared_from_this(), *i);
        } else {
                return false;
        }*/
        const Vector pos = vector_cast<Vector> (ray_.position);
        const Vector dir = vector_cast<Vector> (ray_.direction);
        impl::Ray ray = {
                {pos.x, pos.y, pos.z},
                {dir.x, dir.y, dir.z}
        };
        const float t = impl->intersect (ray);
        if (t < .0f)
                return false;
        const impl::Vector n = impl->normal (ray, t);
        const impl::Point poi = ray * t;
        
        // convert to redshift
        const real_t rt = static_cast<real_t> (t);
        const Normal rn (n.x, n.y, n.z);
        /*const Point  rpoi (
                vector_cast<Point>(Vector(poi.x, poi.y, poi.z))
        );*/

        return Intersection (
                shared_from_this(), 
                DifferentialGeometry (rt, ray_(rt), rn)
        );
}



} }
