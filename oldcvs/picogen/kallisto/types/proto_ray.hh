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

#ifndef PROTO_RAY_H_INCLUDED_20090224
#define PROTO_RAY_H_INCLUDED_20090224

namespace kallisto {
        template <typename point_t, typename direction_t> struct proto_ray_t {
                point_t position;
                direction_t direction;
        };
}



namespace kallisto {
        template <typename point_t, typename direction_t> class ProtoRay {
        public:
                point_t position;
                direction_t direction;

                /*template <typename T>
                position_t
                operator () (
                   T const &s
                ) const {
                        return position + direction * s;
                }*/
        };
}



namespace kallisto { namespace unit_test {
        class test_proto_ray_t {
        private:
                const static test_proto_ray_t test;
                test_proto_ray_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // PROTO_RAY_H_INCLUDED_20090224
