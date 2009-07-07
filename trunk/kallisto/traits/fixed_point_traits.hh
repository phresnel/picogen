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

#ifndef FIXED_POINT_TRAITS_H_20090221
#define FIXED_POINT_TRAITS_H_20090221

///////////////////////////////////////////////////////////////////////////////
// fixed_point_t traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_fixed_point_basetype
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_fixed_point_basetype;
        template <typename T, unsigned int FRACT_BITS>
        struct get_fixed_point_basetype <fixed_point_t<T, FRACT_BITS> > {
                typedef T type;
        };

        STATIC_ASSERT((types_equal<
                long,
                get_fixed_point_basetype< fixed_point_t<long,5> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                uint64_t,
                get_fixed_point_basetype< fixed_point_t<uint64_t,5> >::type
        >::value));


        ///////////////////////////////////////////////////////////////////////
        // get_fixed_point_fract_bits
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_fixed_point_fract_bits;

        template <typename T, unsigned int FRACT_BITS>
        struct get_fixed_point_fract_bits <fixed_point_t<T, FRACT_BITS> > {
                enum { value = FRACT_BITS };
        };


        STATIC_ASSERT(
                (5==get_fixed_point_fract_bits<fixed_point_t<long,5> >::value)
        );
        STATIC_ASSERT(
                (0==get_fixed_point_fract_bits<fixed_point_t<long,0> >::value)
        );
        STATIC_ASSERT(
                (2!=get_fixed_point_fract_bits<fixed_point_t<char,0> >::value)
        );

        ///////////////////////////////////////////////////////////////////////
        // is_fixed_point
        //   is_fixed_point<T>::value = true, if T in {fixed_point_t<>}
        //   is_fixed_point<T>::value = false, otherwise
        ///////////////////////////////////////////////////////////////////////

        // ?
        template <typename T> struct is_fixed_point : public false_value {};

        // fixed_point_t
        template <typename T, unsigned int FRACT_BITS>
        struct is_fixed_point <fixed_point_t<T, FRACT_BITS> >
        : public true_value {};


        STATIC_ASSERT((is_fixed_point<fixed_point_t<long,5> >::value));
        STATIC_ASSERT((is_fixed_point<fixed_point_t<uint64_t,32> >::value));
        STATIC_ASSERT(!(is_fixed_point<float>::value));
        STATIC_ASSERT(!(is_fixed_point<int>::value));



        // Combined tests.
        STATIC_ASSERT((types_equal<
                fixed_point_t<
                  get_fixed_point_basetype<fixed_point_t<int,4> >::type,
                  get_fixed_point_fract_bits<fixed_point_t<int,4> >::value
                >,
                fixed_point_t<int,4> >::value));
        STATIC_ASSERT(!(types_equal<
                fixed_point_t<
                  get_fixed_point_basetype<fixed_point_t<unsigned,4> >::type,
                  get_fixed_point_fract_bits<fixed_point_t<int,4> >::value
                >,
                fixed_point_t<int,4> >::value));
} }

namespace std {
        ///////////////////////////////////////////////////////////////////////
        // numeric_limits        
        ///////////////////////////////////////////////////////////////////////
        template <typename T, unsigned int FRACT_BITS>
        struct numeric_limits<kallisto::fixed_point_t<T,FRACT_BITS> > {
                static kallisto::fixed_point_t<T,FRACT_BITS> max() {
                        const kallisto::fixed_point_t<T,FRACT_BITS> ret = {
                                numeric_limits<T>::max()
                        };
                        return ret;
                }
        };
}


#endif // FIXED_POINT_TRAITS_H_20090221
