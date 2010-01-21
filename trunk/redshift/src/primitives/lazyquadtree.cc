//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#include "../../include/primitives/lazyquadtree.hh"
#include "../../include/material/lambertian.hh"

//#############################################################################
// LazyQuadtreeImpl
//#############################################################################
namespace redshift { namespace primitive {


namespace lazyquadtree {
        struct Vertex {
                real_t u,v;
                real_t h;
                Vertex (real_t u, real_t v, real_t h)
                : u(u), v(v), h(h) {}
                Vertex (Vertex &vtx)
                : u(vtx.u), v(vtx.v), h(vtx.h) {}
                Vertex & operator = (Vertex const &vtx) {
                        u = vtx.u;
                        v = vtx.v;
                        h = vtx.h;
                        return *this;
                }
                Vertex() {}
        };
        
        
                
        static const real_t tri_eps = 0.00000001;
        static int
        raytri_intersect (
            const Ray &ray,
            const Vector &a, const Vector &b, const Vector &c,
            real_t &t, real_t &u, real_t &v,
            Normal &normal_
        ) {
            Vector vect0, vect1, nvect, normal;
            real_t det, inv_det;


            //SUB(vect0, b,a)
            vect0 = b - a;

            //SUB(vect1, c,a)
            vect1 = c - a;

            //CROSS(normal, vect0, vect1);
            normal = cross (vect0, vect1);

            /* orientation of the ray with respect to the triangle's normal,
               also used to calculate output parameters*/
            //det = - DOT(dir,normal);
            det = -(ray.direction * normal);//-( p_ray->direction.x*normal->x + p_ray->direction.y*normal->y + p_ray->direction.z*normal->z );

            //---------

            /* if determinant is near zero, ray is parallel to the plane of triangle */
            if (det > -tri_eps && det < tri_eps) return 0;

            /* calculate vector from ray origin to a */
            //SUB(vect0,a,orig);
            vect0 = a - vector_cast<Vector>(ray.position);

            /* normal vector used to calculate u and v parameters */
            //CROSS(nvect,dir,vect0);
            nvect = cross (ray.direction, vect0);

            inv_det = 1.0 / det;
            /* calculate vector from ray origin to b*/
            //SUB(vect1,b,orig);
            vect1 = b - vector_cast<Vector>(ray.position);

            /* calculate v parameter and test bounds */
            //*v = - DOT(vect1,nvect) * inv_det;
            v = - (vect1*nvect*inv_det);

            if (v < 0.0 || v > 1.0) return 0;

            /* calculate vector from ray origin to c*/
            //SUB(vect1,c,orig);
            vect1 = c - vector_cast<Vector>(ray.position);

            /* calculate v parameter and test bounds */
            //*u = DOT(vect1,nvect) * inv_det;
            u = vect1*nvect*inv_det;

            if (u < 0.0 || u + v > 1.0) return 0;

            /* calculate t, ray intersects triangle */
            //*t = - DOT(vect0,normal) * inv_det;
            t = - (vect0* normal * inv_det);

            //---------
            // pretty crappy but sometimes useful wireframe mode
            /*if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9))
                    return 0;*/
            if (t < 0)
                return 0;
            normal_ = vector_cast<Normal>(normalize (normal));
            if (ray.direction * normal > 0.0)
                return -1;
            return 1;
        }



        class Node {
                BoundingBox aabb;
                real_t min_h, max_h;
                const HeightFunction &fun;
                mutable Node *children[4];
                Vertex vertices[9];
                const int maxRecursion; // When 0, max recursion is reached.
                real_t center_x, center_z;

                Vertex &vertex (int u, int v) {
                        return vertices[v*3+u];
                }
                const Vertex &vertex (int u, int v) const {
                        return vertices[v*3+u];
                }
                
                pair<real_t,Normal> intersect_triangle (
                        Ray const & ray, 
                        int u0,int v0, int u1,int v1, int u2,int v2
                ) const {
                        const Vertex & a_ = vertex(u0,v0);
                        const Vertex & b_ = vertex(u1,v1);
                        const Vertex & c_ = vertex(u2,v2);
                        const Vector a (a_.u, a_.h, a_.v);
                        const Vector b (b_.u, b_.h, b_.v);
                        const Vector c (c_.u, c_.h, c_.v);

                        real_t t=-1, u, v;
                        Normal normal;
                        const bool does_intersect = 
                                0 != raytri_intersect (
                                        ray,
                                        a, b, c,
                                        t, u, v,
                                        normal
                                );
                        return make_pair(t, normal);
                }

