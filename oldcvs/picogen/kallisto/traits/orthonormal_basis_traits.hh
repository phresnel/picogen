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

#ifndef ORTHONORMAL_BASIS_TRAITS_H_INCLUDED_20090223
#define ORTHONORMAL_BASIS_TRAITS_H_INCLUDED_20090223

namespace kallisto { namespace traits {
        ///////////////////////////////////////////////////////////////////////
        // get_vector_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct get_vector_type;

        // orthonormal_basis_t
        template <typename T>
        struct get_vector_type <orthonormal_basis_t<T> >
        : public get_type<T> {};

        // OrthonormalBasis
        template <typename T>
        struct get_vector_type <OrthonormalBasis<T> >
        : public get_type<T> {};


        // Tests
        STATIC_ASSERT((types_equal<
                vector_t<CARTESIAN,float>,
                get_vector_type<
                        orthonormal_basis_t<vector_t<CARTESIAN,float> >
                >::type
        >::value));

        STATIC_ASSERT((types_equal<
                vector_t<SPHERICAL,int>,
                get_vector_type<
                        OrthonormalBasis<vector_t<SPHERICAL,int> >
                >::type
        >::value));
} }

#endif // ORTHONORMAL_BASIS_TRAITS_H_INCLUDED_20090223
