#ifndef PATCH_H_INCLUDED_20110826
#define PATCH_H_INCLUDED_20110826

#include "../intersection.h"

namespace picogen { namespace cracker {
        class Ray;
} }

#include "ray.h"
#include "color.h"
#include <limits>
#include <QDebug>

namespace picogen { namespace cracker { namespace detail {

        enum class XDirection { Left = -1,     Right = 1 };
        enum class ZDirection { Backward = -1, Forward = 1 };

        class Quad {
        public:
                enum class Shape {
                        Bump,
                        Pit
                };

                class Plane {
                public:
                        Normal u, v, normal;
                        real   distance;

                        Plane()
                                : u(1,0,0), v(0,0,1), normal(0,1,0)
                                , distance(0)
                        {}

                        real dot (Point const &point) const {
                                return mixed_dot (normal, point) + distance;
                        }

                        /*Plane (Point point, Normal normal)
                                : normal(normal)
                                , distance(-mixed_dot(normal,point))
                        {
                        }*/

                        Plane (Point a, Point b, Point c)
                                : u(normalize<Normal>(b-a))
                                , v(normalize<Normal>(c-a))
                                , normal(normalize<Normal>(cross(u, v)))
                                , distance (-mixed_dot(normal,a))
                        {
                        }

                        real intersect (Ray const &ray) const {
                                const real
                                    a = mixed_dot (ray.origin(), normal) + distance,
                                    b = mixed_dot (ray.direction(), normal),
                                    t = -a / b;
                                return t;
                        }
                };

                class Rectangle {
                public:
                        real left, right, front, back;
                        real width;

                        Rectangle() {}

                        Rectangle (real left, real right, real front, real back)
                                : left(left), right(right)
                                , front(front), back(back)
                                , width ((right - left))
                        {}

                        bool inside (real x, real z) const {
                                return (x>=left)  & (x<=right)
                                     & (z>=front) & (z<=back);
                                // TODO: possibly just use  < instead of <=
                        }
                };

                Shape shape;
                Plane leftPlane, rightPlane;
                Rectangle rectangle;

                Quad() = default;

                Quad (Rectangle rect, real h0, real h1, real h2, real h3)
                        : rectangle(rect)
                {
                        const Point A { rect.left,  h0, rect.back  },
                                    B { rect.right, h1, rect.back  },
                                    C { rect.right, h2, rect.front },
                                    D { rect.left,  h3, rect.front };
                        leftPlane  = Plane (A,B,D);
                        rightPlane = Plane (B,C,D);

                        // We have a very restricted range of possible
                        // plane normals; it should be enough when we just compare
                        // the left plane's x component to the right plane's
                        // (or the z component).
                        //if (leftPlane.normal.x() < rightPlane.normal.x())
                        /*
                        if (leftPlane.dot(C) <= 0)
                                shape = Shape::Bump;
                        else
                                shape = Shape::Pit;
                        */
                }


                Quad::Plane const * intersect (Ray const &ray, real &t) const
                {
                        const real lt = leftPlane.intersect(ray),
                                   rt = rightPlane.intersect(ray);

                        const real &o_x = ray.origin().x(), &d_x = ray.direction().x(),
                                   &o_z = ray.origin().z(), &d_z = ray.direction().z();

                        const real lpx = o_x + d_x*lt,
                                   lpz = o_z + d_z*lt;

                        const real rpx = o_x + d_x*rt,
                                   rpz = o_z + d_z*rt;

                        // Computation of u/v values depends on layout of quads.
                        const real lu = (lpx - rectangle.left ),// * rectangle.iwidth,
                                   lv = (rectangle.back - lpz);//  * rectangle.idepth;
                        const real ru = (rpx - rectangle.left ),// * rectangle.iwidth,
                                   rv = (rectangle.back - rpz);//  * rectangle.idepth;

                        const int l0 = lpx>=rectangle.left,
                                   l1 = rectangle.back>=lpz,
                                   l2 = lt>=0,
                                   l3 = lu+lv <= rectangle.width,
                                   r0 = rpx<=rectangle.right,
                                   r1 = rectangle.front<=rpz,
                                   r2 = rt >= 0,
                                   r3 = ru+rv > rectangle.width,
                                   la = l0 & l1,
                                   lb = l2 & l3,
                                   ra = r0 & r1,
                                   rb = r2 & r3,
                                   l = la & lb,
                                   r = ra & rb;
                        if (l) {
                                t = lt;
                                return &leftPlane;
                        }
                        if (r) {
                                t = rt;
                                return &rightPlane;
                        }
                        return 0;//return false;
                }
        };


        class Patch {
        public:
                Patch ()                         = delete;
                Patch (Patch const &)            = delete;
                Patch& operator= (Patch const &) = delete;


                Patch (real left, real right,
                       real front, real back,
                       unsigned int res_x,
                       unsigned int res_z,
                       std::function<real (real, real)> fun,
                       real &y_min, real &y_max);
                ~Patch();
                Intersection::Optional operator() (Ray const &ray,
                                                   real min, real max) const;

