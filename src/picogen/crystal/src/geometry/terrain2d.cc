#include "terrain2d.h"

namespace crystal { namespace geometry {

PIntersection Terrain2d::intersect_ (Ray const &ray) const {
        return quadtree_.intersect (ray);
}

} }
