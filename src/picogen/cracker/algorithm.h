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

#ifndef ALGORITHM_HH_INCLUDED_20110726
#define ALGORITHM_HH_INCLUDED_20110726

namespace picogen { namespace cracker {
        template <typename T> inline T min (T const &lhs, T const &rhs) {
                return lhs < rhs ? lhs : rhs;
        }

        template <typename T> inline T max (T const &lhs, T const &rhs) {
                return lhs > rhs ? lhs : rhs;
        }

        template <typename T>
        inline T lerp (
                T const & alpha, T const & begin, T const & end
        ) {
                return alpha*(end-begin) + begin;
        }
} }

//#include "geometry.hh"
#include <tuple>
#include "constants.h"
#include "math3d.h"
namespace picogen { namespace cracker {
        namespace detail {
                inline Normal coordinateSystem_v2 (const Normal &normal) {
                        if (fabs (normal.x()) > fabs (normal.y())) {
                                const real invLen = 1.f / sqrtf (normal.x()*normal.x()
                                                               + normal.z()*normal.z());
                                return normalize<Normal> (-normal.z()*invLen,
                                                          0.f,
                                                          normal.x()*invLen);
                        } else {
                                const real invLen = 1.f / sqrtf (normal.y()*normal.y()
                                                               + normal.z()*normal.z());
                                return normalize<Normal> (0.f,
                                                          normal.z()*invLen,
                                                          -normal.y()*invLen);
                        }
                }
        }
        inline std::tuple<Normal,Normal,Normal>
                coordinateSystem (const Normal &normal)
        {
                auto v2 = detail::coordinateSystem_v2 (normal);
                auto v3 = cross (normal, v2);
                return std::make_tuple (v2, normal, v3);
        }

        /*
        inline Vector reflect (const Vector &in, const Normal &n) {
                const real f = dot(vector_cast<Vector>(n),in) * 2.f;
                return in - vector_cast<Vector>(n*f);
        }
        */
} }

#endif // ALGORITHM_HH_INCLUDED_20110726
