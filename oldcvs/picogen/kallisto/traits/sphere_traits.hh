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

#ifndef SPHERE_TRAITS_H_20090223
#define SPHERE_TRAITS_H_20090223

///////////////////////////////////////////////////////////////////////////////
// sphere_t and Ray traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_position_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_center_type;


        // sphere_t
        template <typename center_t, typename radius_t>
        struct get_center_type <sphere_t<center_t, radius_t> >
        : public get_type<center_t> {};

        // Sphere
        template <typename center_t, typename radius_t>
        struct get_center_type <Sphere<center_t, radius_t> >
        : public get_type<center_t> {};


        // Tests
        STATIC_ASSERT((types_equal<
                vector_t<CARTESIAN,int>,
                get_center_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));

        STATIC_ASSERT(!(types_equal<
                vector_t<CARTESIAN,float>,
                get_center_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                get_center_type<
                        Sphere<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type,
                get_center_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));

        STATIC_ASSERT(!(types_equal<
                vector_t<CARTESIAN,float>,
                get_center_type<
                        Sphere<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));


        ///////////////////////////////////////////////////////////////////////
        // get_direction_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_radius_type;


        // sphere_t
        template <typename center_t, typename radius_t>
        struct get_radius_type <sphere_t<center_t, radius_t> >
        : public get_type<radius_t> {};


        // Sphere
        template <typename center_t, typename radius_t>
        struct get_radius_type <Sphere<center_t, radius_t> >
        : public get_type<radius_t> {};


        // Tests
        STATIC_ASSERT((types_equal<
                float,
                get_radius_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                fixed_point_t<int,16>,
                get_radius_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                fixed_point_t<int,16>
                        >
                >::type
        >::value));

         STATIC_ASSERT((types_equal<
                float,
                get_radius_type<
                        Sphere<
                                vector_t<CARTESIAN,int>,
                                float
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                get_radius_type<
                        sphere_t<
                                vector_t<CARTESIAN,int>,
                                fixed_point_t<int,16>
                        >
                >::type,
                get_radius_type<
                        Sphere<
                                vector_t<CARTESIAN,int>,
                                fixed_point_t<int,16>
                        >
                >::type
        >::value));

} }

#endif // #define SPHERE_TRAITS_H_20090223
