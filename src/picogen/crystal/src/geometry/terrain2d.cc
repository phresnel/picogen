#include "terrain2d.h"

namespace crystal { namespace geometry {

PIntersection Terrain2d::intersect_ (Ray const &ray) const {
        PIntersection pinter = quadtree_.intersect (ray);
        if (pinter) {
                Intersection &inter = *pinter;
                Point const poi = ray(inter.distance);
                inter.color = color_(poi);
        }
        return pinter;
}

} }
