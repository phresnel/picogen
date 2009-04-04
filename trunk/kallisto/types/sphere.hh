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

#ifndef SPHERE_H_INCLUDED_20090223
#define SPHERE_H_INCLUDED_20090223

namespace kallisto {
        template <typename center_t, typename radius_t> struct sphere_t {
                center_t center;
                radius_t radius;
        };
}

namespace kallisto {
        template <typename center_t, typename radius_t> class Sphere {
        public:
                center_t center;
                radius_t radius;

                Sphere (center_t const &center, radius_t const &radius)
                : center (center), radius (radius)
                {}
        };
        
        
        // http://devmaster.net/wiki/Ray-sphere_intersection: Algebraic Method
        template <
                bool do_normalize_ray_direction,
                typename point_t, 
                typename radius_t,
                typename direction_t
        >
        optional <typename traits::get_scalar_type<direction_t>::type>
        intersect(
                Ray<point_t,direction_t> const & ray,
                Sphere<point_t,radius_t> const & sphere
        ) {
                typedef
                    typename traits::get_scalar_type<direction_t>::type real_t;
                
                direction_t ray_direction = ray.direction;
                real_t final_scale = 1.0;
                if (do_normalize_ray_direction) {
                        // Ugly hack to return the distance in 
                        // incoming-ray-space.
                        final_scale = 1.0 / length (ray.direction);
                        ray_direction = normalize (ray.direction); 
                }

        	direction_t const dst = vector_cast<direction_t> 
        	                                (ray.position - sphere.center);

        	real_t const B = dst * ray_direction;

        	real_t const C = (dst*dst) - 
        	              static_cast<real_t>(sphere.radius*sphere.radius);

        	real_t const D = B*B - C;
        	real_t const E = final_scale * (-B - sqrt(D));
        	if ((D>=0) & (E>=ray.minT) & (ray.maxT>=E)) {
        	        return E;
        	} else {
        	        return optional <real_t>();
        	}
        }
}

namespace kallisto { namespace unit_test {
        class test_sphere_t {
        private:
                const static test_sphere_t test;
                test_sphere_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // SPHERE_H_INCLUDED_20090223
