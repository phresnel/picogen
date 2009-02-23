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
