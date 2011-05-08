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

#include "../../include/bxdf/lambertian.hh"
#include "../../include/random.hh"
#include "../../include/sampling.hh"

namespace redshift { namespace bsdf {



Lambertian::Lambertian (
        Color const &color_
)
: Bxdf (BsdfType(Reflective, Diffuse))
, color (color_ * (1.f/constants::pi))
{}



BsdfSample Lambertian::sample_f (
        const Vector &out, Random &rand
) const {
        const tuple<real_t,real_t,real_t> sphere = cosineHemisphereR(rand);
        const Vector in (get<0>(sphere), get<1>(sphere), get<2>(sphere));
        return BsdfSample(f(out, in, rand),
                          in,
                          fabs(in.y * (1/constants::pi)),
                          type());
}



Color Lambertian::f (const Vector &/*out*/, const Vector &/*in*/, Random &) const {
        return color;
}


real_t Lambertian::pdf (const Vector &out, const Vector &in) const {
        if (sameHemisphere (out, in)) return fabs(in.y * (1/constants::pi));
        return 0;
}


} }
