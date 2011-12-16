#include "patch.h"
#include "geoblocks/ray_tri_intersect.h"

namespace crystal { namespace geometry { namespace terrain2d {


Patch::Patch (real left, real right, real front, real back,
              std::function<real (real, real)> fun, int resolution,
              Transition const &transition)
: fanCount_ (resolution*resolution),
  fans_ (new Fan [fanCount_]),
  res_ (resolution),
  left_(left), right_(right), front_(front), back_(back)
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
#if 1

        const PInterval I = crystal::intersect (ray, left_,  min_h_, front_,
                                                     right_, max_h_, back_);
        if (!I) return PIntersection();
        const real minT = I->min,
                   maxT = I->max;

        const real width_ = right_-left_,
                   depth_ = back_-front_,
                   iwidth_  = 1 / width_,
                   idepth_  = 1 / depth_;
        const real voxelWidth_ = width_ / res_,
                   voxelDepth_ = depth_ / res_;
        const real ires_       = real(1) / res_;
        const real p_x = ray.origin.x,
                   p_z = ray.origin.z;
        const real d_x = ray.direction.x(),
                   d_z = ray.direction.z(),
                   id_x = 1 / d_x,
                   id_z = 1 / d_z;

        const real gridinter_x = p_x + d_x * minT,
                   gridinter_z = p_z + d_z * minT;

        const bool
                positive_x = d_x>=0, //d_right==XDirection::Right,
                positive_z = d_z>=0; //d_forward==ZDirection::Forward;

        const int outX = positive_x ? res_ : -1,
                  outZ = positive_z ? res_ : -1;
        const int stepX = positive_x ? 1 : -1,
                  stepZ = positive_z ? 1 : -1;
        const int pstepX = positive_x ? 1 : -1,
                  pstepZ = positive_z ? res_ : -res_; // in the hope it prefeteches res_ while checking positive_z
        const real tdelta_x = stepX * voxelWidth_ * id_x,
                   tdelta_z = stepZ * voxelDepth_ * id_z;


        // subtracting one from res_xy below gives in-bounds results, but yields
        // some superfluous voxel touches
        // TODO: check whether a max(0,cell_x) is needed

        // Minimize to account for floating point inaccuracies (for some bounding
        // boxes, this is needed, for some not)
        const int cell_x_ = res_ * (gridinter_x - left_)  * iwidth_,
                  cell_z_ = res_ * (gridinter_z - front_) * idepth_;
        const int cell_x = std::max (0, std::min(int(res_)-1, cell_x_)),
                  cell_z = std::max (0, std::min(int(res_)-1, cell_z_));

        assert (cell_x >= 0);
        assert (cell_z >= 0);
        assert (cell_x < static_cast<int>(res_));
        assert (cell_z < static_cast<int>(res_));

        const auto voxelToX = [&](real x) -> real { return left_  + x * ires_*width_; };
        const auto voxelToZ = [&](real z) -> real { return front_ + z * ires_*depth_; };

        real tmax_x = (voxelToX(cell_x+(int)positive_x) - gridinter_x) * id_x,
             tmax_z = (voxelToZ(cell_z+(int)positive_z) - gridinter_z) * id_z;

        int X = cell_x,
            Z = cell_z;
        Fan const * ph = this->ph (cell_x, cell_z);

        PIntersection p;
        while (1) {
                if ((p = ph->intersect (ray)))
                        break;

                const int x_lt_z = tmax_x < tmax_z;

                X      += x_lt_z ? stepX    : 0;
                Z      += x_lt_z ? 0        : stepZ;

                ph += x_lt_z ? pstepX : pstepZ;
                tmax_x += x_lt_z ? tdelta_x : 0;
                tmax_z += x_lt_z ? 0        : tdelta_z;

                if ((X==outX) | (Z==outZ)) break;
        }
        return p ?
                Intersection (p->distance, p->normal) :
                PIntersection();
               /*Intersection (t, this->material_, DifferentialGeometry (p->normal,
                                                                       p->normal,
                                                                       p->u,
                                                                       p->v)) :
               Intersection::Optional();*/

#else
        for (int i=0; i<fanCount_; ++i) {
                if (PIntersection p = fans_[i].intersect (ray)) {
                        return p;
                }
        }
        return PIntersection();
#endif
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
