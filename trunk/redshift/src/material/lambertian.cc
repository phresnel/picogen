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
        //DifferentialGeometry const &shadingDG_,
        Color const &color_,
        shared_ptr<MersenneTwister<real_t,0,1> > mt_
)
//: Bsdf (shadingDG_)
: Bxdf (Bsdf::reflection, Bsdf::diffuse)
, color (color_ * (1.f/constants::pi))
, mt(mt_)
{}



optional<tuple<Color,Vector> > Lambertian::sample_f (
        const Vector &in
) const {
        const tuple<real_t,real_t,real_t> sphere = mt->cosine_hemisphere();
        const real_t &sx = get<0>(sphere);
        const real_t &sy = get<1>(sphere);
        const real_t &sz = get<2>(sphere);
        return make_tuple (color, Vector(sx,sy,sz));
}



Color Lambertian::f (const Vector &out, const Vector &in) const {
        return color;
}



} }
