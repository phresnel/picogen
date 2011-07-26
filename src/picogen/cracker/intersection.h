#ifndef INTERSECTION_H_20110708
#define INTERSECTION_H_20110708

#include "real.h"
#include "math3d/normal.h"
#include "differentialgeometry.h"
#include <cassert>
#include <memory>

namespace picogen { namespace cracker {

class Material;

namespace detail {
        class OptionalIntersection;
}

class Intersection {
public:
        Intersection() = delete;

        explicit Intersection (real distance,
                               Normal const &n,
                               std::shared_ptr<Material> mat,
                               DifferentialGeometry differentialGeometry)
        : distance_(distance), normal_(n), material_(mat)
        , differentialGeometry_(differentialGeometry)
        {
                assert(distance>=0);
        }

        real     distance() const { return distance_; }
        Normal   normal()   const { return normal_;   }
        std::shared_ptr<Material> material() const { return material_; }
        const Material& material_ref() const { return *material_; }

        DifferentialGeometry differentialGeometry() const {
                return differentialGeometry_;
        }

        typedef detail::OptionalIntersection Optional;

private:
        real distance_;
        Normal normal_;
        std::shared_ptr<Material> material_;
        DifferentialGeometry differentialGeometry_;
};

namespace detail {

        class OptionalIntersection {
        public:
                OptionalIntersection()
                : intersection_(
                          0,
                          Normal(0,1,0),
                          std::shared_ptr<Material>(),
                          DifferentialGeometry(Normal(0,1,0), Normal(0,1,0),
                                               Normal(0,1,0), Normal(0,1,0)))
                , empty_(true)
                {
                }

                OptionalIntersection (Intersection const &intersection)
                : intersection_(intersection), empty_(false) {
                }

                explicit operator bool() const {
                        return !empty_;
                }

                Intersection intersection() const {
                        assert (!empty_);
                        return intersection_;
                }

        private:
                Intersection intersection_;
                bool empty_;
        };

}

inline bool nearer (const Intersection &lhs, const Intersection &rhs) {
        return lhs.distance() < rhs.distance();
}

} }

#endif // INTERSECTION_H_20110708
