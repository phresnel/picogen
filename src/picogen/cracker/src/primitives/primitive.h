#ifndef PRIMITIVE_H_20110811
#define PRIMITIVE_H_20110811

#include "intersection.h"
#include "ray.h"

namespace picogen { namespace cracker {

class Primitive {
public:
        Intersection::Optional operator() (Ray const &ray) const {
                return this->intersect(ray);
        }

        virtual ~Primitive() {}
protected:
        virtual Intersection::Optional intersect(Ray const &ray) const = 0;
};


} }

#endif // PRIMITIVE_H_20110811
