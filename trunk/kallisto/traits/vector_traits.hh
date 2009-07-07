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

#ifndef VECTOR_TRAITS_H_20090221
#define VECTOR_TRAITS_H_20090221

///////////////////////////////////////////////////////////////////////////////
// vector_t and Vector traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_non_pod_equivalent
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_non_pod_alternative;

        // vector_t
        template <typename T, coordinate_space_t SPACE>
        struct get_non_pod_alternative <vector_t<SPACE,T> >
        : public get_type<Vector<SPACE,T> > {};

        // Vector
        template <typename T, coordinate_space_t SPACE>
        struct get_non_pod_alternative <Vector<SPACE,T> >
        : public get_type<Vector<SPACE,T> > {};

        // Tests
        STATIC_ASSERT((types_equal<
                Vector<CARTESIAN,float>,
                get_non_pod_alternative<vector_t<CARTESIAN,float> >::type
        >::value));

        STATIC_ASSERT((types_equal<
                Vector<CARTESIAN,float>,
                get_non_pod_alternative<Vector<CARTESIAN,float> >::type
        >::value));

        STATIC_ASSERT((types_equal<
                Vector<SPHERICAL,fixed_point_t<int,16> >,
                get_non_pod_alternative<vector_t<SPHERICAL,
                        fixed_point_t<int,16> >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                Vector<SPHERICAL,float>,
                get_non_pod_alternative<Vector<SPHERICAL,float> >::type
        >::value));



        ///////////////////////////////////////////////////////////////////////
        // get_scalar_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_scalar_type;


        // vector_t
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <vector_t<SPACE,T> > : public get_type<T> {};

        // Vector
        template <typename T, coordinate_space_t SPACE>
        struct get_scalar_type <Vector<SPACE,T> > : public get_type<T> {};


        // Tests
        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<vector_t<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        vector_t<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                double,
                get_scalar_type<vector_t<SPHERICAL,double> >::type
        >::value));

        STATIC_ASSERT((types_equal<
                int,
                get_scalar_type<Vector<CARTESIAN,int> >::type
        >::value));
        STATIC_ASSERT((types_equal<
                fixed_point_t<int32_t, 0>,
                get_scalar_type<
                        Vector<CARTESIAN, fixed_point_t<int32_t, 0> >
                >::type
        >::value));
        STATIC_ASSERT((types_equal<
                get_scalar_type<Vector<CARTESIAN,double> >::type,
                get_scalar_type<vector_t<SPHERICAL,double> >::type
        >::value));
        STATIC_ASSERT(!(types_equal<
                get_scalar_type<Vector<CARTESIAN,float> >::type,
                get_scalar_type<vector_t<SPHERICAL,double> >::type
        >::value));

        ///////////////////////////////////////////////////////////////////////
        // get_coordinate_space
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_coordinate_space;

        // vector_t
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <vector_t<SPACE,T> > {
                enum { space = SPACE };
                enum { value = SPACE };
        };

        // Vector
        template <typename T, coordinate_space_t SPACE>
        struct get_coordinate_space <Vector<SPACE,T> > {
                enum { space = SPACE };
                enum { value = SPACE };
        };

        // Tests
        STATIC_ASSERT ((
                static_cast<int>(CARTESIAN)
                ==
                static_cast<int>(
                get_coordinate_space<vector_t<CARTESIAN,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Vector<SPHERICAL,int> >::space)
                ==
                static_cast<int>
                (get_coordinate_space<vector_t<SPHERICAL,int> >::space)
        ));
        STATIC_ASSERT ((
                static_cast<int>
                (get_coordinate_space<Vector<CARTESIAN,int> >::space)
                !=
                static_cast<int>
                (get_coordinate_space<vector_t<SPHERICAL,int> >::space)
        ));


        ///////////////////////////////////////////////////////////////////////
        // is_vector
        //   is_vector<T>::value = true, if T in {vector_t<>, Vector<>}
        //   is_vector<T>::value = false, otherwise
        ///////////////////////////////////////////////////////////////////////

        // ?
        template <typename T> struct is_vector : public false_value {};

        // vector_t
        template <typename T, coordinate_space_t SPACE>
        struct is_vector <vector_t<SPACE,T> > : public true_value {};

        // Vector
        template <typename T, coordinate_space_t SPACE>
        struct is_vector <Vector<SPACE,T> > : public true_value {};

        // Tests
        STATIC_ASSERT((is_vector<vector_t<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_vector<Vector<CARTESIAN,int> >::value));
        STATIC_ASSERT((is_vector<vector_t<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_vector<Vector<CARTESIAN,float> >::value));
        STATIC_ASSERT((is_vector<vector_t<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_vector<Vector<SPHERICAL,int> >::value));
        STATIC_ASSERT((is_vector<vector_t<SPHERICAL,float> >::value));
        STATIC_ASSERT((is_vector<Vector<CARTESIAN,float> >::value));
        //STATIC_ASSERT(!(is_vector<normal_t<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_vector<Normal<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_vector<Point<CARTESIAN,double> >::value));
        //STATIC_ASSERT(!(is_vector<point_t<CARTESIAN,double> >::value));
        STATIC_ASSERT(!(is_vector<int>::value));
        STATIC_ASSERT(!(is_vector<fixed_point_t<int,3> >::value));
} }

#endif // VECTOR_TRAITS_H_20090221
