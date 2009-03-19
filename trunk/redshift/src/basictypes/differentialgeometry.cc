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

namespace redshift {



DifferentialGeometry::DifferentialGeometry () 
: distance ()
, center ()
, normal ()
{
}



DifferentialGeometry::DifferentialGeometry (
        real_t distance_,
        Point  const &center_,
        Normal const &normal_        
) 
: distance (distance_)
, center (center_)
, normal (normal_)
{
}



DifferentialGeometry::DifferentialGeometry (
        DifferentialGeometry const &dg        
) 
: distance (dg.distance)
, center (dg.center)
, normal (dg.normal)
{
}



DifferentialGeometry& 
DifferentialGeometry::operator= (DifferentialGeometry const &dg) {
        distance = dg.distance;
        center   = dg.center;
        normal   = dg.normal;
        return *this;
}


 
real_t DifferentialGeometry::getDistance() const {
        return distance;
}



Point  DifferentialGeometry::getCenter() const {
        return center;
}



Normal DifferentialGeometry::getNormal() const {
        return normal;
}
                
                
                
}
