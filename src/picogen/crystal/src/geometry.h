#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "crystal_math.h"

namespace crystal {
        class Geometry
        {
        public:
                virtual ~Geometry() {}
                PIntersection intersect (Ray const &ray) const {
                        return this->intersect_(ray);
                }
        private:
                virtual PIntersection intersect_ (Ray const &ray) const = 0;
        };
}

#endif // GEOMETRY_H
