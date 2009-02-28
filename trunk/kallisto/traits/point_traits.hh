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

#ifndef POINT_TRAITS_H_20090221
#define POINT_TRAITS_H_20090221

///////////////////////////////////////////////////////////////////////////////
// point_t and Point traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_scalar_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_scalar_type;


        // point_t
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <point_t<SPACE,T> > : public get_type<T> {};

        // Point
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <Point<SPACE,T> > : public get_type<T> {};


        // Tests
        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<point_t<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        point_t<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                double,
                get_scalar_type<point_t<SPHERICAL,double> >::type
        >::value));

        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<Point<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        Point<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                get_scalar_type<Point<CARTESIAN,double> >::type,
                get_scalar_type<point_t<SPHERICAL,double> >::type
        >::value));
        STATIC_ASSERT(!(types_equal<
                get_scalar_type<Point<CARTESIAN,float> >::type,
                get_scalar_type<point_t<SPHERICAL,double> >::type
        >::value));


        ///////////////////////////////////////////////////////////////////////
        // get_coordinate_space
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_coordinate_space;

        // point_t
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <point_t<SPACE,T> > {
                enum { space = SPACE };
        };

        // Point
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <Point<SPACE,T> > {
                enum { space = SPACE };
        };


        // Tests
        STATIC_ASSERT ((
                static_cast<int>(CARTESIAN)
                ==
                static_cast<int>(
                get_coordinate_space<point_t<CARTESIAN,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Point<SPHERICAL,int> >::space)
                ==
                static_cast<int>
                (get_coordinate_space<point_t<SPHERICAL,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Point<CARTESIAN,int> >::space)
                !=
                static_cast<int>
                (get_coordinate_space<point_t<SPHERICAL,int> >::space)
        ));


        ///////////////////////////////////////////////////////////////////////
        // is_point
        //   is_point<T>::value = true, if T in {point_t<>, Point<>}
        //   is_point<T>::value = false, otherwise
        ///////////////////////////////////////////////////////////////////////

        // ?
        template <typename T> struct is_point : public false_value {};

        // point_t
        template <typename T, coordinate_space_t SPACE>
        struct is_point <point_t<SPACE,T> > : public true_value {};

        // Point
        template <typename T, coordinate_space_t SPACE>
        struct is_point <Point<SPACE,T> > : public true_value {};

        // Tests
        STATIC_ASSERT((is_point<point_t<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_point<Point<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_point<point_t<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_point<Point<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_point<point_t<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_point<Point<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_point<point_t<SPHERICAL,float> >::value));
        STATIC_ASSERT((is_point<Point<CARTESIAN,float> >::value));
        //STATIC_ASSERT(!(is_point<vector_t<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_point<vector_t<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_point<Normal<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_point<normal_t<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_point<int>::value));
        STATIC_ASSERT(!(is_point<fixed_point_t<int,3> >::value));
} }

#endif // #define POINT_TRAITS_H_20090221
