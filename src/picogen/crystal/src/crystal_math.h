//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#ifndef CRYSTAL_MATH_H
#define CRYSTAL_MATH_H


#include <cmath>
#include <cassert>

namespace crystal {
        typedef float real;

        using std::sqrt;
        using std::pow;
        using std::fabs;
}

namespace crystal {
        struct rgb_t {
                real r, g, b; // 0=black, 1=white, outside values allowed
        };
}


namespace crystal {
        class Radiance {
        public:
                static Radiance FromRgb (real r, real g, real b)
                {
                        return Radiance(r,g,b);
                }

                static Radiance Gray (real f)
                {
                        return Radiance(f,f,f);
                }

                static Radiance Black ()
                {
                        return Gray(0);
                }

                static Radiance White ()
                {
                        return Gray(1);
                }

                explicit operator rgb_t() const
                {
                        return { r_, g_, b_ };
                }

                Radiance& operator += (Radiance const &rhs)
                {
                        r_ += rhs.r_;
                        g_ += rhs.g_;
                        b_ += rhs.b_;
                        return *this;
                }

                Radiance& operator -= (Radiance const &rhs)
                {
                        r_ -= rhs.r_;
                        g_ -= rhs.g_;
                        b_ -= rhs.b_;
                        return *this;
                }

                Radiance& operator *= (Radiance const &rhs)
                {
                        r_ *= rhs.r_;
                        g_ *= rhs.g_;
                        b_ *= rhs.b_;
                        return *this;
                }

                Radiance& operator /= (Radiance const &rhs)
                {
                        r_ /= rhs.r_;
                        g_ /= rhs.g_;
                        b_ /= rhs.b_;
                        return *this;
                }

                Radiance& operator *= (real rhs)
                {
                        r_ *= rhs;
                        g_ *= rhs;
                        b_ *= rhs;
                        return *this;
                }

                Radiance& operator /= (real rhs)
                {
                        r_ /= rhs;
                        g_ /= rhs;
                        b_ /= rhs;
                        return *this;
                }

        private:
                Radiance (real r, real g, real b) : r_(r), g_(g), b_(b)
                {
                }
                real r_, g_, b_;
        };

        inline Radiance operator + (Radiance lhs, Radiance const &rhs)
        {
                return lhs += rhs;
        }

        inline Radiance operator - (Radiance lhs, Radiance const &rhs)
        {
                return lhs -= rhs;
        }

        inline Radiance operator * (Radiance lhs, Radiance const &rhs)
        {
                return lhs *= rhs;
        }

        inline Radiance operator / (Radiance lhs, Radiance const &rhs)
        {
                return lhs /= rhs;
        }

        inline Radiance operator * (Radiance lhs, real rhs)
        {
                return lhs *= rhs;
        }

        inline Radiance operator / (Radiance lhs, real rhs)
        {
                return lhs /= rhs;
        }
}


namespace crystal {
        struct CameraSample {
        public:
                real imageX, imageY; // [0..height), [0..width)
                real lensU, lensV;   // [0..1), [0..1)

                CameraSample (real imageX, real imageY, real lensU, real lensV)
                        : imageX(imageX), imageY(imageY)
                        , lensU(lensU), lensV(lensV)
                {
                }
        };
}

namespace crystal {
        class Vector {
        public:
                real x, y, z;

                Vector (real x, real y, real z) : x(x), y(y), z(z) {}
                Vector () : x(0), y(0), z(0) {}

                Vector& operator+= (Vector const &rhs) {
                        x += rhs.x;
                        y += rhs.y;
                        z += rhs.z;
                        return *this;
                }

                Vector& operator-= (Vector const &rhs) {
                        x -= rhs.x;
                        y -= rhs.y;
                        z -= rhs.z;
                        return *this;
                }

                Vector& operator*= (real rhs) {
                        x *= rhs;
                        y *= rhs;
                        z *= rhs;
                        return *this;
                }

                Vector& operator/= (real rhs) {
                        x /= rhs;
                        y /= rhs;
                        z /= rhs;
                        return *this;
                }
        };

        inline Vector operator+ (Vector lhs, Vector const &rhs)
        {
                return lhs += rhs;
        }

        inline Vector operator- (Vector lhs, Vector const &rhs)
        {
                return lhs -= rhs;
        }

        inline Vector operator* (Vector lhs, real rhs)
        {
                return lhs *= rhs;
        }

        inline Vector operator/ (Vector lhs, real rhs)
        {
                return lhs /= rhs;
        }

        inline Vector operator* (real lhs, Vector const &rhs)
        {
                return {lhs*rhs.x, lhs*rhs.y, lhs*rhs.z };
        }

        inline Vector operator/ (real lhs, Vector const &rhs)
        {
                return {lhs/rhs.x, lhs/rhs.y, lhs/rhs.z };
        }

        inline real length_sq (Vector const &v)
        {
                return v.x*v.x + v.y*v.y + v.z*v.z;
        }

        inline real length (Vector const &v)
        {
                return sqrt (length_sq (v));
        }

        inline Vector normalize (Vector const &v)
        {
                const real len = length(v);
                return {v.x/len, v.y/len, v.z/len};
        }



        class Point {
        public:
                real x, y, z;

                Point (real x, real y, real z) : x(x), y(y), z(z) {}
                Point () : x(0), y(0), z(0) {}

                Point& operator+= (Vector const &rhs)
                {
                        x+=rhs.x;
                        y+=rhs.y;
                        z+=rhs.z;
                        return *this;
                }

                Point& operator-= (Vector const &rhs)
                {
                        x-=rhs.x;
                        y-=rhs.y;
                        z-=rhs.z;
                        return *this;
                }
        };

        inline Point operator+ (Point lhs, Vector const& rhs)
        {
                return lhs+=rhs;
        }

        inline Point operator- (Point lhs, Vector const& rhs)
        {
                return lhs-=rhs;
        }

        inline Vector operator- (Point const &lhs, Point const &rhs)
        {
                return {lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z};
        }




        class Direction
        {
        public:
                Direction() = delete;

                Direction (real x, real y, real z) :
                        dir (normalize(Vector(x,y,z)))
                {
                        assert (x!=0 || y!=0 || z!=0);
                }

                friend Vector operator* (Direction const &lhs, real f)
                {
                        return {lhs.dir.x * f,
                                lhs.dir.y * f,
                                lhs.dir.z * f};
                }
        private:
                Vector dir;
        };
}

namespace crystal {
        class Ray
        {
        public:
                Point     origin;
                Direction direction;

                Ray (Point const &origin, Direction const &direction)
                        : origin(origin), direction(direction)
                {
                }

                Point operator() (real f) const
                {
                        return origin + direction*f;
                }
        };
}

#endif // CRYSTAL_MATH_H
