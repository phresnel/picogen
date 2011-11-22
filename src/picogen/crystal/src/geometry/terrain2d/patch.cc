#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {

Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution)
        : left_(left), right_(right),
          front_(front), back_(back),
          fun_(fun), res_(resolution)
{
        const real rd = (back_-front_)/res_;

        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();


        for (real iz = front_; iz<back_; iz+=rd) {
                for (real ix = left_; ix<right_; ix+=rd) {
                        const real s = rd;
                        const real x = ix;
                        const real z = iz;

                        const real x0 = x,     z0 = z,
                                   x1 = x,     z1 = z + s,
                                   x2 = x + s, z2 = z + s;
                        const real y0 = fun_(x0, z0),
                                   y1 = fun_(x1, z1),
                                   y2 = fun_(x2, z2);
                        const Point a (x0, y0, z0),
                                    b (x1, y1, z1),
                                    c (x2, y2, z2);

                        if (y0 < min_h_) min_h_ = y0;
                        if (y1 < min_h_) min_h_ = y1;
                        if (y2 < min_h_) min_h_ = y2;
                        if (y0 > max_h_) max_h_ = y0;
                        if (y1 > max_h_) max_h_ = y1;
                        if (y2 > max_h_) max_h_ = y2;
                }
        }
}

PIntersection Patch::intersect_ (const Ray &ray) const {
        //for (real iz=-res_/real(2); iz<res_/real(2); ++iz) {
        //        for (real ix=-res_/real(2); ix<res_/real(2); ++ix) {
        const real rd = (back_-front_)/res_;
        for (real iz = front_; iz<back_; iz+=rd) {
                for (real ix = left_; ix<right_; ix+=rd) {
                        const real s = rd;
                        const real x = ix;
                        const real z = iz;

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
