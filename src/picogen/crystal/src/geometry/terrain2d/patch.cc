#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {

Patch::Patch (std::function<real (real, real)> fun, int resolution)
        : fun_(fun), res_(resolution)
{
}

PIntersection Patch::intersect_ (const Ray &ray) const {
        for (real iz=-res_/real(2); iz<res_/real(2); ++iz) {
                for (real ix=-res_/real(2); ix<res_/real(2); ++ix) {
                        const real s = 10;
                        const real x = ix * s;
                        const real z = iz * s;

                        const real x0 = x,     z0 = z,
                                   x1 = x,     z1 = z + s,
                                   x2 = x + s, z2 = z + s;
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


} } }