                void create_child (int index) const {
                        const real_t left  = scalar_cast<real_t> (aabb.getMinimumX());
                        const real_t right = scalar_cast<real_t> (aabb.getMaximumX());
                        const real_t front = scalar_cast<real_t> (aabb.getMinimumZ());
                        const real_t back  = scalar_cast<real_t> (aabb.getMaximumZ());
                        const real_t bottom= scalar_cast<real_t> (aabb.getMinimumY());
                        const real_t top   = scalar_cast<real_t> (aabb.getMaximumY());
                        switch (index) {
                        case 0:
                                children[0] = new Node (
                                        BoundingBox(
                                         vector_cast<Point>(Vector(left, bottom, front)),
                                         vector_cast<Point>(Vector(center_x, top, center_z))
                                        ),
                                        fun,
                                        maxRecursion-1
                                );
                                break;
                        case 1:
                                children[1] = new Node (
                                        BoundingBox(
                                                vector_cast<Point>(Vector(center_x, bottom, front)),
                                                vector_cast<Point>(Vector(right, top, center_z))
                                        ),
                                        fun,
                                        maxRecursion-1
                                );
                                break;
                        case 2:
                                children[2] = new Node (
                                        BoundingBox(
                                                vector_cast<Point>(Vector(left, bottom, center_z)),
                                                vector_cast<Point>(Vector(center_x, top, back))
                                        ),
                                        fun,
                                        maxRecursion-1
                                );
                                break;
                        case 3:
                                children[3] = new Node (
                                        BoundingBox(
                                                vector_cast<Point>(Vector(center_x, bottom, center_z)),
                                                vector_cast<Point>(Vector(right, top, back))
                                        ),
                                        fun,
                                        maxRecursion-1
                                );
                                break;
                        };
                }

                optional<DifferentialGeometry> traverse (Ray const &ray, int child, real_t t0, real_t t1) const {
                        if (t0<=t1) {
                                if (!children[child]) create_child (child);
                                if (optional<DifferentialGeometry> dg =
                                        children[child]->intersect(ray, t0, t1))
                                        return dg;
                        }
                        return false;
                }
        public:
                Node (
                        const BoundingBox &box,
                        const HeightFunction &fun,
                        const int maxRecursion
                )
                : aabb(box)
                , fun (fun)
                , maxRecursion(maxRecursion)
                {
                        for (int i=0; i<4; ++i) {
                                children[i] = 0;
                        }

                        /*     0   1   2
                            max_z    max_z
                        2 min_x+---+---+max_x
                               |   |   |
                        1      +---c---+
                               |   |   |
                        0 min_x+---+---+max_x
                           min_z    min_z
                        */
                        const real_t min_x = scalar_cast<real_t>(box.getMinimumX());
                        const real_t max_x = scalar_cast<real_t>(box.getMaximumX());
                        const real_t c_x   = (min_x + max_x) / 2;
                        const real_t min_z = scalar_cast<real_t>(box.getMinimumZ());
                        const real_t max_z = scalar_cast<real_t>(box.getMaximumZ());
                        const real_t c_z   = (min_z + max_z) / 2;
                        vertex(0,0) = Vertex (min_x, min_z, fun(min_x, min_z));
                        vertex(1,0) = Vertex (c_x,   min_z, fun(c_x,   min_z));
                        vertex(2,0) = Vertex (max_x, min_z, fun(max_x, min_z));
                        vertex(0,1) = Vertex (min_x,   c_z, fun(min_x, c_z));
                        vertex(1,1) = Vertex (c_x,     c_z, fun(c_x,   c_z));
                        vertex(2,1) = Vertex (max_x,   c_z, fun(max_x, c_z));
                        vertex(0,2) = Vertex (min_x, max_z, fun(min_x, max_z));
                        vertex(1,2) = Vertex (c_x,   max_z, fun(c_x,   max_z));
                        vertex(2,2) = Vertex (max_x, max_z, fun(max_x, max_z));
                        
                        center_x = c_x;
                        center_z = c_z;

                        // refine bounding box height
                        /*min_h = constants::real_max;
                        max_h = -constants::real_max;
                        for (int u=0; u<2; ++u)
                        for (int v=0; v<2; ++v) {
                                const real_t h = vertex(u,v).h;
                                if (h < min_h) min_h = h;
                                if (h > max_h) max_h = h;
                        }
                        aabb.setMinimumY(scalar_cast<BoundingBox::scalar_t>(min_h));
                        aabb.setMaximumY(scalar_cast<BoundingBox::scalar_t>(max_h));*/
                        min_h = scalar_cast<real_t>(aabb.getMinimumY());
                        max_h = scalar_cast<real_t>(aabb.getMaximumY());
                }
                
