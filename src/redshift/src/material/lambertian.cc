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

#include "../../include/material/lambertian.hh"

namespace redshift { namespace bsdf {



Lambertian::Lambertian (
        Color const &color_
)
: Bxdf (Bsdf::reflection, Bsdf::diffuse)
, color (color_ * (1.f/constants::pi))
{}



tuple<Color,Vector,real_t> Lambertian::sample_f (
        const Vector &out, Random &rand
) const {
        const tuple<real_t,real_t,real_t> sphere = cosineHemisphereR(rand);
        const Vector in (get<0>(sphere), get<1>(sphere), get<2>(sphere));
        return make_tuple (f(out, in, rand), in, in.y * (1/constants::pi));
}



Color Lambertian::f (const Vector &out, const Vector &in, Random &) const {
        return color;
}



} }
