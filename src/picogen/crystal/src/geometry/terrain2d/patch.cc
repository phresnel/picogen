#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {


Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution,
              Transition const &transition)
: fanCount_ (resolution*resolution),
  fans_ (new Fan [fanCount_])
{
        const real width = right - left,
                   depth = back - front;
        const real ires = 1 / real(resolution);

        auto grid2point = [&] (real x, real z) {
                const real fx0 = left + x * ires * width,
                           fz0 = front + z * ires * depth,
                           fy0 = fun(fx0, fz0);
                return Point(fx0, fy0, fz0);
        };


        for (int z_=0; z_<resolution; ++z_) {
                for (int x_=0; x_<resolution; ++x_) {
                        Fan &fan = fans_[x_+z_*resolution];

                        const int size = 6;
                        fan.vertices = new Point[size];
                        fan.size = size;

                        const real x = x_ + 0.5,
                                   z = z_ + 0.5;
                        Point *it = fan.vertices;

                        const real t = 0.5;
                        *(it++) = grid2point(x,   z  );
                        *(it++) = grid2point(x-t, z+t);
                        *(it++) = grid2point(x+t, z+t);
                        *(it++) = grid2point(x+t, z-t);
                        *(it++) = grid2point(x-t, z-t);
                        *(it++) = grid2point(x-t, z+t);
                }
        }

        // Find maximum and minimum height of grid.
        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();
        for (int i=0; i<fanCount_; ++i) {
                Fan const &fan = fans_ [i];
                for (int i=0; i<fan.size; ++i) {
                        const real &h = fan.vertices[i].y;
                        min_h_ = std::min(min_h_, h);
                        max_h_ = std::max(max_h_, h);
                }
        }

}

Patch::~Patch()
{
}




PIntersection Patch::Fan::intersect (Ray const &ray) const
{
        Point const &a = vertices[0];

        Normal nearest_n(0,1,0);
        real nearest_t = std::numeric_limits<real>::infinity();
        for (int i=1; i<size-1; ++i) {
                Point const &b = vertices[i];
                Point const &c = vertices[i+1];

                real t_, u_, v_;
                Normal normal_(0,1,0);
                if (geoblocks::raytri_intersect (ray,
                                                 a, b, c,
                                                 t_, u_, v_,
                                                 normal_))
                {
                        if (t_ < nearest_t) {
                                nearest_t = t_;
                                nearest_n = normal_;
                        }
                }
        }
        if (nearest_t != std::numeric_limits<real>::infinity())
                return Intersection (nearest_t, nearest_n);
        return PIntersection();
}


PIntersection Patch::intersect_ (const Ray &ray) const
{
        for (int i=0; i<fanCount_; ++i) {
                if (PIntersection p = fans_[i].intersect (ray)) {
                        return p;
                }
        }
        return PIntersection();
}


