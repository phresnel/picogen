#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {

Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution)
        : quads_(new Quad [resolution*resolution]),
          left_(left), right_(right),
          front_(front), back_(back),
          fun_(fun), res_(resolution)
{
        //const real rd = (back_-front_)/res_;

        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();


        /*for (real iz = front_; iz<back_; iz+=rd) {
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
        }*/

        const real width = right_ - left_,
                   depth = back_ - front_;
        const real ires = 1 / real(res_);
        for (int z=0; z<res_; ++z) {
                for (int x=0; x<res_; ++x) {
                        const real fx0 = left_ + x * ires * width,
                                   fz0 = front_ + z * ires * depth,
                                   fy0 = fun(fx0, fz0);
                        const real fx1 = left_ + (1+x) * ires * width,
                                   fz1 = front_ + z * ires * depth,
                                   fy1 = fun(fx1, fz1);
                        const real fx2 = left_ + (1+x) * ires * width,
                                   fz2 = front_ + (1+z) * ires * depth,
                                   fy2 = fun(fx2, fz2);
                        const real fx3 = left_ + x * ires * width,
                                   fz3 = front_ + (1+z) * ires * depth,
                                   fy3 = fun(fx3, fz3);
                        Quad &q = quads_[z * res_ + x];
                        q.a = Point (fx0, fy0, fz0);
                        q.b = Point (fx1, fy1, fz1);
                        q.c = Point (fx2, fy2, fz2);
                        q.d = Point (fx3, fy3, fz3);

                        min_h_ = min(min_h_, fy0);
                        min_h_ = min(min_h_, fy1);
                        min_h_ = min(min_h_, fy2);
                        min_h_ = min(min_h_, fy3);
                        max_h_ = max(max_h_, fy0);
                        max_h_ = max(max_h_, fy1);
                        max_h_ = max(max_h_, fy2);
                        max_h_ = max(max_h_, fy3);
                }
        }
}

PIntersection Patch::intersect_ (const Ray &ray) const {
        //for (real iz=-res_/real(2); iz<res_/real(2); ++iz) {
        //        for (real ix=-res_/real(2); ix<res_/real(2); ++ix) {

        for (int i=0; i<res_*res_; ++i) {
                Quad const &q = quads_ [i];
                real t_, u_, v_;
                Normal normal_(0,1,0);
                if (geoblocks::raytri_intersect (ray,
                                                 q.a, q.b, q.c,
                                                 t_, u_, v_,
                                                 normal_))
                {
                        return Intersection (t_, normal_);
                }
                if (geoblocks::raytri_intersect (ray,
                                                 q.a, q.c, q.d,
                                                 t_, u_, v_,
                                                 normal_))
                {
                        return Intersection (t_, normal_);
                }
        }
        return PIntersection();
#if 0
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
#endif
}


} } }
