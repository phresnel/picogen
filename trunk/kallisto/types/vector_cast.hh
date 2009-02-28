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

#ifndef VECTOR_CAST_H__INCLUDED__20090210
#define VECTOR_CAST_H__INCLUDED__20090210

namespace kallisto {
        ///////////////////////////////////////////////////////////////////////
        // vector cast
        ///////////////////////////////////////////////////////////////////////

        template <typename lhs_t, typename rhs_t> struct vector_cast_def ;

        // T==T
        template <typename T>
        struct vector_cast_def<T,T> {
                static T cast (T const & rhs) { return rhs; }
        };

        // T!=T
        template <typename lhs_t, typename rhs_t>
        struct vector_cast_def {
                static lhs_t cast (rhs_t const & rhs) {
                        // TODO: ret shalt be const.
                        lhs_t ret;
                        scalar_cast (ret.x, rhs.x);
                        scalar_cast (ret.y, rhs.y);
                        scalar_cast (ret.z, rhs.z);
                        return ret;
                }
        };

        template <typename lhs_t, typename rhs_t>
        inline lhs_t vector_cast (rhs_t const & rhs) {
                return vector_cast_def<lhs_t, rhs_t>::cast (rhs);
        }
}


namespace kallisto { namespace unit_test {
        class test_vector_cast {
        private:
                const static test_vector_cast test;
                test_vector_cast () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // VECTOR_CAST_H__INCLUDED__20090210
