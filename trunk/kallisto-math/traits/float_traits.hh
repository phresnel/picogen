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

#ifndef FLOAT_TRAITS_H_20090221
#define FLOAT_TRAITS_H_20090221

///////////////////////////////////////////////////////////////////////////////
// point_t and Point traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // is_float
        //   is_float<T>::value = true, if T in {float, double, long double}
        //   is_float<T>::value = false, otherwise
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct is_float    : public false_value {};
        template <> struct is_float<float>       : public true_value {};
        template <> struct is_float<double>      : public true_value {};
        template <> struct is_float<long double> : public true_value {};

        STATIC_ASSERT(!is_float<int>::value);
        STATIC_ASSERT(!is_float<bool>::value);
        STATIC_ASSERT(!is_float<void*>::value);
        STATIC_ASSERT(is_float<float>::value);
        STATIC_ASSERT(is_float<double>::value);
        STATIC_ASSERT(is_float<long double>::value);
} }

#endif // FLOAT_TRAITS_H_20090221
