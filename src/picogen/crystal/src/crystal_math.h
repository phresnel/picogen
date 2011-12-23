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

#include <boost/optional.hpp>

#include <memory>
#include <cmath>
#include <cassert>
#include <algorithm>

namespace crystal {
        using std::shared_ptr;
        using boost::optional;
}

namespace crystal {
        typedef float real;

        using std::sqrt;
        using std::pow;
        using std::fabs;
        using std::min;
        using std::max;
        using std::sin;
        using std::cos;


        template <typename T>
        inline T lerp (real const & alpha, T const & begin, T const & end)
        {
                return begin + alpha*(end-begin);
        }

        template <typename T>
        inline real ilerp (T const &value, T const &begin, T const &end)
        {
                return (value-begin) / (end-begin);
        }

        template <typename T>
        inline real ilerp_sat (T const &value, T const &begin, T const &end)
        {
                const real f = ilerp (value, begin, end);
                return f<0?0:f>1?1:f;
        }
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

                friend Radiance pow (Radiance lhs, real rhs);
                friend Radiance pow (Radiance lhs, Radiance rhs);
                friend Radiance operator / (real lhs, Radiance rhs);

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

        inline Radiance operator * (real lhs, Radiance rhs)
        {
                return rhs *= lhs;
        }

        inline Radiance operator / (Radiance lhs, real rhs)
        {
                return lhs /= rhs;
        }

        inline Radiance operator / (real lhs, Radiance rhs)
        {
                return Radiance(lhs / rhs.r_,
                                lhs / rhs.g_,
                                lhs / rhs.b_);
        }

        inline Radiance pow (Radiance lhs, real rhs)
        {
                return Radiance(pow (lhs.r_, rhs),
                                pow (lhs.g_, rhs),
                                pow (lhs.b_, rhs));
        }

