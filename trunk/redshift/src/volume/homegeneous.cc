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

#include "../../include/volume/homogeneous.hh"


namespace redshift { namespace volume {



Homogeneous::Homogeneous (
        Color const & sigma_a,
        Color const & sigma_s,
        Color const & Lve
)
: sigma_a_(sigma_a)
, sigma_s_(sigma_s)
, Lve_(Lve)
{
}



// absorption
Color Homogeneous::sigma_a (const Point &p, const Vector &w) const {
        return sigma_a_;
}



// out scattering probability
Color Homogeneous::sigma_s (const Point &p, const Vector &w) const {
        return sigma_s_;
}



// emission
Color Homogeneous::Lve (const Point &p,const Vector &w) const {
        return Lve_;
}



real_t Homogeneous::p (
        const Point &p,
        const Vector &w_in,
        const Vector &w_out
) const {
        return PhaseHG (w_in, w_out, 0.1f);
}



Color Homogeneous::sigma_t (const Point &p, const Vector &w) const {
        return sigma_a(p,w)+sigma_s(p,w);
}



Color Homogeneous::Tau (
        const Ray &r, const Interval &i,
        real_t step, real_t offset
) const {
        return sigma_t(r.position,r.direction) * i.mag();
}



} }

