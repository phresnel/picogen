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

#ifndef NORMAL_POINT_TRAITS_H_20090221
#define NORMAL_POINT_TRAITS_H_20090221

///////////////////////////////////////////////////////////////////////////////
// normal_t and Normal traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_scalar_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_scalar_type;


        // normal_t
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <normal_t<SPACE,T> > : public get_type<T> {};

        // Normal
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <Normal<SPACE,T> > : public get_type<T> {};


        // Tests
        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<normal_t<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        normal_t<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                double,
                get_scalar_type<normal_t<SPHERICAL,double> >::type
        >::value));

        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<Normal<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        Normal<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                get_scalar_type<Normal<CARTESIAN,double> >::type,
                get_scalar_type<normal_t<SPHERICAL,double> >::type
        >::value));
        STATIC_ASSERT(!(types_equal<
                get_scalar_type<Normal<CARTESIAN,float> >::type,
                get_scalar_type<normal_t<SPHERICAL,double> >::type
        >::value));


        ///////////////////////////////////////////////////////////////////////
        // get_coordinate_space
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_coordinate_space;

        // normal_t
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <normal_t<SPACE,T> > {
                enum { space = SPACE };
                enum { value = SPACE };
        };

        // Normal
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <Normal<SPACE,T> > {
                enum { space = SPACE };
                enum { value = SPACE };
        };


        // Tests
        STATIC_ASSERT ((
                static_cast<int>(CARTESIAN)
                ==
                static_cast<int>(
                get_coordinate_space<normal_t<CARTESIAN,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Normal<SPHERICAL,int> >::space)
                ==
                static_cast<int>
                (get_coordinate_space<normal_t<SPHERICAL,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Normal<CARTESIAN,int> >::space)
                !=
                static_cast<int>
                (get_coordinate_space<normal_t<SPHERICAL,int> >::space)
        ));


        ///////////////////////////////////////////////////////////////////////
        // is_normal
        //   is_normal<T>::value = true, if T in {normal_t<>, Normal<>}
        //   is_normal<T>::value = false, otherwise
        ///////////////////////////////////////////////////////////////////////

        // ?
        template <typename T> struct is_normal : public false_value {};

        // normal_t
        template <typename T, coordinate_space_t SPACE>
        struct is_normal <normal_t<SPACE,T> > : public true_value {};

        // Normal
        template <typename T, coordinate_space_t SPACE>
        struct is_normal <Normal<SPACE,T> > : public true_value {};

        // Tests
        STATIC_ASSERT((is_normal<normal_t<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_normal<Normal<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_normal<normal_t<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_normal<Normal<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_normal<normal_t<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_normal<Normal<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_normal<normal_t<SPHERICAL,float> >::value));
        STATIC_ASSERT((is_normal<Normal<CARTESIAN,float> >::value));
        STATIC_ASSERT(!(is_normal<int>::value));
        STATIC_ASSERT(!(is_normal<int>::value));
        STATIC_ASSERT(!(is_normal<vector_t<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_normal<vector_t<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_normal<Point<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_normal<point_t<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_normal<fixed_point_t<int,3> >::value));
} }

#endif // NORMAL_POINT_TRAITS_H_20090221
