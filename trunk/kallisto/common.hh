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

#ifndef COMMON_H__INCLUDED_20090215
#define COMMON_H__INCLUDED_20090215

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <typeinfo>

#include <vector>

#include <limits>
#include <climits>

#include "testing/static_assert.hh"
#include "testing/unit_test.hh"


#include "kallisto.hh"

#include "metamath/bitwise.hh"


#include "traits/traits.hh"

#include "traits/float_traits.hh"
#include "traits/int_traits.hh"

#include "types/fixed_point.hh"
#include "traits/fixed_point_traits.hh"

#include "types/vector.hh"
#include "traits/vector_traits.hh"

#include "types/point.hh"
#include "traits/point_traits.hh"

#include "types/normal.hh"
#include "traits/normal_traits.hh"

#include "types/scalar_cast.hh"
#include "types/vector_cast.hh"

#include "types/proto_ray.hh"
#include "traits/proto_ray_traits.hh"

#include "types/ray.hh"
#include "traits/ray_traits.hh"

#include "types/ray_differential.hh"
#include "traits/ray_differential_traits.hh"

#include "types/orthonormal_basis.hh"
#include "traits/orthonormal_basis_traits.hh"

#include "types/aabb.hh"
//#include "traits/orthonormal_basis_traits.hh"

#include "types/sphere.hh"
#include "traits/sphere_traits.hh"

#include "rng/mt.hh"

#endif // COMMON_H__INCLUDED_20090215
