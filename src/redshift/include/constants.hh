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

#ifndef CONSTANTS_HH_INCLUDED_20100701
#define CONSTANTS_HH_INCLUDED_20100701

#include "redshift-kallisto.hh"
namespace redshift {
        namespace static_config {
                enum { debug = 0 }; // should be set by #define macro
        }

        namespace constants {

                // TODO clean up epsilon value
                template <typename> struct epsilon_value;
                template <> struct epsilon_value<float> {
                        static float const value;
                };
                template <> struct epsilon_value<double> {
                        static double const value;
                };
                template <> struct epsilon_value<long double> {
                        static long double const value;
                };


                // TODO clean up infinity value
                template <typename> struct infinity_value;
                template <> struct infinity_value<float> {
                        static float const value;
                };
                template <> struct infinity_value<double> {
                        static double const value;
                };
                template <> struct infinity_value<long double> {
                        static long double const value;
                };

                template <typename> struct real_max_value;
                template <> struct real_max_value<float> {
                        static float const value;
                };
                template <> struct real_max_value<double> {
                        static double const value;
                };
                template <> struct real_max_value<long double> {
                        static long double const value;
                };


                // TODO clean up pi value
                template <typename> struct pi_value;
                template <> struct pi_value<float> {
                        static float const value;
                };
                template <> struct pi_value<double> {
                        static double const value;
                };
                template <> struct pi_value<long double> {
                        static long double const value;
                };


                extern real_t const epsilon;
                extern real_t const real_max;
                extern real_t const pi;
                extern real_t const inv_pi;
                extern real_t const infinity;

                template<typename T> inline T km2m (T const &v) {
                        return T(1000) * v;
                }

                extern real_t const earth_radius_m;
                extern real_t const sun_radius_m;
                extern real_t const jupiter_radius_m;

                extern real_t const zero;
                extern real_t const one;
                extern real_t const two;
        }
}

#endif // CONSTANTS_HH_INCLUDED_20100701
