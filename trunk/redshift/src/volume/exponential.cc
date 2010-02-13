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

#include "../../include/volume/exponential.hh"
#include <cmath>


namespace redshift { namespace volume {



Exponential::Exponential (
        Color const & sigma_a,
        Color const & sigma_s,
        Color const & Lve,
        real_t henyeyGreensteinParameter,
        real_t baseFactor, real_t exponentFactor,
        Point const & min,
        Vector const & up
)
: DensityRegion (sigma_a, sigma_s, Lve, henyeyGreensteinParameter)
, baseFactor(baseFactor)
, exponentFactor(exponentFactor)
, min(min)
, up(up)
{
}



real_t Exponential::density(const Point &p) const {
        const real_t h = dot (vector_cast<Vector>(p-min), up);
        return baseFactor * std::exp (-exponentFactor * h);
}


} }

