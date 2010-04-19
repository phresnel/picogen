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

#ifndef PLANE_HH_INCLUDED_20100510
#define PLANE_HH_INCLUDED_20100510

namespace kallisto {
        // Plane of form P*N - d = 0
        template <typename real_t> class Plane {
        public:
                Vector<CARTESIAN, real_t> normal;
                real_t offset;

                Plane (Point<CARTESIAN, real_t> const & point,
                       Vector<CARTESIAN, real_t> const & normal_
                )
                : normal (normal_ * (1/length(normal_)))
                , offset (
                        -dot (normal_,
                             vector_cast<Vector<CARTESIAN, real_t> >(point))
                        * (1/length(normal_)))
                {}
        };


        // We are a bit sloppy here w.r.t. scalar types
        template <
                typename point_t,
                typename direction_t,
                typename real_t
        >
        real_t
        intersect(
                Ray<point_t,direction_t> const & ray,
                Plane<real_t> const & plane
        ) {
                typedef Vector<CARTESIAN,real_t> Vec;

                // http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
                const Vec P0 = vector_cast<Vec>(ray.position);
                const real_t
                        a = dot (P0, plane.normal) - plane.offset,
                        b = dot (ray.direction, plane.normal),
                        t = -a / b;
                return t;
        }


        template <
                typename real_t
        >
        real_t
        distance (
                Plane<real_t> const & plane,
                Point<CARTESIAN, real_t> const & point
        ) {
                typedef Vector<CARTESIAN, real_t> Vec;
                typedef Normal<CARTESIAN, real_t> Normal;
                return dot(
                        plane.normal,
                        vector_cast<Vec>(point)
                ) + plane.offset;
        }



        template <
                typename real_t
        >
        bool is_above (
                Plane<real_t> const & plane,
                Point<CARTESIAN, real_t> const & point
        ) {
                return distance(plane, point) >= 0;
        }
}

namespace kallisto { namespace unit_test {
        class test_plane_t {
        private:
                const static test_plane_t test;
                test_plane_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // PLANE_HH_INCLUDED_20100510
