#ifndef QUADTREE_H
#define QUADTREE_H

#include "geometry.h"
#include "terrain2d/patch.h"
#include <functional>

namespace crystal { namespace geometry {

class Terrain2d : public Geometry {
public:
        Terrain2d (std::function<real(real,real)> fun)
                : patch_(fun, 5)
        {
        }
private:
        PIntersection intersect_ (Ray const &ray) const ;
        terrain2d::Patch patch_;
};

} }

#endif // QUADTREE_H
