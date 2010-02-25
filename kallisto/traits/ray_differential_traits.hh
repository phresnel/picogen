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

#ifndef RAYDIFFERENTIAL_TRAITS_H_20090311
#define RAYDIFFERENTIAL_TRAITS_H_20090311

///////////////////////////////////////////////////////////////////////////////
// ray_differential_t and RayDifferential traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // get_position_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_position_type;


        // ray_differential_t
        template <typename point_t, typename direction_t>
        struct get_position_type <ray_differential_t<point_t, direction_t> >
        : public get_type<point_t> {};

        // RayDifferential
        template <typename point_t, typename direction_t>
        struct get_position_type <RayDifferential<point_t, direction_t> >
        : public get_type<point_t> {};


        // Tests
        STATIC_ASSERT((types_equal<
                vector_t<CARTESIAN,int>,
                get_position_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type
        >::value));

        STATIC_ASSERT(!(types_equal<
                vector_t<CARTESIAN,float>,
                get_position_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                get_position_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type,
                get_position_type<
                        RayDifferential<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type
        >::value));


        ///////////////////////////////////////////////////////////////////////
        // get_direction_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_direction_type;


        // ray_differential_t
        template <typename point_t, typename direction_t>
        struct get_direction_type <ray_differential_t<point_t, direction_t> >
        : public get_type<direction_t> {};


        // RayDifferential
        template <typename point_t, typename direction_t>
        struct get_direction_type <RayDifferential<point_t, direction_t> >
        : public get_type<direction_t> {};


        // Tests
        STATIC_ASSERT((types_equal<
                vector_t<CARTESIAN,float>,
                get_direction_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                vector_t<CARTESIAN,long double>,
                get_direction_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,long double>
                        >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                get_direction_type<
                        ray_differential_t<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type,
                get_direction_type<
                        RayDifferential<
                                vector_t<CARTESIAN,int>,
                                vector_t<CARTESIAN,float>
                        >
                >::type
        >::value));

} }

#endif // RAYDIFFERENTIAL_TRAITS_H_20090311
