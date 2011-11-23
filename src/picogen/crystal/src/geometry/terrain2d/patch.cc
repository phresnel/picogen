#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {

Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution,
              Transition const &transition)
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

        auto grid2point = [&] (int x, int z) {
                const real fx0 = left + x * ires * width,
                           fz0 = front + z * ires * depth,
                           fy0 = fun(fx0, fz0);
                return Point(fx0, fy0, fz0);
        };

        if (transition.front()) {
                for (int x=0; x<resolution; ++x) {
                }
        }
        for (int z=transition.front(); z<resolution; ++z) {
                for (int x=0; x<resolution; ++x) {

                        Triangle &A = triangles_[tindex++];
                        Triangle &B = triangles_[tindex++];
                        A.a = grid2point (x,  z);
                        A.b = grid2point (x+1,z);
                        A.c = grid2point (x+1,z+1);
                        B.a = grid2point (x,  z);
                        B.b = grid2point (x+1,z+1);
                        B.c = grid2point (x,  z+1);

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
