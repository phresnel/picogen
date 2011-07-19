#ifndef INTERSECTION_H_20110708
#define INTERSECTION_H_20110708

#include "real.h"
#include "math3d/normal.h"
#include <cassert>

namespace picogen { namespace cracker {

class Intersection {
public:
        Intersection() = delete;
        explicit Intersection (real distance, Normal const &n)
                : distance_(distance), normal_(n)
        {
                assert(distance>=0);
        }

        real distance() const { return distance_; }
        Normal normal() const { return normal_; }
private:
        real distance_;
        Normal normal_;
};

inline bool nearer (const Intersection &lhs, const Intersection &rhs) {
        return lhs.distance() < rhs.distance();
}

} }

#endif // INTERSECTION_H_20110708
