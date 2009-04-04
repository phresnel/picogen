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
#include "../../include/shapes/shape.hh"
#include "../../include/shapes/closedsphere.hh"


namespace redshift { namespace shape {



ClosedSphere::ClosedSphere (Point const & center, real_t radius) 
: sphereData(center, radius) {
}



bool ClosedSphere::doesIntersect (Ray const &ray) const {
        if (kallisto::intersect<true> (ray, sphereData))
                return true;
        else
                return false;
}




optional<DifferentialGeometry> ClosedSphere::intersect(Ray const &ray) const {
        const optional<real_t> d = kallisto::intersect<true> (ray, sphereData);
        if (!d) {
                return false;
        }

        const Point  poi = ray (*d);
        const Normal normal = 
                     normalize (vector_cast<Normal> (poi - sphereData.center));
        
        return DifferentialGeometry (*d, poi, normal);
}



} }