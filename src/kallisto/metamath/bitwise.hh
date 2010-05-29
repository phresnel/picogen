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

#ifndef BITWISE_H__INCLUDED__20090210
#define BITWISE_H__INCLUDED__20090210

namespace kallisto {
        // Define left-shift function with positive as well as negative operand.
        template <typename T, int count, bool isPositive>
        struct signed_shl_def;

        template <typename T, int count>
        struct signed_shl_def<T, count, true> {
                static T shift (T const &val) {
                        return val << count;
                }
        };

        template <typename T, int count>
        struct signed_shl_def<T, count, false> {
                static T shift (T const &val) {
                        enum { count_ = -count };
                        return val >> count_;
                }
        };

        template <int count, typename T>
        inline T signed_shl (T const &val) {
                return signed_shl_def<T, count, (count>0)>::shift (val);
        }

        // Define right-shift function with positive as well as negative operand.
        template <typename T, int count, bool isPositive>
        struct signed_shr_def;

        template <typename T, int count>
        struct signed_shr_def<T, count, true> {
                static T shift (T const &val) {
                        return val >> count;
                }
        };

        template <typename T, int count>
        struct signed_shr_def<T, count, false> {
                static T shift (T const &val) {
                        enum { count_ = -count };
                        return val << count_;
                }
        };

        template <int count, typename T>
        inline T signed_shr (T const &val) {
                return signed_shr_def<T, count, (count>0)>::shift (val);
        }
}
#endif // BITWISE_H__INCLUDED__20090210
