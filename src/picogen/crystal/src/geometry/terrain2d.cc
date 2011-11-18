#include "terrain2d.h"
#include "geoblocks/ray_tri_intersect.h"


namespace crystal { namespace geometry {

PIntersection Terrain2d::intersect_ (Ray const &ray) const {
        /*const Point a (-1, 1, 3),
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
        }*/

        for (real z=-10; z<10; ++z) {
                for (real x=-10; x<10; ++x) {
                        const real x0 = x,     z0 = z,
                                   x1 = x,     z1 = z + 1,
                                   x2 = x + 1, z2 = z + 1;
                        const real y0 = fun_(x0, z0),
                                   y1 = fun_(x1, z1),
                                   y2 = fun_(x2, z2);
                        const Point a (x0, y0, z0),
                                    b (x1, y1, z1),
                                    c (x2, y2, z2);
                        real t_, u_, v_;
                        Normal normal_(0,1,0);
                        if (geoblocks::raytri_intersect (ray,
                                                         a, b, c,
                                                         t_, u_, v_,
                                                         normal_))
                        {
                                return Intersection (t_, normal_);
                        }
                }
        }
        return PIntersection();
}

} }
