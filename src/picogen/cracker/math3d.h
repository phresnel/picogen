#ifndef MATH3D_H_20110813
#define MATH3D_H_20110813

#include "real.h"
#include "math3d/vector.h"
#include "math3d/point.h"
#include "math3d/direction.h"
#include "math3d/normal.h"

namespace picogen { namespace cracker {
        // any, any
        template <typename L, typename R>
        inline real mixed_dot (L const &lhs, R const &rhs) {
                return lhs.x()*rhs.x() +
                       lhs.y()*rhs.y() +
                       lhs.z()*rhs.z();
        }

        inline real length_squared (Vector const &v) {
                return v.x()*v.x() + v.y()*v.y() + v.z()*v.z();
        }

        inline real length (Vector const &v) {
                return sqrt (length_squared (v));
        }

        //
        template <typename Out=Vector>
        inline Out normalize (Vector const &in) {
                const real l = 1 / length (in);
                return static_cast<Out>(in * l);
        }


        inline Vector cross(Vector const &lhs, Vector const &rhs) {
                return Vector(lhs.y()*rhs.z() - lhs.z()*rhs.y(),
                              lhs.z()*rhs.x() - lhs.x()*rhs.z(),
                              lhs.x()*rhs.y() - lhs.y()*rhs.x()
                             );
        }

        // vector, point
        inline Vector operator- (Point const &lhs, Point const &rhs) {
                return Vector(lhs.x()-rhs.x(),
                              lhs.y()-rhs.y(),
                              lhs.z()-rhs.z());
        }

        inline Point operator+ (Point lhs, Vector const &rhs) {
                return lhs += rhs;
        }

        inline Point& Point::operator += (Vector const &rhs) {
                x_ += rhs.x();
                y_ += rhs.y();
                z_ += rhs.z();
                return *this;
        }

        // direction, direction
        inline real dot (Direction const &lhs, Direction const &rhs) {
                return mixed_dot (lhs, rhs);
        }

        // direction, vector
        inline Vector Direction::operator* (real f) const {
                return Vector (x_*f, y_*f, z_*f);
        }

        inline Vector Direction::operator/ (real f) const {
                assert (f != 0);
                return *this * (1 / f);
        }

        // normal, vector
        inline Vector Normal::operator* (real f) const {
                return Vector (x_*f, y_*f, z_*f);
        }

        inline Vector operator* (real f, const Normal &vec) {
                return vec * f;
        }

        inline Vector Normal::operator/ (real f) const {
                assert (f != 0);
                return *this * (1 / f);
        }


        // normal, normal
        inline real dot (Normal const &lhs, Normal const &rhs) {
                return mixed_dot (lhs, rhs);
        }
} }

#include "math3d/vector.inl.h"
#include "math3d/point.inl.h"
#include "math3d/direction.inl.h"
#include "math3d/normal.inl.h"

#endif // MATH3D_H_20110813
