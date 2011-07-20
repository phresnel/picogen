#ifndef INTERSECTION_H_20110708
#define INTERSECTION_H_20110708

#include "real.h"
#include "math3d/normal.h"
#include <cassert>
#include <memory>

namespace picogen { namespace cracker {

class Material;

class Intersection {
public:
        Intersection() = delete;

        explicit Intersection (real distance,
                               Normal const &n,
                               std::shared_ptr<Material> mat)
        : distance_(distance), normal_(n), material_(mat)
        {
                assert(distance>=0);
        }

        real     distance() const { return distance_; }
        Normal   normal()   const { return normal_;   }
        std::shared_ptr<Material> material() const { return material_; }

private:
        real distance_;
        Normal normal_;
        std::shared_ptr<Material> material_;
};

inline bool nearer (const Intersection &lhs, const Intersection &rhs) {
        return lhs.distance() < rhs.distance();
}

} }

#endif // INTERSECTION_H_20110708
