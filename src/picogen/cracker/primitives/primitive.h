#ifndef PRIMITIVE_H_20110811
#define PRIMITIVE_H_20110811

#include "potentialintersection.h"
#include "ray.h"

namespace picogen { namespace cracker {

class Primitive {
public:
        PotentialIntersection operator() (Ray const &ray) const {
                return this->intersect(ray);
        }

        virtual ~Primitive() {}
protected:
        virtual PotentialIntersection intersect(Ray const &ray) const = 0;
};


} }

#endif // PRIMITIVE_H_20110811