                ~Node () {
                        for (int i=0; i<4; ++i) {
                                if (children[i])
                                        delete children[i];
                        }
                }

                optional<DifferentialGeometry> intersect (Ray const &ray, real_t minT, real_t maxT) const {
                        struct Triangle {
                                Vertex &a, &b, &c;
                        };
                        // We can assume minT and maxT to be a correct interval on the xz plane.
                        // But we got to check for vertical intersection now.
                        /*const real_t p_h = scalar_cast<real_t>(ray.position.y);
                        const real_t min_h = p_h + minT * ray.direction.y;
                        const real_t max_h = p_h + maxT * ray.direction.y;
                        if (((min_h < this->min_h) & (max_h < this->min_h))
                           |((min_h > this->max_h) & (max_h > this->max_h))
                        )
                                return false;*/
                        /*if (!does_intersect<false>(ray,aabb))
                                return false;*/

                        if (maxRecursion == 0) {
                                pair<real_t,Normal> i;
                                if (0 < (i = intersect_triangle (ray, 0,0, 0,1, 1,1)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 0,0, 1,1, 1,0)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 1,0, 1,1, 2,1)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 1,0, 2,1, 2,0)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 0,1, 0,2, 1,2)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 0,1, 1,2, 1,1)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 1,1, 1,2, 2,2)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                if (0 < (i = intersect_triangle (ray, 1,1, 2,2, 2,1)).first)
                                        return DifferentialGeometry(
                                                i.first,ray(i.first),i.second);
                                return false;
                        }
                        // Find out which one to traverse.
                        const bool d_right = ray.direction.x >= 0;
                        const bool d_up    = ray.direction.z >= 0;
                        const real_t d_x = (center_x - scalar_cast<real_t>(ray.position.x)) / ray.direction.x;
                        const real_t d_z = (center_z - scalar_cast<real_t>(ray.position.z)) / ray.direction.z;
                        const bool upper_three = d_x > d_z;

                        // +----+----+
                        // | 2  | 3  |
                        // -----+-----
                        // | 0  | 1  |
                        // +----+----+

