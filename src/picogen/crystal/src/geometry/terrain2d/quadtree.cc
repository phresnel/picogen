#include "quadtree.h"

namespace crystal { namespace geometry { namespace terrain2d {

Quadtree::Quadtree(std::function<real(real,real)> fun, int resolution)
        : patch_(fun, resolution)
{
}

PIntersection Quadtree::intersect_ (const Ray &ray) const {
        return patch_.intersect (ray);
}

} } }
