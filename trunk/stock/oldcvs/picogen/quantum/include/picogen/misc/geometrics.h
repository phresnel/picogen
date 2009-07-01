/***************************************************************************
 *            geometrics.h
 *
 *  Thu Oct 11 19:34:19 2007
 *  Copyright  2007  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _GEOMETRICS_H
#define _GEOMETRICS_H

namespace picogen {
        namespace geometrics {

            class Vector3d {
                private:
                    typedef ::picogen::real real;
                    real m[3];
                public:

                    template <class T> inline real &operator [] (T u) {
                        XRT_CALL_STACK_PUSH ("template <class T> inline real& vector3d::operator [] ( T u )");
#ifdef XRT_DEBUG
                        if (u < 0 || u > 2) throw exceptions::index_out_of_bounds (__FILE__,__LINE__,0,3,u);
#endif
                        XRT_CALL_STACK_POP();
                        return m[u]; // do not make any type conversion, so compiler-warnings are produced
                    }

                    template <class T> const inline real operator [] (T u) const {
                        XRT_CALL_STACK_PUSH ("template <class T> const inline real vector3d::operator [] ( T u ) const");
#ifdef XRT_DEBUG
                        if (u < 0 || u > 2) throw exceptions::index_out_of_bounds (__FILE__,__LINE__,0,3,u);
#endif
                        XRT_CALL_STACK_POP();
                        return m[u]; // do not make any type conversion, so compiler-warnings are produced
                    }

                    Vector3d (real X, real Y, real Z) {
                        XRT_CALL_STACK_PUSH ("vector3d::vector3d( real X, real Y, real Z )");
                        m[0] = X;
                        m[1] = Y;
                        m[2] = Z;
                        XRT_CALL_STACK_POP();
                    }

                    Vector3d() {
                        XRT_CALL_STACK_PUSH ("vector3d::vector3d()");
                        m[0] = m[1] = m[2] = 0;
                        XRT_CALL_STACK_POP();
                    }

                    inline const Vector3d operator = (const Vector3d &v) {
                        return Vector3d (m[0]=v.m[0], m[1]=v.m[1], m[2]=v.m[2]);
                    }

                    inline const Vector3d operator + (const Vector3d &v) const {
                        return Vector3d (m[0]+v.m[0], m[1]+v.m[1], m[2]+v.m[2]);
                    }

                    inline const Vector3d operator - (const Vector3d &v) const {
                        return Vector3d (m[0]-v.m[0], m[1]-v.m[1], m[2]-v.m[2]);
                    }

                    inline real operator * (const Vector3d &v) const {
                        return (m[0]*v.m[0] + m[1]*v.m[1] + m[2]*v.m[2]);
                    }

                    inline const Vector3d operator * (const real f) const {
                        return Vector3d (m[0]*f, m[1]*f, m[2]*f);
                    }

                    inline const Vector3d operator - () const {
                        return Vector3d (-m[0], -m[1], -m[2]);
                    }

                    inline const Vector3d operator + () const {
                        return *this;
                    }

                    inline const Vector3d computeCross (const Vector3d &v) const {
                        return Vector3d (
                                   m[1]*v.m[2] - m[2]*v.m[1],
                                   m[2]*v.m[0] - m[0]*v.m[2],
                                   m[0]*v.m[1] - m[1]*v.m[0]);
                    }

                    inline real computeLengthSq() const {
                        return m[0]*m[0] + m[1]*m[1] + m[2]*m[2];
                    }
                    inline real computeLength() const {
                        return sqrt (m[0]*m[0] + m[1]*m[1] + m[2]*m[2]);
                    }

                    inline const Vector3d computeNormal() const {
                        return *this * (1.0/length());
                    }

                    void computeCoordinateSystem (Vector3d &v1, Vector3d &v2, Vector3d &v3) const {
                        v1 = computeNormal();
                        // from PBRT
                        if (fabs (v1.m[0]) > fabs (v1.m[1])) {
                            real ilen = 1.0 / sqrt (v1.m[0]*v1.m[0] + v1.m[2]*v1.m[2]);
                            v2 = Vector3d (-v1.m[2] * ilen, 0.0, v1.m[0] * ilen);
                        } else {
                            real ilen = 1.0 / sqrt (v1.m[1]*v1.m[1] + v1.m[2]*v1.m[2]);
                            v2 = Vector3d (0.0, v1.m[2] * ilen, -v1.m[1] * ilen);
                        }
                        v3 = v1.computeCross (v2);
                    }

                    // --- below function are deprecated
                    /// \todo get rid of below function
                    inline Vector3d cross (const Vector3d &v) const {
                        return Vector3d (
                                   m[1]*v.m[2] - m[2]*v.m[1],
                                   m[2]*v.m[0] - m[0]*v.m[2],
                                   m[0]*v.m[1] - m[1]*v.m[0]);
                    }

                    /// \todo get rid of below function
                    inline real lengthSq() const {
                        return m[0]*m[0] + m[1]*m[1] + m[2]*m[2];
                    }

                    /// \todo get rid of below function
                    inline real length() const {
                        return sqrt (m[0]*m[0] + m[1]*m[1] + m[2]*m[2]);
                    }

                    /// \todo get rid of below function
                    inline Vector3d normal() const {
                        return *this * (1.0/length());
                    }
            };



            class Ray {
                private:
                    typedef ::picogen::real real;
                private:
                    Vector3d position, direction;
                public:
                    Ray()
                        : position(), direction()
                    {}

                    Ray (const Vector3d &position, const Vector3d &direction)
                        : position (position), direction (direction)
                    {}

                    inline const Vector3d operator () (real f) const {
                        return position + direction * f;
                    }

                    const inline Vector3d getPosition() const {
                        return position;
                    }

                    const inline Vector3d getDirection() const {
                        return direction;
                    }

                    void setPosition( const Vector3d &position ) {
                        this->position = position;
                    }

                    void setDirection( const Vector3d &direction ) {
                        this->direction = direction;
                    }

                    // --- below functions are deprecated

                    /// \todo get rid of below function
                    const inline Vector3d x() const {
                        return position;
                    }

                    /// \todo get rid of below function
                    const inline Vector3d w() const {
                        return direction;
                    }

                    /// \todo get rid of below function
                    inline Vector3d &x() {
                        return position;
                    }

                    /// \todo get rid of below function
                    inline Vector3d &w() {
                        return direction;
                    }
            };


            class BoundingBox {
                private:
                    Vector3d bbmin,bbmax;
                public:
                    BoundingBox() :
                            bbmin (constants::real_max,constants::real_max,constants::real_max),
                            bbmax (-constants::real_max,-constants::real_max,-constants::real_max) {
                    }

                    BoundingBox (const Vector3d & bbmin, const Vector3d & bbmax) :
                            bbmin (bbmin),
                            bbmax (bbmax) {
                    }

                    /*BoundingBox (Vector3d bbmin, Vector3d bbmax) :
                            bbmin (bbmin),
                            bbmax (bbmax) {
                    }*/

                    void reset() {
                        bbmin = Vector3d (constants::real_max,constants::real_max,constants::real_max);
                        bbmax = Vector3d (-constants::real_max,-constants::real_max,-constants::real_max);
                    }

                    int getAxisOfMaxExtent() const {
                        const Vector3d diff = bbmax - bbmin;
                        int ret = 0;
                        if (fabs (diff[1]) > fabs (diff[0]) && fabs (diff[1]) > fabs (diff[2])) ret = 1;
                        if (fabs (diff[2]) > fabs (diff[0]) && fabs (diff[2]) > fabs (diff[1])) ret = 2;
                        return ret;
                    }

                    const Vector3d computeMedian() const {
                        return (bbmin+bbmax) *0.5;
                    }

                    const Vector3d getMin() const {
                        return bbmin;
                    }

                    const Vector3d getMax() const {
                        return bbmax;
                    }

                    const void setMin (const Vector3d &b) {
                        bbmin = b;
                    }

                    const void setMax (const Vector3d &b) {
                        bbmax = b;
                    }

                    const real computeWidth () const {
                        return bbmax[0] - bbmin[0];
                    }

                    const real computeHeight () const {
                        return bbmax[1] - bbmin[1];
                    }

                    const real computeDepth () const {
                        return bbmax[2] - bbmin[2];
                    }

                    const real computeSize (unsigned int axis) const {
                        return bbmax[axis] - bbmin[axis];
                    }

                    const Vector3d computeCenter () const {
                        return (bbmin + bbmax) * 0.5;
                    }

                    void update (param_in (Vector3d, x)) {
                        if (x[0] < bbmin[0]) bbmin[0] = x[0];
                        if (x[1] < bbmin[1]) bbmin[1] = x[1];
                        if (x[2] < bbmin[2]) bbmin[2] = x[2];
                        if (x[0] > bbmax[0]) bbmax[0] = x[0];
                        if (x[1] > bbmax[1]) bbmax[1] = x[1];
                        if (x[2] > bbmax[2]) bbmax[2] = x[2];
                    }

                    bool intersects (param_in (Vector3d,x)) const {
                        return x[0]>=bbmin[0]
                               && x[1]>=bbmin[1]
                               && x[2]>=bbmin[2]
                               && x[0]<=bbmax[0]
                               && x[1]<=bbmax[1]
                               && x[2]<=bbmax[2]
                               ;
                    }

                    bool intersect (param_out (real,t_min), param_out (real,t_max), param_in (Ray,r)) const {
                        real tmin, tmax, tymin, tymax, tzmin, tzmax;

                        real inx = 1.0 / r.w() [0];
                        if (r.w() [0] >= 0) {
                            tmin = (bbmin[0] - r.x() [0]) * inx;
                            tmax = (bbmax[0] - r.x() [0]) * inx;
                        } else {
                            tmin = (bbmax[0] - r.x() [0]) * inx;
                            tmax = (bbmin[0] - r.x() [0]) * inx;
                        }

                        real iny = 1.0 / r.w() [1];
                        if (r.w() [1] >= 0) {
                            tymin = (bbmin[1] - r.x() [1]) * iny;
                            tymax = (bbmax[1] - r.x() [1]) * iny;
                        } else {
                            tymin = (bbmax[1] - r.x() [1]) * iny;
                            tymax = (bbmin[1] - r.x() [1]) * iny;
                        }

                        if ( (tmin > tymax)  || (tymin > tmax))
                            return false;

                        if (tymin > tmin)
                            tmin = tymin;
                        if (tymax < tmax)
                            tmax = tymax;

                        real inz = 1.0 / r.w() [2];
                        if (r.w() [2] >= 0) {
                            tzmin = (bbmin[2] - r.x() [2]) * inz;
                            tzmax = (bbmax[2] - r.x() [2]) * inz;
                        } else {
                            tzmin = (bbmax[2] - r.x() [2]) * inz;
                            tzmax = (bbmin[2] - r.x() [2]) * inz;
                        }

                        if ( (tmin > tzmax)  || (tzmin > tmax))
                            return false;
                        if (tzmin > tmin)
                            tmin = tzmin;
                        if (tzmax < tmax)
                            tmax = tzmax;

                        t_min = tmin;
                        t_max = tmax;

                        using namespace ::picogen::constants;
                        return t_min>epsilon || t_max > epsilon;
                    }
            };

        } // namespace geometrics {
} //namespace picogen {
#include "geometrics/Transformation.h"

#endif /* _GEOMETRICS_H */
