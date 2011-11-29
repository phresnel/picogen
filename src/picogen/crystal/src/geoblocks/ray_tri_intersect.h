#ifndef RAY_TRI_INTERSECT_H
#define RAY_TRI_INTERSECT_H

#include "../crystal_math.h"
namespace crystal { namespace geoblocks {
        static const real tri_eps = 0.001;
        inline int
        raytri_intersect (
            const Ray &ray,
            const Point &a, const Point &b, const Point &c,
            real &t, real &u, real &v,
            Normal &normal_
        ) {
            Vector vect0, vect1, nvect, normal;
            real det, inv_det;

            vect0 = b - a;
            vect1 = c - a;

            normal = cross (vect0, vect1);
            det = -dot((Vector)ray.direction, normal);

            //---------

            /* if determinant is near zero, ray is parallel to the plane of triangle */
            if (det > -tri_eps && det < tri_eps) return 0;

            /* calculate vector from ray origin to a */
            //SUB(vect0,a,orig);
            vect0 = a - ray.origin;

            /* normal vector used to calculate u and v parameters */
            //CROSS(nvect,dir,vect0);
            nvect = cross ((Vector)ray.direction, vect0);

            inv_det = 1.0 / det;
            /* calculate vector from ray origin to b*/
            //SUB(vect1,b,orig);
            vect1 = b - ray.origin;

            /* calculate v parameter and test bounds */
            //*v = - DOT(vect1,nvect) * inv_det;
            v = -dot(vect1, nvect) * inv_det;

            if (v < 0.0 || v > 1.0) return 0;

            /* calculate vector from ray origin to c*/
            //SUB(vect1,c,orig);
            vect1 = c - ray.origin;

            /* calculate v parameter and test bounds */
            //*u = DOT(vect1,nvect) * inv_det;
            u = dot(vect1,nvect) * inv_det;

            if (u < 0.0 || u + v > 1.0) return 0;

            /* calculate t, ray intersects triangle */
            //*t = - DOT(vect0,normal) * inv_det;
            t = -dot (vect0, normal) * inv_det;

            //---------
            // pretty crappy but sometimes useful wireframe mode
            //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

            if (t < 0)
                return 0;
            normal_ = Normal (normal.x, normal.y, normal.z);
            if (dot ((Vector)ray.direction, normal) > 0.0)
                return -1;
            return 1;
        }
} }

#endif // RAY_TRI_INTERSECT_H