        inline Radiance pow (Radiance lhs, Radiance rhs)
        {
                return Radiance(pow (lhs.r_, rhs.r_),
                                pow (lhs.g_, rhs.g_),
                                pow (lhs.b_, rhs.b_));
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

        inline Vector operator- (Vector const &rhs)
        {
                return {-rhs.x, -rhs.y, -rhs.z};
        }

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

        inline real length_sq (real x, real y, real z)
        {
                return x*x + y*y + z*z;
        }

        inline real length (real x, real y, real z)
        {
                return sqrt (length_sq (x,y,z));
        }

        template <typename Target=Vector>
        inline Target normalize (Vector const &v)
        {
                const real len = length(v);
                return {v.x/len, v.y/len, v.z/len};
        }

        template <typename Target=Vector>
        inline Target normalize (real x, real y, real z)
        {
                const real len = length(x,y,z);
                return {x/len, y/len, z/len};
        }

        inline Vector cross (Vector const &lhs, Vector const &rhs)
        {
                return {lhs.y*rhs.z - lhs.z*rhs.y,
                        lhs.z*rhs.x - lhs.x*rhs.z,
                        lhs.x*rhs.y - lhs.y*rhs.x};
        }

        inline real dot (Vector const &lhs, Vector const &rhs)
        {
                return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
        }



        class Point {
        public:
                real x, y, z;

                Point (real x, real y, real z) : x(x), y(y), z(z) {}
                Point () : x(0), y(0), z(0) {}

                real operator[] (int i) const {
                        switch (i) {
                        case 0: return x;
                        case 1: return y;
                        case 2: return z;
                        }
                        /* something */ //throw std:
                }
                real& operator[] (int i) {
                        switch (i) {
                        case 0: return x;
                        case 1: return y;
                        case 2: return z;
                        }
                        /* something */ //throw std:
                }

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

        inline Point lerp (Point const &lhs, Point const &rhs, real f)
        {
                return {
                        lhs.x*(1-f) + rhs.x*f,
                        lhs.y*(1-f) + rhs.y*f,
                        lhs.z*(1-f) + rhs.z*f
                };
        }

        inline Point min (Point const &lhs, Point const &rhs)
        {
                return {
                        lhs.x < rhs.x ? lhs.x : rhs.x,
                        lhs.y < rhs.y ? lhs.y : rhs.y,
                        lhs.z < rhs.z ? lhs.z : rhs.z
                };
        }

        inline Point max (Point const &lhs, Point const &rhs)
        {
                return {
                        lhs.x > rhs.x ? lhs.x : rhs.x,
                        lhs.y > rhs.y ? lhs.y : rhs.y,
                        lhs.z > rhs.z ? lhs.z : rhs.z
                };
        }

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




        class Normal;

        class Direction
        {
        public:
                Direction() = delete;

                Direction (real x, real y, real z) :
                        dir (normalize(Vector(x,y,z)))
                {
                        assert (x!=0 || y!=0 || z!=0);
                }

                static Direction FromNormalized (real x, real y, real z)
                {
                        return Direction (x,y,z, 0);
                }

                explicit operator Vector () const;
                explicit operator Normal () const;

                real x() const { return dir.x; }
                real y() const { return dir.y; }
                real z() const { return dir.z; }

                bool positive_x() const { return dir.x>=0; }
                bool positive_y() const { return dir.y>=0; }
                bool positive_z() const { return dir.z>=0; }

                friend Vector operator* (Direction const &lhs, real f)
                {
                        return {lhs.dir.x * f,
                                lhs.dir.y * f,
                                lhs.dir.z * f};
                }

                Direction operator- () const
                {
                        return {-dir.x, -dir.y, -dir.z};
                }

                friend real dot (Direction const &lhs, Direction const &rhs)
                {
                        return dot (lhs.dir, rhs.dir);
                }

        private:
                Vector dir;

                Direction (real x, real y, real z, void*) : dir (x,y,z)
                {
                        assert (x!=0 || y!=0 || z!=0);
                }
        };


        class Normal
        {
        public:
                Normal() = delete;

                Normal (real x, real y, real z) :
                        n (normalize(Vector(x,y,z)))
                {
                        assert (x!=0 || y!=0 || z!=0);
                }

                static Normal FromNormalized (real x, real y, real z)
                {
                        return Normal (x,y,z, 0);
                }

                explicit operator Vector    () const;
                explicit operator Direction () const;

                Normal operator- () const
                {
                        return {-n.x, -n.y, -n.z};
                }

                friend real dot (Normal const &lhs, Normal const &rhs)
                {
                        return dot (lhs.n, rhs.n);;
                }

                friend Normal cross (Normal lhs, Normal const &rhs)
                {
                        lhs.n = cross (lhs.n, rhs.n);
                        return lhs;
                }

                real x() const { return n.x; }
                real y() const { return n.y; }
                real z() const { return n.z; }
        private:

                Normal (real x, real y, real z, void*) : n (x,y,z)
                {
                        assert (x!=0 || y!=0 || z!=0);
                }

                Vector n;
        };


        inline Direction::operator Vector () const
        {
                return {dir.x, dir.y, dir.z};
        }

        inline Direction::operator Normal () const
        {
                return Normal::FromNormalized (dir.x, dir.y, dir.z);
        }


        inline Normal::operator Vector () const
        {
                return {n.x, n.y, n.z};
        }

        inline Normal::operator Direction () const
        {
                return Direction::FromNormalized (n.x, n.y, n.z);
        }

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

        inline Ray flip_direction(Ray const &ray)
        {
                return Ray (ray.origin, -ray.direction);
        }
}

namespace crystal {
        struct Intersection
        {
                real distance;
                Normal normal;
                Radiance color; // TODO: should be some real material/bxdf

                Intersection() = delete;

                Intersection (real distance, Normal const &normal)
                        : distance(distance), normal(normal), color(Radiance::White())
                {}
        };
        typedef optional<Intersection> PIntersection;
}



namespace crystal {

        class Interval {
        public:
                Interval() = delete;
                Interval (real min, real max)
                        : min(min), max(max)
                {
                        assert (min <= max);
                }

                real size() const { return max - min; }

                void merge (real f) {
                        if (f < min) min = f;
                        if (f > max) max = f;
                }

                real min, max;
        };

        typedef boost::optional<Interval> PInterval;

}



namespace crystal {

        class BoundingBox {
        public:
                // Use max() instead of infinity() to allow for integer types.
                BoundingBox ()
                : minimum_ (
                        std::numeric_limits<real>::max(),
                        std::numeric_limits<real>::max(),
                        std::numeric_limits<real>::max())
                , maximum_ (
                        -std::numeric_limits<real>::max(),
                        -std::numeric_limits<real>::max(),
                        -std::numeric_limits<real>::max())
                {
                }


                BoundingBox (Point const &center, real width, real height, real depth)
                        : minimum_(center - Vector(width/2,height/2,depth/2))
                        , maximum_(center + Vector(width/2,height/2,depth/2))
                {}


                BoundingBox (Point const & p)
                : minimum_ (p), maximum_ (p)
                {
                }



                BoundingBox (Point const & a, Point const & b)
                : minimum_ (crystal::min(a,b))
                , maximum_ (crystal::max(a,b))
                {
                }

                // New naming scheme.
                Point min () const { return minimum_; }
                Point max () const { return maximum_; }

                real width ()  const { return maximum_.x - minimum_.x; }
                real height () const { return maximum_.y - minimum_.y; }
                real depth ()  const { return maximum_.z - minimum_.z; }

                real size (unsigned int axis) const {
                        return maximum_[axis] - minimum_[axis];
                }
                real center (unsigned int axis) const {
                        return maximum_[axis]*real(0.5)
                             + minimum_[axis]*real(0.5);
                }
                Point center() const {
                        return lerp (minimum_, maximum_, 0.5);
                }

                void reset() {
                        *this = BoundingBox();
                }

                /*void translate (real x, real y, real z) {
                        minimum_.x += x;
                        minimum_.y += y;
                        minimum_.z += z;
                        maximum_.x += x;
                        maximum_.y += y;
                        maximum_.z += z;
                }*/

                real centerX() const {
                        return 0.5 * minimum_.x + 0.5 * maximum_.x;
                }
                real centerY() const {
                        return 0.5 * minimum_.y + 0.5 * maximum_.y;
                }
                real centerZ() const {
                        return 0.5 * minimum_.z + 0.5 * maximum_.z;
                }

                real minX() const { return minimum_.x; }
                real minY() const { return minimum_.y; }
                real minZ() const { return minimum_.z; }

                real maxX() const { return maximum_.x; }
                real maxY() const { return maximum_.y; }
                real maxZ() const { return maximum_.z; }

        private:
                Point minimum_, maximum_;

        };



        inline
        BoundingBox merge ( // <-- "union" was already occupied by someone.
                BoundingBox const &box,
                Point       const &p
        ) {
                return BoundingBox (min (box.min(), p),
                                    max (box.max(), p));
        }

        inline
        BoundingBox merge ( // <-- "union" was already occupied by someone.
                BoundingBox const &lhs,
                BoundingBox const &rhs
        ) {
                return BoundingBox (min (lhs.min(), rhs.min()),
                                    max (lhs.max(), rhs.max()));
        }

        inline
        bool overlap (BoundingBox const &lhs, BoundingBox const &rhs) {
                const Point &lmin = lhs.min(), &lmax = lhs.max(),
                            &rmin = rhs.min(), &rmax = rhs.max();
                const bool x  = (lmax.x >= rmin.y)
                              & (lmin.x <= rmax.x);
                const bool y  = (lmax.y >= rmin.y)
                              & (lmin.y <= rmax.y);
                const bool z  = (lmax.z >= rmin.z)
                              & (lmin.z <= rmax.z);
                return x & y & z;
        }


        inline
        bool inside (BoundingBox const &box, Point const &point) {
                const Point &min = box.min();
                const Point &max = box.max();

                const bool x  = (point.x >= min.x)
                              & (point.x <= max.x);
                const bool y  = (point.y >= min.y)
                              & (point.y <= max.y);
                const bool z  = (point.z >= min.z)
                              & (point.z <= max.z);
                return x & y & z;
        }



        inline real volume (BoundingBox const &box) {
                const Vector diff = box.max() - box.min();
                return diff.x * diff.y * diff.z;
        }


        // Returns the intersection of a ray with a bounding box. Note: As we
        // really define "Ray" as a Half-Ray with only positive distance sign,
        // only intersections with at least max>=0 are considered; if both, min and max
        // are negative, no intersection is returned; if only max>=0, then the interval
        // is max,max.
        inline PInterval intersect (Ray const & ray, BoundingBox const & box) {
                using std::swap;

                real t0 = 0;//ray.minT;
                real t1 = std::numeric_limits<real>::infinity();//ray.maxT;

                // X
                {
                        real
                                i = real(1) / ray.direction.x(),
                                near = (box.min().x - ray.origin.x) * i,
                                far  = (box.max().x - ray.origin.x) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }

                // Y
                {
                        real
                                i = real(1) / ray.direction.y(),
                                near = (box.min().y - ray.origin.y) * i,
                                far  = (box.max().y - ray.origin.y) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }

                // Z
                {
                        real
                                i = real(1) / ray.direction.z(),
                                near = (box.min().z - ray.origin.z) * i,
                                far  = (box.max().z - ray.origin.z) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }
                //if ((t0 < 0) & (t1 < 0)) return Interval::Optional();
                /*if (t0 < 0) {
                        //if (std::numeric_limits<real>::infinity() != t1)
                                return Interval(t1,t1);
                        //return Interval::Optional();
                }*/
                return Interval (t0, t1);
        }

        inline PInterval intersect (Ray const & ray,
                                    real left,  real bottom, real front,
                                    real right, real top,    real back)
        {
                using std::swap;

                real t0 = 0;//ray.minT;
                real t1 = std::numeric_limits<real>::infinity();//ray.maxT;

                // X
                {
                        real
                                        i = real(1) / ray.direction.x(),
                                        near = (left  - ray.origin.x) * i,
                                        far  = (right - ray.origin.x) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }

                // Y
                {
                        real
                                        i = real(1) / ray.direction.y(),
                                        near = (bottom - ray.origin.y) * i,
                                        far  = (top    - ray.origin.y) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }

                // Z
                {
                        real
                                        i = real(1) / ray.direction.z(),
                                        near = (front - ray.origin.z) * i,
                                        far  = (back  - ray.origin.z) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return PInterval();
                }
                //if ((t0 < 0) & (t1 < 0)) return Interval::Optional();
                /*if (t0 < 0) {
                        //if (std::numeric_limits<real>::infinity() != t1)
                                return Interval(t1,t1);
                        //return Interval::Optional();
                }*/
                return Interval (t0, t1);
        }

        inline bool does_intersect (Ray const & ray, BoundingBox const & box) {
                using std::swap;
                real t0 = 0;//ray.minT;
                real t1 = std::numeric_limits<real>::max();

                // X
                {
                        real i = real(1) / ray.direction.x();
                        real near = (box.min().x - ray.origin.x) * i;
                        real far  = (box.max().x - ray.origin.x) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }

                // Y
                {
                        real i = real(1) / ray.direction.y();
                        real near = (box.min().y - ray.origin.y) * i;
                        real far  = (box.max().y - ray.origin.y) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }

                // Z
                {
                        real i = real(1) / ray.direction.z();
                        real near = (box.min().z - ray.origin.z)         * i;
                        real far  = (box.max().z - ray.origin.z) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }
                return true;
        }

        inline bool does_intersect (Ray const & ray,
                                    real left, real bottom, real front,
                                    real right, real top, real back)
        {
                using std::swap;
                real t0 = 0;//ray.minT;
                real t1 = std::numeric_limits<real>::max();

                // X
                {
                        real i = real(1) / ray.direction.x();
                        real near = (left  - ray.origin.x) * i;
                        real far  = (right - ray.origin.x) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }

                // Y
                {
                        real i = real(1) / ray.direction.y();
                        real near = (bottom - ray.origin.y) * i;
                        real far  = (top    - ray.origin.y) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }

                // Z
                {
                        real i = real(1) / ray.direction.z();
                        real near = (front - ray.origin.z) * i;
                        real far  = (back  - ray.origin.z) * i;

                        if (near > far) swap (near, far);

                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;

                        if (t0 > t1) return false;
                }
                return true;
        }
}



#endif // CRYSTAL_MATH_H
