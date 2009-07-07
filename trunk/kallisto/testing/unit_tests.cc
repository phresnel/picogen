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

#include "common.hh"

namespace kallisto { namespace unit_test {
        test_fixed_point_t const test_fixed_point_t::test;
        test_normal_t const test_normal_t::test;
        test_point_t const test_point_t::test;
        test_vector_t const test_vector_t::test;

	test_ray_t const test_ray_t::test;
	test_proto_ray_t const test_proto_ray_t::test;
	test_proto_ray_differential_t const test_proto_ray_differential_t::test;
        test_sphere_t const test_sphere_t::test;
        test_bounding_box_t const test_bounding_box_t::test;
        test_orthonormal_basis_t const test_orthonormal_basis_t::test;

        test_scalar_cast const test_scalar_cast::test;
        test_vector_cast const test_vector_cast::test;

        test_traits const test_traits::test;
} }
