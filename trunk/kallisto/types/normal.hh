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

#ifndef NORMAL_H__INCLUDED__20090210
#define NORMAL_H__INCLUDED__20090210

namespace kallisto {
        template <coordinate_space_t SPACE, typename T> class normal_t ;

        template <typename T>
        struct normal_t<CARTESIAN, T> {
                T x, y, z;
        };

        template <typename T>
        struct normal_t<SPHERICAL, T> {
                T azimuth; // phi
                T zenith;  // theta
                T radius;
        };
}

namespace kallisto {
        template <coordinate_space_t SPACE, typename T> class Normal ;
}

namespace kallisto {
        template <typename T>
        class Normal <CARTESIAN,T> {
        public: // Construct.
                T x, y, z;

                Normal ()
                : x(T()), y(T()), z(T()) {
                }

                Normal (Normal const &rhs)
                : x(rhs.x), y(rhs.y), z(rhs.z) {
                }

                Normal (T const &x, T const &y, T const &z)
                : x(x), y(y), z(z) {
                }

                inline Normal & operator= (Normal const &rhs) {
                        this->x = rhs.x;
                        this->y = rhs.y;
                        this->z = rhs.z;
                        return *this;
                }

                operator normal_t<CARTESIAN, T> () const {
                        const normal_t<CARTESIAN, T> ret = { x, y, z };
                        return ret;
                }

        public: // Friend injections.

                friend inline
                Normal operator- (Normal const & rhs) {
                        return Normal (-rhs.x, -rhs.y, -rhs.z);
                }

                friend inline
                Normal operator+ (Normal const & rhs) {
                        return rhs;
                }

                friend inline
                Normal operator+ (Normal const & lhs, Normal const & rhs) {
                        return Normal (lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
                }

                friend inline
                Normal operator- (Normal const & lhs, Normal const & rhs) {
                        return Normal (lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
                }

                friend inline
                T operator* (Normal const & lhs, Normal const & rhs) {
                        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
                }

                friend inline
                T dot (Normal const & lhs, Normal const & rhs) {
                        return lhs * rhs;
                }

                friend inline
                T absDot (Normal const & lhs, Normal const & rhs) {
                        return abs (lhs * rhs);
                }

                friend inline
                Normal operator* (Normal const & lhs, T const & rhs) {
                        return Normal (lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
                }

                friend inline
                Normal operator* (T const & lhs, Normal const & rhs) {
                        return Normal (lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
                }

                friend inline
                Normal operator/ (Normal const & lhs, T const & rhs) {
                        T const inv = 1.0 / rhs;
                        return Normal (lhs.x*inv, lhs.y*inv, lhs.z*inv);
                }

                friend inline T lengthSq (T const & v) {
                        return v * v;
                }

                friend inline T length (T const & v) {
                        return kallisto::sqrt (lengthSq (v));
                }

                friend inline Normal normalize (Normal const & v) {
                        return v / length (v);
                }
        };
}

namespace kallisto {
        template <typename T>
        class Normal <SPHERICAL,T> {
        };
}

namespace kallisto { namespace unit_test {
        class test_normal_t {
        private:
                const static test_normal_t test;
                test_normal_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }
#endif // NORMAL_H__INCLUDED__20090210
