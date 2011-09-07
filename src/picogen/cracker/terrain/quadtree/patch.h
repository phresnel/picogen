#ifndef PATCH_H_INCLUDED_20110826
#define PATCH_H_INCLUDED_20110826

#include "../intersection.h"

namespace picogen { namespace cracker {
        class Ray;
} }

namespace picogen { namespace cracker { namespace detail {

        enum class XDirection { Left = -1,     Right = 1 };
        enum class ZDirection { Backward = -1, Forward = 1 };


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
                Vector *h_;

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

                Intersection::Optional intersect_quad (Ray const &ray, int X, int Z) const ;
                /*Intersection::Optional intersect_amanatides (Ray const &ray,
                                                   real minT,
                                                   real maxT) const;*/

                //template <XDir

                Vector const * ph (unsigned int x, unsigned z) const;
                Vector& h (unsigned int x, unsigned z);
        };

} } }


#include "ray.h"
#include "terrain/morton.h"

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

inline Intersection::Optional Patch::intersect_quad (Ray const &ray,
                                              int X, int Z) const {

        // TODO: morton indexing in ph(x,z)
        const Vector *A = ph(X,   Z),
                     *B = ph(X,   Z+1),
                     *C = ph(X+1, Z),
                     *D = ph(X+1, Z+1);

        const Vector *a,*b,*c;
        real t, tu, tv;
        Normal tn(0,1,0);
        if (0 != raytri_intersect(
                ray,
                *(a=A),
                *(b=B),
                *(c=C),
                t, tu, tv, tn)
        || 0 != raytri_intersect(
                ray,
                *(a=B),
                *(b=D),
                *c,
                t, tu, tv, tn)
        )
        {
                return Intersection (
                     t,
                     material_,
                     DifferentialGeometry(
                         tn, tn,
                         normalize<Normal>(*b-*a),
                         normalize<Normal>(*c-*a)));
        }
        return Intersection::Optional();
}



inline Vector const * Patch::ph (unsigned int x, unsigned z) const {
        return h_ + x + z*stride_;
}
inline Vector& Patch::h (unsigned int x, unsigned z) {
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
                positive_x = d_right==XDirection::Right,//d_x>=0
                positive_z = d_forward==ZDirection::Forward//d_z>=0
        };
        const int outX = positive_x ? res_x_ : -1,
                  outZ = positive_z ? res_z_ : -1;
        const int stepX = positive_x ? 1 : -1,
                  stepZ = positive_z ? 1 : -1;
        const real tdelta_x = stepX * voxelWidth_ * id_x,
                   tdelta_z = stepZ * voxelDepth_ * id_z;


        // subtracting one from res_xy below gives in-bounds results, but yields
        // some superfluous voxel touches
        // TODO: check whether a max(0,cell_x) is needed

        // Minimize to account for floating point inaccuracies (for some bounding
        // boxes, this is need, for some not)
        static real x__  = (res_x_*iwidth_);
        static real z__  = (res_z_*idepth_);

        const int cell_x = std::min(int(res_x_)-1,
                                    int((gridinter_x - left_ ) * x__)),
                  cell_z = std::min(int(res_z_)-1,
                                    int((gridinter_z - front_) * z__));

        assert (cell_x >= 0);
        assert (cell_z >= 0);
        assert (cell_x < res_x_);
        assert (cell_z < res_z_);

        const auto voxelToX = [&](real x) -> real { return left_  + x * ires_x_*width_; };
        const auto voxelToZ = [&](real z) -> real { return front_ + z * ires_z_*depth_; };

        real tmax_x = (voxelToX(cell_x+(int)positive_x) - gridinter_x) * id_x,
             tmax_z = (voxelToZ(cell_z+(int)positive_z) - gridinter_z) * id_z;

        int X = cell_x,
            Z = cell_z;

        while (1) {
                if (const Intersection::Optional p = intersect_quad (ray, X, Z)) {
                        return p;
                }

#if 0
                if (tmax_x < tmax_z) {
                        X += stepX;
                        if (X == outX)  break;
                        tmax_x += tdelta_x;
                } else {
                        Z += stepZ;
                        if (Z == outZ) break;
                        tmax_z += tdelta_z;
                }
#else
                const int x_lt_z = tmax_x < tmax_z;
                X      += x_lt_z ? stepX : 0;
                Z      += x_lt_z ? 0 : stepZ;
                tmax_x += x_lt_z ? tdelta_x : 0;
                tmax_z += x_lt_z ? 0 : tdelta_z;

                if ((X==outX) | (Z==outZ)) break;
#endif
        }
        return Intersection::Optional();
}
} } }

#endif // PATCH_H_INCLUDED_20110826
