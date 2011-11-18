#ifndef QUADTREE_H
#define QUADTREE_H

#include "geometry.h"
#include <functional>

namespace crystal { namespace geometry {

class Terrain2d : public Geometry {
public:
        Terrain2d (std::function<real(real,real)> fun)
                : fun_(fun)
        {
        }
private:
        PIntersection intersect_ (Ray const &ray) const ;
        std::function<real(real,real)> fun_;
};

} }

#endif // QUADTREE_H
