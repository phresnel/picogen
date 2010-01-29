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

#include "../../include/basictypes/differentialgeometry.hh"

namespace redshift {



DifferentialGeometry::DifferentialGeometry ()
{
}


/*
DifferentialGeometry::DifferentialGeometry (
        real_t distance_,
        Point  const &center_,
        Normal const &shadingNormal_,
        Normal const &geometricNormal_
)
: distance (distance_)
, center (center_)
, geometricNormal (geometricNormal_)
, shadingNormal (shadingNormal_)
{
        //std::cerr << "warning: old constructor call to DifferentialGeometry" << std::endl;
}*/



DifferentialGeometry::DifferentialGeometry (
        real_t distance_,
        Point  const &center_,
        Normal const &geometricNormal_,
        Vector const &dpdu_, Vector const &dpdv_,
        Vector const &dndu_, Vector const &dndv_
)
: distance(distance_)
, center(center_)
, geometricNormal(geometricNormal_)
, shadingNormal(vector_cast<Normal>(normalize (cross (dpdu_, dpdv_))))
, dpdu(dpdu_), dpdv(dpdv_)
, dndu(dpdu_), dndv(dpdv_)
{
        //std::cout << shadingNormal.x << ":" << shadingNormal.y << ":" << shadingNormal.z << "L" << length(shadingNormal) << "\n";
}



/*DifferentialGeometry::DifferentialGeometry (
        DifferentialGeometry const &dg
)
: distance (dg.distance)
, center (dg.center)
, shadingNormal (dg.shadingNormal)
, geometricNormal (dg.geometricNormal)
{
}



DifferentialGeometry&
DifferentialGeometry::operator= (DifferentialGeometry const &dg) {
        distance = dg.distance;
        center   = dg.center;
        shadingNormal   = dg.shadingNormal;
        geometricNormal = dg.geometricNormal;
        return *this;
}*/



real_t DifferentialGeometry::getDistance() const {
        return distance;
}



Point  DifferentialGeometry::getCenter() const {
        return center;
}



Normal DifferentialGeometry::getShadingNormal () const {
        return shadingNormal;
}



Normal DifferentialGeometry::getGeometricNormal () const {
        return geometricNormal;
}



}
