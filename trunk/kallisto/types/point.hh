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

#ifndef POINT_H__INCLUDED__20090210
#define POINT_H__INCLUDED__20090210

namespace kallisto {

        template <coordinate_space_t SPACE, typename T> class point_t ;

        template <typename T>
        struct point_t<CARTESIAN, T> {
                T x, y, z;
        };

        template <typename T>
        struct point_t<SPHERICAL, T> {
                T azimuth; // phi
                T zenith;  // theta
                T radius;
        };
}

namespace kallisto {
        template <coordinate_space_t SPACE, typename T> class Point ;
}

namespace kallisto {
        template <typename T>
        class Point <CARTESIAN,T> {
        public: // Construct.
                T x, y, z;

                Point ()
                : x(T()), y(T()), z(T()) {
                }

                Point (Point const &rhs)
                : x(rhs.x), y(rhs.y), z(rhs.z) {
                }

                Point (T const &x, T const &y, T const &z)
                : x(x), y(y), z(z) {
                }

                inline Point & operator= (Point const &rhs) {
                        this->x = rhs.x;
                        this->y = rhs.y;
                        this->z = rhs.z;
                        return *this;
                }

                operator point_t<CARTESIAN, T> () const {
                        const point_t<CARTESIAN, T> ret = { x, y, z };
                        return ret;
                }

        public: // Friend injections.

                // vector = point - point
                // Left for reference: Below thing is useless, as ADL won't
                // take the return value into account:
                // *template <coordinate_space_t S> friend inline
                // *Vector<S, T> operator- (Point const &lhs, Point const &rhs)


                // vector = point - point
                friend inline Vector<CARTESIAN, T>
                operator- (Point const &lhs, Point const &rhs) {
                        return Vector<CARTESIAN, T> (
                                lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z
                        );
                }

                // point = point + vector
                friend inline
                Point operator+ (Point const &lhs, 
                                              Vector<CARTESIAN,T> const &rhs) {
                        return Point (lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
                }

                // point = point - vector
                friend inline
                Point operator- (Point const &lhs, 
                                              Vector<CARTESIAN,T> const &rhs) {
                        return Point (lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
                }

                // I think "point = vector [-+] point" would not make sense, as
                // as adding a *position* to a *direction* sounds crazy.


                // To allow for weighted sums.
                // Disallowed until *really* needed. Restrict first!
                /*
                friend inline
                T operator* (Vector const & lhs, Vector const & rhs) {
                        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
                }


                friend inline
                Point operator* (Vector const & lhs, T const & rhs) {
                        return Point (lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
                }

                friend inline
                Point operator* (T const & lhs, Vector const & rhs) {
                        return Point (lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
                }
                */

                friend inline T distSq (Point const &lhs, Point const &rhs) {
                        return lengthSq (lhs - rhs);
                }

                friend inline T dist (Point const &lhs, Point const &rhs) {
                        return length (lhs - rhs);
                }
        };
}

namespace kallisto {
        template <typename T>
        class Point <SPHERICAL,T> {
        };
}

namespace kallisto { namespace unit_test {
        class test_point_t {
        private:
                template <typename T> void run (T const &epsilon) {
                        std::string log = std::string("(for basetype ") +
                                        std::string(typeid(T).name()) + ")";

                        typedef kallisto::Point<CARTESIAN,T> point_t;
                        typedef kallisto::Vector<CARTESIAN,T> vector_t;
                        {
                                const point_t a (1, 1, 1);
                                const vector_t b = a - a;
                                if (b.x != 0 || b.y != 0 || b.z != 0) {
                                        UNIT_TEST_REPORT_FAIL(log);
                                }
                        }
                        {
                                const point_t a (1, 1, 1);
                                const vector_t b (-1, -1, -1);
                                const point_t c = a + b;
                                if (!cmpz_epsilon(c.x,epsilon)
                                 || !cmpz_epsilon(c.y,epsilon)
                                 || !cmpz_epsilon(c.z,epsilon)
                                ) {
                                        UNIT_TEST_REPORT_FAIL(log);
                                }
                        }
                        {
                                const point_t a (1, 1, 1);
                                const vector_t b (-1, -1, -1);
                                const point_t c = a - b;
                                if (!cmp_epsilon (c.x,T(2),epsilon)
                                 || !cmp_epsilon (c.y,T(2),epsilon)
                                 || !cmp_epsilon (c.z,T(2),epsilon)
                                ) {
                                        UNIT_TEST_REPORT_FAIL(log);
                                }
                        }
                        {
                                const point_t a (1500, 1000, -500);
                                const vector_t b (55, -77, 1050);
                                const point_t c = a - b;
                                if (!cmp_epsilon (c.x,T(1445),epsilon)
                                 || !cmp_epsilon (c.y,T(1077),epsilon)
                                 || !cmp_epsilon (c.z,T(-1550),epsilon)
                                ) {
                                        UNIT_TEST_REPORT_FAIL(log);
                                }
                        }
                        {
                                const point_t a (static_cast<T>(150001.4),
                                                static_cast<T>(100000),
                                                static_cast<T>(-50000));
                                const vector_t b (static_cast<T>(5502.4),
                                                static_cast<T>(-7700),
                                                static_cast<T>(105000));
                                const point_t c = a + b;
                                if (!cmp_epsilon (c.x,static_cast<T>(155503.8),
                                                                       epsilon)
                                 || !cmp_epsilon (c.y,static_cast<T>(92300),
                                                                       epsilon)
                                 || !cmp_epsilon (c.z,static_cast<T>(55000),
                                                                       epsilon)
                                ) {
                                        UNIT_TEST_REPORT_FAIL(log);
                                }
                        }
                }

                const static test_point_t test;
                test_point_t () {
                        using namespace std;
                        UNIT_TEST_REPORT_BEGIN("");
                        run<int> (0);
                        run<float> (0.02f);
                        run<double> (0.001);
                        run<long double> (0.001l);
                }
        };
} }

#endif // POINT_H__INCLUDED__20090210
