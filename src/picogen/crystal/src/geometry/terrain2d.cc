#include "terrain2d.h"

namespace crystal { namespace geometry {


Terrain2d::Terrain2d (std::function<Radiance(Point const &p)> color,
           terrain2d::Deepness const &deepness, int patchResolution,
           std::function<real(real,real)> fun)
        : color_(color),
          quadtree_(deepness, fun, patchResolution)
{
}


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