#if 0
Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution,
              Transition const &transition)
{
        const real width = right - left,
                   depth = back - front;
        const real ires = 1 / real(resolution);

        auto grid2point = [&] (real x, real z) {
                const real fx0 = left + x * ires * width,
                           fz0 = front + z * ires * depth,
                           fy0 = fun(fx0, fz0);
                return Point(fx0, fy0, fz0);
        };

        std::vector<Triangle> tris_;
        if (transition.front()) {
                for (int x=transition.left(); x<resolution-transition.right(); ++x) {
                        const int z = 0;
                        tris_.emplace_back (grid2point (x,    z),
                                            grid2point (x,    z+1),
                                            grid2point (x+0.5,z));
                        tris_.emplace_back (grid2point (x,    z+1),
                                            grid2point (x+1,  z+1),
                                            grid2point (x+0.5,z));
                        tris_.emplace_back (grid2point (x+0.5,z),
                                            grid2point (x+1,  z+1),
                                            grid2point (x+1,  z));
                }
        }
        if (transition.left()) {
                for (int z=transition.front(); z<resolution-transition.back(); ++z) {
                        const int x = 0;
                        tris_.emplace_back (grid2point (x,  z),
                                            grid2point (x,  z+0.5),
                                            grid2point (x+1,z));
                        tris_.emplace_back (grid2point (x,  z+0.5),
                                            grid2point (x+1,z+1),
                                            grid2point (x+1,z));
                        tris_.emplace_back (grid2point (x,  z+1),
                                            grid2point (x+1,z+1),
                                            grid2point (x,  z+0.5));
                }
        }
        if (transition.right()) {
                for (int z=transition.front(); z<resolution-transition.back(); ++z) {
                        const int x = resolution-1;
                        tris_.emplace_back (grid2point (x,  z),
                                            grid2point (x+1,z+0.5),
                                            grid2point (x+1,z));
                        tris_.emplace_back (grid2point (x,  z),
                                            grid2point (x,  z+1),
                                            grid2point (x+1,z+0.5));
                        tris_.emplace_back (grid2point (x,  z+1),
                                            grid2point (x+1,z+1),
                                            grid2point (x+1,z+0.5));
                }
        }
        // main set.
        for (int z=transition.front(); z<resolution-transition.back(); ++z) {
                for (int x=transition.left(); x<resolution-transition.right(); ++x) {
                        tris_.emplace_back (grid2point (x,  z+1),
                                            grid2point (x+1,z+1),
                                            grid2point (x,  z));
                        tris_.emplace_back (grid2point (x+1,z+1),
                                            grid2point (x+1,z),
                                            grid2point (x,  z));
                }
        }
        if (transition.back()) {
                for (int x=transition.left(); x<resolution-transition.right(); ++x) {
                        const int z = resolution-1;
                        tris_.emplace_back (grid2point (x,    z),
                                            grid2point (x,    z+1),
                                            grid2point (x+0.5,z+1));
                        tris_.emplace_back (grid2point (x+0.5,z+1),
                                            grid2point (x+1,  z),
                                            grid2point (x,    z));
                        tris_.emplace_back (grid2point (x+0.5,z+1),
                                            grid2point (x+1,  z+1),
                                            grid2point (x+1,  z));
                }
        }

        // corner transitions
        if (transition.left() && transition.front()) {
                const int x = 0, z = 0;
                tris_.emplace_back (grid2point (x,    z),
                                    grid2point (x,    z+0.5),
                                    grid2point (x+1,  z+1));
                tris_.emplace_back (grid2point (x,    z+0.5),
                                    grid2point (x,    z+1),
                                    grid2point (x+1,  z+1));
                tris_.emplace_back (grid2point (x+1,  z+1),
                                    grid2point (x+0.5,z),
                                    grid2point (x,    z));
                tris_.emplace_back (grid2point (x+1,  z+1),
                                    grid2point (x+1,  z),
                                    grid2point (x+0.5,z));
        }
        if (transition.right() && transition.front()) {
                const int x = resolution-1, z = 0;
                tris_.emplace_back (grid2point (x,    z+1),
                                    grid2point (x+1,  z+1),
                                    grid2point (x+1,  z+0.5));
                tris_.emplace_back (grid2point (x,    z+1),
                                    grid2point (x+1,  z+0.5),
                                    grid2point (x+1,  z+0));
                tris_.emplace_back (grid2point (x,    z+1),
                                    grid2point (x+1,  z),
                                    grid2point (x+0.5,z));
                tris_.emplace_back (grid2point (x,    z+1),
                                    grid2point (x+0.5,z),
                                    grid2point (x,    z));
        }
        if (transition.left() && transition.back()) {
                const int x = 0, z = resolution-1;
                tris_.emplace_back (grid2point (x+1,  z),
                                    grid2point (x,    z),
                                    grid2point (x,    z+0.5));
                tris_.emplace_back (grid2point (x+1,  z),
                                    grid2point (x,    z+0.5),
                                    grid2point (x,    z+1));
                tris_.emplace_back (grid2point (x+1,  z),
                                    grid2point (x,    z+1),
                                    grid2point (x+0.5,z+1));
                tris_.emplace_back (grid2point (x+1,  z),
                                    grid2point (x+0.5,z+1),
                                    grid2point (x+1,  z+1));
        }
        if (transition.right() && transition.back()) {
                const int x = resolution-1, z = resolution-1;
                tris_.emplace_back (grid2point (x,    z),
                                    grid2point (x,    z+1),
                                    grid2point (x+0.5,z+1));
                tris_.emplace_back (grid2point (x,    z),
                                    grid2point (x+0.5,z+1),
                                    grid2point (x+1,  z+1));
                tris_.emplace_back (grid2point (x,    z),
                                    grid2point (x+1,  z+1),
                                    grid2point (x+1,  z+0.5));
                tris_.emplace_back (grid2point (x,    z),
                                    grid2point (x+1,  z+0.5),
                                    grid2point (x+1,  z));
        }

        triangleCount_ = tris_.size();
        triangles_ = new Triangle [triangleCount_];
        std::copy(tris_.begin(), tris_.end(), triangles_);



        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();
        for (int i=0; i<triangleCount_; ++i) {
                Triangle const &t = triangles_ [i];
                min_h_ = std::min(min_h_, t.a.y);
                min_h_ = std::min(min_h_, t.b.y);
                min_h_ = std::min(min_h_, t.c.y);
                max_h_ = std::max(max_h_, t.a.y);
                max_h_ = std::max(max_h_, t.b.y);
                max_h_ = std::max(max_h_, t.c.y);
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

#endif


} } }
