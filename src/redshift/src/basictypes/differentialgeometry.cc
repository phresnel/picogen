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



DifferentialGeometry::DifferentialGeometry (
        //real_t distance_,
        Point  const &center_,
        Normal const &geometricNormal_,
        Vector const &dpdu_, Vector const &dpdv_,
        Vector const &dndu_, Vector const &dndv_
)
: center(center_)
, geometricNormal(geometricNormal_)
, shadingNormal(vector_cast<Normal>(normalize (cross (dpdu_, dpdv_))))
, dpdu(dpdu_), dpdv(dpdv_)
, dndu(dpdu_), dndv(dpdv_)
{
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



void DifferentialGeometry::applyTransform (Transform const &t) {
        center = t * center;
        geometricNormal = normalize(t * geometricNormal);
        shadingNormal = normalize(t * shadingNormal);
        dpdu = t * dpdu;
        dpdv = t * dpdv;
        dndu = t * dndu;
        dndv = t * dndv;
}



}
