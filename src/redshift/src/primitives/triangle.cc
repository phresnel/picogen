//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../../include/primitives/triangle.hh"
#include "../../include/sampling.hh"

namespace {
        using redshift::real_t;
        using redshift::Point;
        using redshift::Vector;
        using redshift::Normal;
        using redshift::Ray;
        using redshift::vector_cast;

        static const real_t tri_eps = 0.00000001;
        static int
        raytri_intersect (
            const Ray &ray,
            const Point &a, const Point &b, const Point &c,
            real_t &t, real_t &u, real_t &v,
            Normal &normal_
        ) {
            Vector vect0, vect1, nvect, normal;
            real_t det, inv_det;

            vect0 = b - a;
            vect1 = c - a;

            normal = cross (vect0, vect1);
            det = -(ray.direction * normal);

            //---------

            /* if determinant is near zero, ray is parallel to the plane of triangle */
            if (det > -tri_eps && det < tri_eps) return 0;

            /* calculate vector from ray origin to a */
            //SUB(vect0,a,orig);
            vect0 = a - ray.position;

            /* normal vector used to calculate u and v parameters */
            //CROSS(nvect,dir,vect0);
            nvect = cross (ray.direction, vect0);

            inv_det = 1.0 / det;
            /* calculate vector from ray origin to b*/
            //SUB(vect1,b,orig);
            vect1 = b - ray.position;

            /* calculate v parameter and test bounds */
            //*v = - DOT(vect1,nvect) * inv_det;
            v = - (vect1*nvect*inv_det);

            if (v < 0.0 || v > 1.0) return 0;

            /* calculate vector from ray origin to c*/
            //SUB(vect1,c,orig);
            vect1 = c - ray.position;

            /* calculate v parameter and test bounds */
            //*u = DOT(vect1,nvect) * inv_det;
            u = vect1*nvect*inv_det;

            if (u < 0.0 || u + v > 1.0) return 0;

            /* calculate t, ray intersects triangle */
            //*t = - DOT(vect0,normal) * inv_det;
            t = - (vect0* normal * inv_det);

            //---------
            // pretty crappy but sometimes useful wireframe mode
            //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

            if (t < 0)
                return 0;
            normal_ = vector_cast<Normal>(normalize (normal));
            if (ray.direction * normal > 0.0)
                return -1;
            return 1;
        }
}

namespace redshift { namespace primitive {



Triangle::Triangle (Vertex A, Vertex B, Vertex C)
: A(A), B(B), C(C)
{
}



Triangle::~Triangle () {
}



BoundingBox Triangle::boundingBox () const {
        BoundingBox ret;
        ret = merge (ret, A.position);
        ret = merge (ret, B.position);
        ret = merge (ret, C.position);
        return ret;
}



bool Triangle::doesIntersect (Ray const &ray) const {
        real_t dum0, dum1, dum2;
        Normal dum3;
        return 0 != raytri_intersect(ray,
                                     A.position, B.position, C.position,
                                     dum0, dum1, dum2,
                                     dum3);
}



optional<Intersection>
 Triangle::intersect(Ray const &ray) const {
        real_t t, u, v;
        Normal normal;
        const int does = raytri_intersect(ray,
                                          A.position, B.position, C.position,
                                          t, u, v,
                                          normal);
        if (does != 0) {
                const Vector du = does>0?
                        normalize (B.position-A.position) :
                        normalize (C.position-A.position);
                const Vector dv = does>0?
                        normalize (C.position-A.position) :
                        normalize (B.position-A.position);
                const Normal n = does>0?
                        normal :
                        -normal;

                const DifferentialGeometry dg (
                        ray(t),
                        n,
                        du,
                        dv,
                        Vector(), Vector()
                );
                return Intersection (*this, dg);
        } else {
                return false;
        }
}



} }
