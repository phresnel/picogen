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

#ifndef FIXED_POINT_H__INCLUDED__20090210
#define FIXED_POINT_H__INCLUDED__20090210

namespace kallisto {
        template <typename INT_T, unsigned int FRACT_BITS>
        struct fixed_point_t {
                INT_T bits;
                // Currently, the information in FRACT_BITS can only be retrieved through traits::get_fixed_point_fract_bits<>
                // or traits::get_fixed_point_properties<>.
                // This is done so as to keep this structure as minimal as possible.
        };
}

namespace kallisto {
        template <typename INT_T, unsigned int FRACT_BITS>
        inline fixed_point_t<INT_T,FRACT_BITS>
        operator+ (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {
                fixed_point_t<INT_T,FRACT_BITS> const ret = {
                        lhs.bits + rhs.bits
                };
                return ret;
        }

        template <typename INT_T, unsigned int FRACT_BITS>
        inline fixed_point_t<INT_T,FRACT_BITS>
        operator- (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {
                fixed_point_t<INT_T,FRACT_BITS> const ret = {
                        lhs.bits - rhs.bits
                };
                return ret;
        }
        
        template <typename INT_T, unsigned int FRACT_BITS>
        bool
        operator < (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {                
                return lhs.bits < rhs.bits;
        }
        
        template <typename INT_T, unsigned int FRACT_BITS>
        bool
        operator > (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {                
                return lhs.bits > rhs.bits;
        }
        
        template <typename INT_T, unsigned int FRACT_BITS>
        bool
        operator == (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {                
                return lhs.bits == rhs.bits;
        }
        
        template <typename INT_T, unsigned int FRACT_BITS>
        bool
        operator != (fixed_point_t<INT_T,FRACT_BITS> const &lhs,
                        fixed_point_t<INT_T,FRACT_BITS> const &rhs) {                
                return lhs.bits != rhs.bits;
        }
}


namespace kallisto { namespace unit_test {
        class test_fixed_point_t {
        private:
                const static test_fixed_point_t test;
                test_fixed_point_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }
#endif // FIXED_POINT_H__INCLUDED__20090210
