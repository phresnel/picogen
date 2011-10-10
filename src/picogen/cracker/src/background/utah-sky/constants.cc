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

#include <limits>
#include "constants.hh"

namespace picogen { namespace redshift { namespace constants {

        float const epsilon_value<float>::value = 0.01f;
        double const epsilon_value<double>::value = 0.01;
        long double const epsilon_value<long double>::value = 0.01L;

        // TODO: use infinity
        float const infinity_value<float>::value = std::numeric_limits<float>::infinity();
        double const infinity_value<double>::value = std::numeric_limits<double>::infinity();
        long double const infinity_value<long double>::value = std::numeric_limits<long double>::infinity();

        float const real_max_value<float>::value = std::numeric_limits<float>::max();
        double const real_max_value<double>::value = std::numeric_limits<double>::max();
        long double const real_max_value<long double>::value = std::numeric_limits<long double>::max();

        float const pi_value<float>::value = 3.141592654f;
        double const pi_value<double>::value = 3.141592654;
        long double const pi_value<long double>::value = 3.141592654L;

        real_t const epsilon = epsilon_value<real_t>::value;
        real_t const infinity = infinity_value<real_t>::value;
        real_t const pi = pi_value<real_t>::value;
        real_t const inv_pi = 1. / pi_value<real_t>::value;
        real_t const real_max = real_max_value<real_t>::value;

        real_t const earth_radius_m   = static_cast<real_t>(km2m (12756.0));
        real_t const sun_radius_m = static_cast<real_t>(km2m (1.392 * 10.0e9));
        real_t const jupiter_radius_m = static_cast<real_t>(km2m (133708.0));

        real_t const zero = static_cast<real_t>(0);
        real_t const one = static_cast<real_t>(1);
        real_t const two = static_cast<real_t>(2);
} } }
