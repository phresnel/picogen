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

#include "../../include/backgrounds/preetham-adapter.hh"

namespace redshift { namespace backgrounds {

PreethamAdapter::PreethamAdapter (
        shared_ptr<redshift::background::Preetham> preetham
) : preetham (preetham) {
}

Color PreethamAdapter::query (Ray const &ray) const {
        return preetham->shade (ray);
                //+ preetham->sunShade(ray);
        //return Color::fromRgb (1,0.5,0.25);
}

Color PreethamAdapter::diffuseQuery (
        Point const &poi, Normal const &normal
) const {
        Color sum = Color::fromRgb (0,0,0);
        Ray ray;
        ray.position = poi;

        int numSamples;
        for (numSamples = 0; numSamples < 10; ++numSamples) {
                const tuple<real_t,real_t,real_t> sphere = diffuseRng.cosine_hemisphere();
                const tuple<Vector,Vector,Vector> cs = coordinateSystem (normal);
        
                const real_t &sx = get<0>(sphere);
                const real_t &sy = get<1>(sphere);
                const real_t &sz = get<2>(sphere);
                const Vector &X = get<0>(cs);
                const Vector &Y = get<1>(cs);
                const Vector &Z = get<2>(cs);
                const Vector d = X * sx + Y * sy + Z * sz;
                if (d.y > 0) {
                        ray.direction = d;
                        sum = sum + query (ray);
                }
        }
        return sum * (1./numSamples);

        return query (Ray(poi, vector_cast<Vector>(normal)));
}

Color PreethamAdapter::atmosphereShade (
        Color const &color, Ray const &ray, real_t distance
) const {
        return preetham->atmosphereShade (color,ray,distance);
}

} } // namespace redshift { namespace backgrounds {