                template <ZDirection d_forward, XDirection d_right>
                Intersection::Optional fast_intersect (Ray const &ray,
                                                       real min,
                                                       real max,
                                                       real p_x, real p_z,
                                                       real d_x, real d_z,
                                                       real id_x, real id_z) const;

                //try that out and if won''t be fast than 0.72 secs, rollback changes

        private:
                real left_, right_, front_, back_;
                unsigned int res_x_, res_z_, stride_;
                //Vector *h_;
                Quad *h_;

                // speed ups (could be easily computed at runtime)
                real width_, depth_;
                real voxelWidth_, voxelDepth_;
                real ires_x_, ires_z_;
                real iwidth_, idepth_;

                std::shared_ptr<Material> material_;


                template <int step_x, int step_z>
                Intersection::Optional intersect (Ray const &ray,
                                                  int left, int right,
                                                  int front, int back) const;

                //Intersection::Optional intersect_quad (Ray const &, int X, int Z) const ;
                //Quad::Plane const * intersect_quad (Ray const &, Quad const *, real &t) const ;
                /*Intersection::Optional intersect_amanatides (Ray const &ray,
                                                   real minT,
                                                   real maxT) const;*/

                //template <XDir

                Quad const * ph (unsigned int x, unsigned z) const;
                Quad& h (unsigned int x, unsigned z);
        };

} } }


#include "ray.h"
#include "terrain/morton.h"
#include "materials/lambertmaterial.h"

