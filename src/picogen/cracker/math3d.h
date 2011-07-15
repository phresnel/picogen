#ifndef MATH3D_H_20110813
#define MATH3D_H_20110813

#include "math3d/vector.h"
#include "math3d/point.h"
#include "math3d/direction.h"
#include "math3d/vector.inl.h"
#include "math3d/point.inl.h"
#include "math3d/direction.inl.h"


namespace picogen { namespace cracker {
        // any, any
        template <typename L, typename R>
        inline real mixed_dot (L const &lhs, R const &rhs) {
                return lhs.x()*rhs.x() +
                       lhs.y()*rhs.y() +
                       lhs.z()*rhs.z();
        }

        // point, point
        inline Vector operator- (Point const &lhs, Point const &rhs) {
                return Vector(lhs.x()-rhs.x(),
                              lhs.y()-rhs.y(),
                              lhs.z()-rhs.z());
        }

        // direction, direction
        inline real dot (Direction const &lhs, Direction const &rhs) {
                return mixed_dot (lhs, rhs);
        }

        // vector, vector
        inline real dot (Vector const &lhs, Vector const &rhs) {
                return mixed_dot (lhs, rhs);
        }
} }

#endif // MATH3D_H_20110813
