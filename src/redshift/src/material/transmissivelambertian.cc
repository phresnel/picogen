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

#include "../../include/material/transmissivelambertian.hh"
#include "../../include/random.hh"
#include "../../include/sampling.hh"

namespace redshift { namespace bsdf {



TransmissiveLambertian::TransmissiveLambertian (
        Color const &color_,
        real_t transmissionProb
)
: Bxdf (BsdfType(Transmissive, Diffuse))
, color (color_ * (1.f/constants::pi))
, transmissionProb(transmissionProb)
{}



BsdfSample TransmissiveLambertian::sample_f (
        const Vector &out, Random &rand
) const {
        return BsdfSample(
                f(out, -out, rand),
                -out, 1,
                type()
        );

        const tuple<real_t,real_t,real_t> sphere = cosineHemisphereR(rand);
        const bool transmission = rand() <= transmissionProb;
        const Vector in (
                get<0>(sphere),
                transmission ? -get<1>(sphere) : get<1>(sphere),
                get<2>(sphere));
        return BsdfSample(f(out, in, rand),
                          in,
                          in.y * (1/constants::pi),
                          type());
}



Color TransmissiveLambertian::f (const Vector &out,
                                 const Vector &in, Random &
) const {
        return color;
}



} }