namespace picogen { namespace cracker { namespace detail {

static const real tri_eps = 0.00000001;
static int
inline raytri_intersect (
    const Ray &ray,
    const Vector &a, const Vector &b, const Vector &c,
    real &t, real &u, real &v,
    Normal &normal_
) {
    Vector vect0, vect1, nvect, normal;
    real det, inv_det;

    vect0 = b - a;
    vect1 = c - a;

    normal = cross (vect0, vect1);
    /*if (normal.x()==0 && normal.y()==0 && normal.z()==0) {
            qDebug() << "vect0:" << vect0.x() << vect0.y() << vect0.z();
            qDebug() << "vect1:" << vect1.x() << vect1.y() << vect1.z();
    }*/

    /* orientation of the ray with respect to the triangle's normal,
       also used to calculate output parameters*/
    det = -mixed_dot(ray.direction(), normal);

    /* if determinant is near zero, ray is parallel to the plane of triangle */
    if ((det > -tri_eps) & (det < tri_eps)) return 0;

    /* calculate vector from ray origin to a */
    vect0 = a - static_cast<Vector>(ray.origin());

    /* normal vector used to calculate u and v parameters */
    nvect = mixed_cross<Vector> (ray.direction(), vect0);

    inv_det = 1.0 / det;
    vect1 = b - static_cast<Vector>(ray.origin());

    /* calculate v parameter and test bounds */
    v = - dot(vect1, nvect) * inv_det;

    if ((v < 0.0) | (v > 1.0)) return 0;

    /* calculate vector from ray origin to c*/
    vect1 = c - static_cast<Vector>(ray.origin());

    /* calculate v parameter and test bounds */
    u = dot (vect1, nvect) * inv_det;

    if ((u < 0.0) | (u + v > 1.0)) return 0;

    /* calculate t, ray intersects triangle */
    t = - dot(vect0, normal) * inv_det;

    //---------
    // pretty crappy but sometimes useful wireframe mode
    //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;
    if (t < 0)
        return 0;

    normal_ = normalize<Normal>(normal); // if we can make this really fast,
                                         // the optimization (t>=0)*... may work out
    const int ret[2] = {1, -1};
    return ret[mixed_dot (ray.direction(), normal) > 0.0];
}


/*
TODO: blazing fast ray/place Intersection
      blazing fast determination of hit/no hit
      in quadtree: calculate u/v
TODO: memory pool for nodes
TODO: replace width/height with just width
*/
/*inline Intersection::Optional Patch::intersect_quad (Ray const &ray,
                                              int X, int Z) const {

        Quad const *q = ph (X, Z);
        real t=-1;
        Normal u(1,0,0), v(0,0,1), n(0,1,0);
        Color color;
        if (!q->intersect(ray, t, n, u, v))
                return Intersection::Optional();

        return Intersection (
             t,
             material_,
             DifferentialGeometry(n, n, u, v));
}*/

#if 0
inline Quad::Plane const * Patch::intersect_quad (Ray const &ray,
                                                  Quad const *q,
                                                  real &t) const {

        real t=-1;
        Normal u(1,0,0), v(0,0,1), n(0,1,0);
        Color color;
        if (!q->intersect(ray, t, n, u, v))
                return Intersection::Optional();

        return Intersection (
                                t,
                                material_,
                                DifferentialGeometry(n, n, u, v));
}
#endif



inline Quad const * Patch::ph (unsigned int x, unsigned z) const {
        return h_ + x + z*stride_;
}
inline Quad& Patch::h (unsigned int x, unsigned z) {
        return h_[x + z*stride_];
}


inline Intersection::Optional Patch::operator () (
        Ray const &ray, real minT, real maxT) const
{
        if (ray.direction().x() >= 0) {
                if (ray.direction().z() >= 0) {
                        return fast_intersect<ZDirection::Forward,
                                                  XDirection::Right> (ray, minT, maxT,
                                                                      ray.origin().x(),
                                                                      ray.origin().z(),
                                                                      ray.direction().x(),
                                                                      ray.direction().z(),
                                                                      1/ray.direction().x(),
                                                                      1/ray.direction().z());
                } else {
                        return fast_intersect<ZDirection::Backward,
                                                  XDirection::Right> (ray, minT, maxT,
                                                                      ray.origin().x(),
                                                                      ray.origin().z(),
                                                                      ray.direction().x(),
                                                                      ray.direction().z(),
                                                                      1/ray.direction().x(),
                                                                      1/ray.direction().z());
                }
        } else {
                if (ray.direction().z() >= 0) {
                        return fast_intersect<ZDirection::Forward,
                                                  XDirection::Left> (ray, minT, maxT,
                                                                     ray.origin().x(),
                                                                     ray.origin().z(),
                                                                     ray.direction().x(),
                                                                     ray.direction().z(),
                                                                     1/ray.direction().x(),
                                                                     1/ray.direction().z());
                } else {
                        return fast_intersect<ZDirection::Backward,
                                                  XDirection::Left> (ray, minT, maxT,
                                                                     ray.origin().x(),
                                                                     ray.origin().z(),
                                                                     ray.direction().x(),
                                                                     ray.direction().z(),
                                                                     1/ray.direction().x(),
                                                                     1/ray.direction().z());
                }
        }
}


template <ZDirection d_forward, XDirection d_right>
inline Intersection::Optional Patch::fast_intersect (
        Ray const &ray,
        const real minT, const real maxT,
        const real p_x, const real p_z,
        const real d_x, const real d_z,
        const real id_x, const real id_z) const
{
        /*const real p_x = ray.origin().x(),
                   p_z = ray.origin().z();
        const real d_x = ray.direction().x(),
                   d_z = ray.direction().z();*/

        // dda

        const real gridinter_x = p_x + d_x * minT,
                   gridinter_z = p_z + d_z * minT;

        enum {
                positive_x = d_right==XDirection::Right,
                positive_z = d_forward==ZDirection::Forward
        };
        const int outX = positive_x ? res_x_ : -1,
                  outZ = positive_z ? res_z_ : -1;
        //const int stepX = positive_x ? 1 : -1,
        //          stepZ = positive_z ? 1 : -1;
        const int stepX = positive_x ? 1 : -1,
                  stepZ = positive_z ? 1 : -1;
        const int pstepX = positive_x ? 1 : -1,
                  pstepZ = positive_z ? stride_ : -stride_;
        const real tdelta_x = stepX * voxelWidth_ * id_x,
                   tdelta_z = stepZ * voxelDepth_ * id_z;


        // subtracting one from res_xy below gives in-bounds results, but yields
        // some superfluous voxel touches
        // TODO: check whether a max(0,cell_x) is needed

        // Minimize to account for floating point inaccuracies (for some bounding
        // boxes, this is needed, for some not)
        static real x__  = (res_x_*iwidth_);
        static real z__  = (res_z_*idepth_);

        const int cell_x = std::min(int(res_x_)-1,
                                    int((gridinter_x - left_ ) * x__)),
                  cell_z = std::min(int(res_z_)-1,
                                    int((gridinter_z - front_) * z__));

        assert (cell_x >= 0);
        assert (cell_z >= 0);
        assert (cell_x < static_cast<int>(res_x_));
        assert (cell_z < static_cast<int>(res_z_));

        const auto voxelToX = [&](real x) -> real { return left_  + x * ires_x_*width_; };
        const auto voxelToZ = [&](real z) -> real { return front_ + z * ires_z_*depth_; };

        real tmax_x = (voxelToX(cell_x+(int)positive_x) - gridinter_x) * id_x,
             tmax_z = (voxelToZ(cell_z+(int)positive_z) - gridinter_z) * id_z;

        int X = cell_x,
            Z = cell_z;
        Quad const * ph = this->ph (cell_x, cell_z);

        real t;
        Quad::Plane const *p;
        while (1) {
                if ((p = ph->intersect (ray, t)))
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
               Intersection (t, this->material_, DifferentialGeometry (p->normal,
                                                                       p->normal,
                                                                       p->u,
                                                                       p->v)) :
               Intersection::Optional();
}
} } }

#endif // PATCH_H_INCLUDED_20110826
