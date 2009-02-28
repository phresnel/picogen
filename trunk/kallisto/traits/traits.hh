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

#ifndef TRAITS_H__INCLUDED__20090210
#define TRAITS_H__INCLUDED__20090210

namespace kallisto { namespace traits {


        template <typename T> struct get_type { typedef T type; };

        struct true_value { enum { value = true }; };
        struct false_value { enum { value = false }; };


        ///////////////////////////////////////////////////////////////////////
        // types_equal
        //   types_equal<A,B>::value = true,  if A == B
        //   types_equal<A,B>::value = false, if A != B
        ///////////////////////////////////////////////////////////////////////
        template<typename A, typename B>
        struct types_equal : public false_value {};

        template<typename T>
        struct types_equal<T,T> : public true_value  {};

        STATIC_ASSERT(!(types_equal<int32_t,int64_t>::value));
        STATIC_ASSERT(!(types_equal<uint64_t,int64_t>::value));
        STATIC_ASSERT((types_equal<float,float>::value));
} }

namespace kallisto { namespace unit_test {
        class test_traits {
        private:
                const static test_traits test;
                test_traits () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // TRAITS_H__INCLUDED__20090210
