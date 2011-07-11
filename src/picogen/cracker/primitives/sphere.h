#ifndef SPHERE_H_20110711
#define SPHERE_H_20110711

#include "potentialintersection.h"
#include "ray.h"
#include "point.h"
#include "real.h"
#include "primitives/primitive.h"

namespace picogen { namespace cracker {

class Sphere : public Primitive {
public:
        Sphere() = delete;
        Sphere (Point center, real radius) :
                center_(center), radius_(radius)
        {}

        PotentialIntersection operator() (Ray const &ray) const {
                return intersect(ray);
        }

private:
        PotentialIntersection intersect (Ray const &ray) const {
                if (ray.direction().x()<0) {
                        const Intersection isect(std::fabs(100*ray.direction().x()));
                        return isect;
                }
                return PotentialIntersection();
        }

        Point center_;
        real radius_;
};

} }

#endif // SPHERE_H_20110711
