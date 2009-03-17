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

#include "../../include/setup.hh"
#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/shapes/shape.hh"
#include "../../include/shapes/closedsphere.hh"
#include "../../include/primitives/primitive.hh"
#include "../../include/primitives/closedsphere.hh"

namespace redshift { namespace primitive {



ClosedSphere::ClosedSphere (Point const & center, real_t radius) 
: sphereData (center, radius) {
}



ClosedSphere::~ClosedSphere () {
}



bool ClosedSphere::doesIntersect (RayDifferential const &ray) const {
        return sphereData.doesIntersect (ray);
}



tuple<bool,Intersection>
 ClosedSphere::intersect(RayDifferential const &ray) const {

        tuple<bool const,DifferentialGeometry const> 
                       i(sphereData.intersect(ray));


        const bool                  & does (get<0>(i));
        const DifferentialGeometry  & dg   (get<1>(i));
        

        Intersection iret = Intersection(
                shared_from_this(),
                dg);

        return make_tuple (does, iret);
}



} }
