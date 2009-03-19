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

#ifndef VECTOR_H__INCLUDED__20090210
#define VECTOR_H__INCLUDED__20090210

namespace kallisto {
        template <coordinate_space_t SPACE, typename T> class vector_t ;

        template <typename T>
        struct vector_t<CARTESIAN, T> {
                T x, y, z;
        };

        template <typename T>
        struct vector_t<SPHERICAL, T> {
                T azimuth; // phi
                T zenith;  // theta
                T radius;
        };
}

namespace kallisto {
        template <coordinate_space_t SPACE, typename T> class Vector ;
}

namespace kallisto {
        template <typename T>
        class Vector <CARTESIAN,T> {
        public: // Construct.
                T x, y, z;

                Vector ()
                : x(T()), y(T()), z(T()) {
                }

                Vector (Vector const &rhs)
                : x(rhs.x), y(rhs.y), z(rhs.z) {
                }

                Vector (T const &x, T const &y, T const &z)
                : x(x), y(y), z(z) {
                }

                inline Vector & operator= (Vector const &rhs) {
                        this->x = rhs.x;
                        this->y = rhs.y;
                        this->z = rhs.z;
                        return *this;
                }

                operator vector_t<CARTESIAN, T> () const {
                        const vector_t<CARTESIAN, T> ret = { x, y, z };
                        return ret;
                }
                
                /*inline
                T operator* (Vector const & rhs) {
                        return this->x*rhs.x + this->y*rhs.y + this->z*rhs.z;
                }

                inline T lengthSq () {
                        return *this * *this;
                }
                
                inline T length () {
                        return kallisto::sqrt (this->lengthSq ());
                }*/

        public: // I would have liked to use friend injections (in fact I had
                // quite some, that were much nicer to read), but that
                // doesn't work without many using directives over namespaces.
        };
        
        
        template <typename T> inline
        Vector <CARTESIAN,T> operator- (Vector <CARTESIAN,T> const & rhs) {
                return Vector <CARTESIAN,T> (-rhs.x, -rhs.y, -rhs.z);
        }

        template <coordinate_space_t SPACE, typename T> inline
        Vector <SPACE,T> operator+ (Vector <SPACE,T> const & rhs) {
                return rhs;
        }

        template <typename T> inline
        Vector <CARTESIAN,T> operator+ (Vector <CARTESIAN,T> const & lhs, 
                                            Vector <CARTESIAN,T> const & rhs) {
                return Vector<CARTESIAN,T>(lhs.x+rhs.x, lhs.y+rhs.y,
                                                                  lhs.z+rhs.z);
        }

        template <typename T> inline
        Vector <CARTESIAN,T> operator- (Vector <CARTESIAN,T> const & lhs,
                                            Vector <CARTESIAN,T> const & rhs) {
                return Vector <CARTESIAN,T> (lhs.x-rhs.x, lhs.y-rhs.y,
                                                                  lhs.z-rhs.z);
        }

        template <typename T> inline
        T operator* (Vector <CARTESIAN,T> const & lhs, 
                                            Vector <CARTESIAN,T> const & rhs) {
                return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
        }
        
        template <typename T> inline
        Vector<CARTESIAN,T> cross (Vector <CARTESIAN,T> const & lhs, 
                                            Vector <CARTESIAN,T> const & rhs) {
                return Vector<CARTESIAN,T>(
                        lhs.y*rhs.z - lhs.z*rhs.y,
                        lhs.z*rhs.x - lhs.x*rhs.z,
                        lhs.x*rhs.y - lhs.y*rhs.x
                );
        }

        template <coordinate_space_t SPACE, typename T> inline
        T dot (Vector <SPACE,T> const & lhs, Vector <SPACE,T> const & rhs) {
                return lhs * rhs;
        }

        template <coordinate_space_t SPACE, typename T> inline
        T absDot (Vector <SPACE,T> const & lhs, Vector <SPACE,T> const & rhs) {
                return abs (lhs * rhs);
        }

        template <typename T> inline
        Vector <CARTESIAN,T> operator* (Vector <CARTESIAN,T> const & lhs, 
                                                               T const & rhs) {
                return Vector <CARTESIAN,T> (lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
        }

        template <typename T> inline
        Vector <CARTESIAN,T> operator* (T const & lhs,
                                            Vector <CARTESIAN,T> const & rhs) {
                return Vector <CARTESIAN,T> (lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
        }

        template <typename T> inline
        Vector <CARTESIAN,T> operator/ (Vector <CARTESIAN,T> const & lhs,
                                                               T const & rhs) {
                T const inv = 1.0 / rhs;
                return Vector <CARTESIAN,T> (lhs.x*inv, lhs.y*inv, lhs.z*inv);
        }

        template <coordinate_space_t SPACE, typename T> inline
        T lengthSq (Vector<SPACE,T> const & v) {
                return v * v;
        }

        template <coordinate_space_t SPACE, typename T> inline
        T length (Vector<SPACE,T> const & v) {
                return kallisto::sqrt (lengthSq (v));
        }

        //template <typename T> inline T normalize (T const & v);        
        template <coordinate_space_t SPACE, typename T> inline
        Vector <SPACE,T> normalize (Vector <SPACE,T> const & v) {
                return v / length (v);
        }
}

namespace kallisto {
        template <typename T>
        class Vector <SPHERICAL,T> {
        };
}

namespace kallisto { namespace unit_test {
        class test_vector_t {
        private:
                const static test_vector_t test;
                test_vector_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // VECTOR_H__INCLUDED__20090210
