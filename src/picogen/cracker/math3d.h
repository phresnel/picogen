#ifndef MATH3D_H_20110813
#define MATH3D_H_20110813

#include "math3d/vector.h"
#include "math3d/point.h"
#include "math3d/direction.h"

namespace picogen { namespace cracker {
        // any, any
        template <typename L, typename R>
        inline real mixed_dot (L const &lhs, R const &rhs) {
                return lhs.x()*rhs.x() +
                       lhs.y()*rhs.y() +
                       lhs.z()*rhs.z();
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

} }

#include "math3d/vector.inl.h"
#include "math3d/point.inl.h"
#include "math3d/direction.inl.h"

#endif // MATH3D_H_20110813
