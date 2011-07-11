#ifndef PRIMITIVE_H_20110811
#define PRIMITIVE_H_20110811

namespace picogen { namespace cracker {

class Primitive {
public:
        PotentialIntersection operator() (Ray const &ray) {
                return this->intersect(ray);
        }

        virtual ~Primitive() {}
protected:
        virtual PotentialIntersection intersect(Ray const &ray) const = 0;
};


} }

#endif // PRIMITIVE_H_20110811
