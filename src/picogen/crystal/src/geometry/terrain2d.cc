#include "terrain2d.h"
#include "geoblocks/ray_tri_intersect.h"


namespace crystal { namespace geometry {

PIntersection Terrain2d::intersect_ (Ray const &ray) const {
        const Point a (-1, 1, 3),
                    b (1,  1, 3),
                    c (0, 0, 3);
        real t_, u_, v_;
        Normal normal_(0,1,0);
        if (geoblocks::raytri_intersect (ray,
                                         a, b, c,
                                         t_, u_, v_,
                                         normal_))
        {
                return Intersection (t_, normal_);
        }
        return PIntersection();
}

} }
