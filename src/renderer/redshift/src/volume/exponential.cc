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

#include "ignore_strict_aliasing" // because of boost::optional
#include "../../include/volume/exponential.hh"
#include "../../include/interval.hh"
#include <cmath>


namespace redshift { namespace volume {



Exponential::Exponential (
        Color const & sigma_a,
        Color const & sigma_s,
        Color const & Lve,
        real_t henyeyGreensteinParameter,
        real_t baseFactor, real_t exponentFactor,
        Point const & min,
        Vector const & up,
        real_t epsilon
)
: DensityRegion (sigma_a, sigma_s, Lve, henyeyGreensteinParameter)
, baseFactor(baseFactor)
, exponentFactor(exponentFactor)
, min(min)
, up(normalize(up))
, upPlane (approximateUpperPlane(epsilon))
{
        /*std::cout << "plane = {" << upPlane.normal.x
                                 << ", " << upPlane.normal.y
                                 << ", " << upPlane.normal.z
                                 << ", " << upPlane.offset << "}\n";*/
}



Plane Exponential::approximateUpperPlane (real_t epsilon) const {
        //std::cout << "random sampling exponential volume...\n";

        // TODONT: use a real root finder
        real_t h;
        int steps = 0;
        for (h=1; density(h) > epsilon; ++steps, h+=h/32){}
        /*std::cout << "approximate root at h=" << h
                  << " in " << steps
                  << " steps (density=" << density(h) << ")\n";
        std::cout << "done!\n";*/

        const Point through = min+up*h;
        /*std::cout << "plane through: "
                  << through.x << ", "
                  << through.y << ", "
                  << through.z << "\n";*/

        return Plane(vector_cast<PointF>(through), up);
}



Interval Exponential::cull (const Ray &ray) const {
        // TODO: take into account plane parallel planes
        const real_t dist = //(ray.position.y-1215.54)/-ray.direction.y;
                intersect(ray, upPlane);

        if (is_above(upPlane, ray.position)) {
                if (dist >= 0) { // is above, pointing down
                        return Interval(dist, constants::real_max);
                } else { // is above, pointing up
                        return Interval(constants::real_max,-constants::real_max);
                }
        } else {
                if (dist >= 0) { // is below, pointing up
                        return Interval(0, dist);
                } else { // is below, pointing down
                        return Interval(0, constants::real_max);
                }
        }
}



real_t Exponential::density (real_t h) const {
        return baseFactor * std::exp (-exponentFactor * h);
}



real_t Exponential::density(const Point &p, Random& /*rand*/) const {
        const real_t h = dot (vector_cast<Vector>(p-min), up);
        return density (h);
}


} }

