#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {

Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution)
{
        triangleCount_ = resolution*resolution*2;
        triangles_ = new Triangle [triangleCount_];
        //const real rd = (back_-front_)/res_;

        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();

        const real width = right - left,
                   depth = back - front;
        const real ires = 1 / real(resolution);
        int tindex = 0;
        for (int z=0; z<resolution; ++z) {
                for (int x=0; x<resolution; ++x) {
                        const real fx0 = left + x * ires * width,
                                   fz0 = front + z * ires * depth,
                                   fy0 = fun(fx0, fz0);
                        const real fx1 = left + (1+x) * ires * width,
                                   fz1 = front + z * ires * depth,
                                   fy1 = fun(fx1, fz1);
                        const real fx2 = left + (1+x) * ires * width,
                                   fz2 = front + (1+z) * ires * depth,
                                   fy2 = fun(fx2, fz2);
                        const real fx3 = left + x * ires * width,
                                   fz3 = front + (1+z) * ires * depth,
                                   fy3 = fun(fx3, fz3);

                        Triangle &A = triangles_[tindex++];
                        Triangle &B = triangles_[tindex++];
                        A.a = Point (fx0, fy0, fz0);
                        A.b = Point (fx1, fy1, fz1);
                        A.c = Point (fx2, fy2, fz2);
                        B.a = Point (fx0, fy0, fz0);
                        B.b = Point (fx2, fy2, fz2);
                        B.c = Point (fx3, fy3, fz3);

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

        for (int i=0; i<triangleCount_; ++i) {
                Triangle const &t = triangles_ [i];
                real t_, u_, v_;
                Normal normal_(0,1,0);
                if (geoblocks::raytri_intersect (ray,
                                                 t.a, t.b, t.c,
                                                 t_, u_, v_,
                                                 normal_))
                {
                        return Intersection (t_, normal_);
                }
        }
        return PIntersection();
}


} } }