                        struct {
                                int child;
                                real_t t0, t1;
                        } t[3];
                        if (d_right & d_up) {
                                if (upper_three) {
                                        // 0, 2, 3
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 2; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 3; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 0, 1, 3
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 1; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 3; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        if (!d_right & d_up) {
                                if (upper_three) {
                                        // 1, 3, 2
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 3; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 2; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 1, 0, 2
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 0; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 2; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        // TODO: implement other cases
                        if (d_right & !d_up) {
                                if (upper_three) {
                                        // 2, 0, 1
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 0; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 1; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 2, 3, 1
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 3; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 1; t[2].t0 = d_z;  t[2].t1 = maxT;                                        
                                }
                        }
                        if (!d_right & !d_up) {
                                if (upper_three) {
                                        // 3, 1, 0
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 1; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 0; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 3, 2, 0
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 2; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 0; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        optional<DifferentialGeometry> dg;
                        for (int i=0; i<3; ++i)
                                if (dg = traverse (ray, t[i].child, t[i].t0, t[i].t1))
                                        return dg;
                        return false;
                }                
        };
}






class LazyQuadtreeImpl {
public:
        LazyQuadtreeImpl (
                shared_ptr<HeightFunction const> fun,
                real_t size,
                shared_ptr<HeightFunction const> distortionFun_
        )
        : fun(fun)
        , primaryBB(initBB (size,min(1000.f,(size*size*size)/100)))
        , primaryNode(primaryBB, *fun.get(),4)
        , distortionFun(distortionFun_)
        {}



        bool doesIntersect (Ray const &ray) const {
                //return does_intersect<false> (ray, primaryBB);
                return intersect(ray);
        }



        optional<DifferentialGeometry> intersect (Ray const &ray) const {
                const optional<tuple<real_t,real_t> > i = 
                                        redshift::intersect<false>(ray,primaryBB);
                if (!i) return false;

                const real_t minT = get<0>(*i);
                const real_t maxT = get<1>(*i);

                /*return DifferentialGeometry(
                        minT,
                        ray(minT),
                        Normal(0,1,0)
                );*/
                const optional<DifferentialGeometry> dg_ = primaryNode.intersect (ray, minT, maxT);
                if (!dg_) return false;
                const DifferentialGeometry dg =*dg_;

                const real_t f = dg.getNormal().y;/* * (*distortionFun)(
                        scalar_cast<real_t>(dg.getCenter().x), 
                        scalar_cast<real_t>(dg.getCenter().z)
                );*/
                return DifferentialGeometry(
                        dg.getDistance(),
                        dg.getCenter(),
                        normalize(Normal(dg.getNormal().x,f,dg.getNormal().z))
                );
        }



private:

        shared_ptr<HeightFunction const> fun;
        BoundingBox primaryBB;
        lazyquadtree::Node primaryNode;
        shared_ptr<HeightFunction const> distortionFun;



        BoundingBox initBB(const real_t size, const int numSamples) const {
                real_t minh = 10000000.0f , maxh = -10000000.0f;
                MersenneTwister<real_t,0,1> mt (4654641);
                const real_t size05 = size/2;
                for (int x=0; x<numSamples; ++x) {
                        const real_t uv[] = {
                                mt.rand()*size-size05,
                                mt.rand()*size-size05
                        };
                        const real_t h = (*fun)(uv[0],uv[1]);
                        if (h < minh) minh = h;
                        if (h > maxh) maxh = h;
                }
                return BoundingBox (
                        Point(
                                scalar_cast<BoundingBox::scalar_t>(-size05),
                                scalar_cast<BoundingBox::scalar_t>(minh),
                                scalar_cast<BoundingBox::scalar_t>(-size05)
                        ),
                        Point(
                                scalar_cast<BoundingBox::scalar_t>(size05),
                                scalar_cast<BoundingBox::scalar_t>(maxh),
                                scalar_cast<BoundingBox::scalar_t>(size05)
                        )
                );
        }
};
} }






//#############################################################################
// LazyQuadtree
//#############################################################################
namespace redshift { namespace primitive {



LazyQuadtree::LazyQuadtree (
        shared_ptr<HeightFunction const> fun,
        real_t size,
        shared_ptr<HeightFunction const> distortionFun_
)
: impl (new LazyQuadtreeImpl (fun, size, distortionFun_))
, mt(shared_ptr<MersenneTwister<real_t,0,1> > (new MersenneTwister<real_t,0,1>))
, distortionFun(distortionFun_)
{
}



LazyQuadtree::~LazyQuadtree () {
}



bool LazyQuadtree::doesIntersect (RayDifferential const &ray) const {
        return impl->doesIntersect (ray);
}



bool LazyQuadtree::doesIntersect (Ray const &ray) const {
        return impl->doesIntersect (ray);
}



optional<Intersection>
 LazyQuadtree::intersect(RayDifferential const &ray) const {

        /*optional<DifferentialGeometry> 
                       i(sphereData.intersect(ray));
        if (i) { 
                return Intersection (shared_from_this(), *i);
        } else {
                return false;
        }*/
        const optional<DifferentialGeometry> dg =impl->intersect (ray);
        if (!dg) return false;
        return Intersection (
                shared_from_this(),
                *dg
        );
}



shared_ptr<Bsdf> LazyQuadtree::getBsdf(
        const DifferentialGeometry & dgGeom
) const {
        return shared_ptr<Bsdf> (new bsdf::Lambertian (
                dgGeom, 
                dgGeom.getNormal(), 
                Color::fromRgb(1,1,1), 
                mt/*,
                distortionFun*/
        ));
}



} }
