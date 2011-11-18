#ifndef TERRAIN2D_H_20111118
#define TERRAIN2D_H_20111118

#include "geometry.h"
#include "terrain2d/quadtree.h"
#include <functional>

namespace crystal { namespace geometry {

class Terrain2d : public Geometry {
public:
        Terrain2d (std::function<real(real,real)> fun)
                : quadtree_(fun, 5)
        {
        }
private:
        PIntersection intersect_ (Ray const &ray) const ;
        terrain2d::Quadtree quadtree_;
};

} }

#endif // TERRAIN2D_H_20111118
