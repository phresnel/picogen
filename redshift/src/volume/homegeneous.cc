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
        Color const & Lve,
        real_t henyeyGreensteinParameter
)
: sigma_a_(sigma_a)
, sigma_s_(sigma_s)
, Lve_(Lve)
, henyeyGreensteinParameter(henyeyGreensteinParameter)
{
}



// absorption
Color Homogeneous::sigma_a (const Point &p, const Vector &w, Random& rand) const {
        return sigma_a_;
}



// out scattering probability
Color Homogeneous::sigma_s (const Point &p, const Vector &w, Random& rand) const {
        return sigma_s_;
}



// emission
Color Homogeneous::Lve (const Point &p,const Vector &w, Random& rand) const {
        return Lve_;
}



real_t Homogeneous::p (
        const Point &p,
        const Vector &w_in,
        const Vector &w_out, Random& rand
) const {
        return phaseHG (w_in, w_out, henyeyGreensteinParameter);
}



Color Homogeneous::sigma_t (const Point &p, const Vector &w, Random& rand) const {
        const Color a = sigma_a(p,w,rand);
        const Color b = sigma_s(p,w,rand);
        return a+b;
}



Color Homogeneous::tau (
        const Ray &r, const Interval &i,
        real_t step, real_t offset, Random& rand
) const {
        const real_t min = i.min();
        real_t max = i.max();
        if (max == constants::infinity)
                max = constants::real_max;
        return sigma_t(r.position,r.direction,rand) * (max-min);
}



} }

