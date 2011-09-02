
#include "patch.h"
#include "../ray.h"
#include "../materials/lambertmaterial.h"
#include <QDebug>
#include <algorithm>

namespace picogen { namespace cracker {


namespace detail {

        static const real tri_eps = 0.00000001;
        static int
        raytri_intersect (
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
}


namespace detail {


Patch::Patch (real left, real right,
              real front, real back,
              unsigned int res_x,
              unsigned int res_z,
              std::function<real (real, real)> fun,
              real &y_min, real &y_max)
: left_(left), right_(right), front_(front), back_(back)
, res_x_(res_x), res_z_(res_z)
, stride_(res_x_+1)
, h_(new Vector [(res_x+1)*(res_z+1)])
{
        assert (res_x >= 2);
        assert (res_z >= 2);
        assert (left < right);
        assert (front < back);

        y_min = std::numeric_limits<real>::max();
        y_max = -y_min;
        for (unsigned int uz=0; uz<=res_z; ++uz) {
                for (unsigned int ux=0; ux<=res_x; ++ux) {
                        const real u = ux/static_cast<real>(res_x);
                        const real v = uz/static_cast<real>(res_z);

                        const real x = (1-u)*left + u*right;
                        const real z = (1-v)*front + v*back;
                        const real y = fun(x,z);

                        if (y < y_min) y_min = y;
                        if (y > y_max) y_max = y;

                        //h(ux,uz) = y;
                        h(ux,uz) = Vector(x,y,z);
                }
        }

        /*exactBB = BoundingBox (Point(left, y_min, front),
                               Point(right, y_max, back));*/

        material_.reset (new LambertMaterial(Color::FromRgb(
                                                     0.3,0.8,0.4)));
}

Patch::~Patch() {
        delete [] h_;
}

Intersection::Optional Patch::operator() (Ray const &ray,
                                          real min, real max) const
{
        if (true) {
                return intersect_amanatides (ray, min, max);
        } else {
                const bool pro_x = ray.direction().x() >= 0;
                const bool pro_z = ray.direction().z() >= 0;

                if (pro_x & pro_z) {
                        return intersect<1,1> (ray, 0, res_x_,
                                                    0, res_z_);
                }
                if (pro_x & !pro_z) {
                        return intersect<1,-1> (ray, 0, res_x_,
                                                    res_z_-1, -1);
                }
                if (!pro_x & pro_z) {
                        intersect<-1,1> (ray, res_x_-1, -1,
                                              0,        res_z_);
                }
                return intersect<-1,-1> (ray, res_x_-1, -1,
                                              res_z_-1, -1);
        }
}



template <int step_x, int step_z>
Intersection::Optional Patch::intersect (Ray const &ray,
                                         int left, int right,
                                         int front, int back) const
{
        for (int uz=front; uz!=back; uz+=step_z) {
                for (int ux=left; ux!=right; ux+=step_x) {
                        const Vector *A = ph(ux,   uz),
                                     *B = ph(ux,   uz+1),
                                     *C = ph(ux+1, uz),
                                     *D = ph(ux+1, uz+1);
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
                }
        }

        return Intersection::Optional();
}


Intersection::Optional Patch::intersect_quad (Ray const &ray,
                                              int X, int Z) const {
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


Intersection::Optional Patch::intersect_amanatides (Ray const &ray,
                                                    real minT,
                                                    real maxT) const
{
        const real p_x = ray.origin().x(),
                   p_z = ray.origin().z();
        const real d_x = ray.direction().x(),
                   d_z = ray.direction().z();

        // dda

        const real gridinter_x = p_x + d_x * minT,
                   gridinter_z = p_z + d_z * minT;

        const real width_ = right_ - left_;
        const real depth_ = back_ - front_;

        const bool positive_x = d_x>=0,
                   positive_z = d_z>=0;
        const int outX = positive_x ? res_x_ : -1,
                  outZ = positive_z ? res_z_ : -1;
        const int stepX = positive_x ? 1 : -1,
                  stepZ = positive_z ? 1 : -1;
        const real voxelWidth = width_ / res_x_,
                   voxelDepth = depth_ / res_z_;
        const real tdelta_x = stepX * voxelWidth / d_x,
                   tdelta_z = stepZ * voxelDepth / d_z;


        // subtracting one from res_xy below gives in-bounds results, but yields
        // some superfluous voxel touches
        // TODO: check whether a max(0,cell_x) is needed

        // Minimize to account for floating point inaccuracies (for some bounding
        // boxes, this is need, for some not)
        const int cell_x = std::min(int(res_x_)-1,
                                    int((gridinter_x - left_ ) * (res_x_/width_))),
                  cell_z = std::min(int(res_z_)-1,
                                    int((gridinter_z - front_) * (res_z_/depth_)));

        assert (cell_x >= 0);
        assert (cell_z >= 0);
        assert (cell_x < res_x_);
        assert (cell_z < res_z_);

        const auto voxelToX = [&](real x) -> real { return left_  + (x / (real)(res_x_)) * width_; };
        const auto voxelToZ = [&](real z) -> real { return front_ + (z / (real)(res_z_)) * depth_; };

        real tmax_x = (voxelToX(cell_x+(int)positive_x) - gridinter_x) / d_x,
             tmax_z = (voxelToZ(cell_z+(int)positive_z) - gridinter_z) / d_z;

        int X = cell_x,
            Z = cell_z;

        while (1) {
                if (const Intersection::Optional p = intersect_quad (ray, X, Z)) {
                        return p;
                }

                // TODO: can potentially optimize this:
                //  tmax_x += (tmax_x<tmax_z) * tdelta_x
                // etc.
                if (tmax_x < tmax_z) {
                        X += stepX;
                        if (X == outX)  break;
                        tmax_x += tdelta_x;
                } else {
                        Z += stepZ;
                        if (Z == outZ) break;
                        tmax_z += tdelta_z;
                }
        }
        return Intersection::Optional();
}

Vector const * Patch::ph (unsigned int x, unsigned z) const {
        return h_ + x + z*stride_;
}
Vector& Patch::h (unsigned int x, unsigned z) {
        return h_[x + z*stride_];
}

} } }
