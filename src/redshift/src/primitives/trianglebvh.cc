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

#include "../../include/constants.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/primitives/trianglebvh.hh"
#include "../../include/material/transmissivelambertian.hh"

#include <algorithm>


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


        static int
        raytri_intersect (
            const Ray &ray,
            const Point &a, const Point &b, const Point &c,
            real_t &t, real_t &u, real_t &v,
            const Vector &normal_,
            const Vector &normalizedNormal_
        ) {
            Vector vect0, vect1, nvect;
            real_t det, inv_det;

            vect0 = b - a;
            vect1 = c - a;

            det = -(ray.direction * normal_);

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
            t = - (vect0* normal_ * inv_det);

            //---------
            // pretty crappy but sometimes useful wireframe mode
            //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

            if (t < 0)
                return 0;
            if (ray.direction * normalizedNormal_ > 0.0)
                return -1;
            return 1;
        }
}



namespace redshift { namespace primitive {

//-----------------------------------------------------------------------------
// Node
//-----------------------------------------------------------------------------
struct TriangleBvhTri : BoundPrimitive {
        Triangle::Vertex A, B, C;
        Vector U, V;
        Vector normal;
        Vector normalizedNormal;
        shared_ptr<ColorTexture> texture;

        TriangleBvhTri (Triangle tri)
        : A(tri.a()), B(tri.b()), C(tri.c())
        , U(B.position-A.position)
        , V(C.position-A.position)
        , normal(cross (U, V)), normalizedNormal (normalize(normal))
        , texture(tri.texture())
        {}

        TriangleBvhTri() {}

        BoundingBox boundingBox () const {
                BoundingBox ret;
                ret = merge (ret, A.position);
                ret = merge (ret, B.position);
                ret = merge (ret, C.position);
                return ret;
        }

        bool doesIntersect (Ray const &ray) const {
                real_t dum0, dum1, dum2;
                return 0 != raytri_intersect(ray,
                                             A.position, B.position, C.position,
                                             dum0, dum1, dum2,
                                             normal, normalizedNormal);
        }

        optional<Intersection> intersect(Ray const &ray) const {
                real_t t, u, v;
                const int does = raytri_intersect(ray,
                                                  A.position, B.position, C.position,
                                                  t, u, v,
                                                  normal, normalizedNormal);
                if (does != 0) {
                        const Vector du = does>0?
                                normalize (B.position-A.position) :
                                normalize (C.position-A.position);
                        const Vector dv = does>0?
                                normalize (C.position-A.position) :
                                normalize (B.position-A.position);
                        const Vector n = does>0?
                                normalizedNormal :
                                -normalizedNormal;

                        const real_t suv = 1 - u - v;
                        const real_t U = suv*A.textureCoordinates.s
                                       +   u*B.textureCoordinates.s
                                       +   v*C.textureCoordinates.s;
                        const real_t V = suv*A.textureCoordinates.t
                                       +   u*B.textureCoordinates.t
                                       +   v*C.textureCoordinates.t;

                        const DifferentialGeometry dg (
                                ray(t),
                                vector_cast<Normal>(n),
                                du,
                                dv,
                                Vector(), Vector(),
                                U, V, (unsigned char)(0)
                        );

                        if (texture && texture->alpha(dg)<0.5)
                                return false;
                        return Intersection (*this, dg);
                } else {
                        return false;
                }
        }

        shared_ptr<Bsdf> getBsdf(
                const DifferentialGeometry & dgGeom
        ) const {
                shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));

                real_t u = dgGeom.u();
                real_t v = dgGeom.v();
                v = v - (int)v;
                u = u - (int)u;

                bsdf->add (shared_ptr<Bxdf>(new bsdf::Lambertian (
                                //texture->color(dgGeom),
                                Color(1)//, 1.0
                )));
                return bsdf;
        }
};

class TriangleBvhTriSort {
        unsigned int axis;
public:
        TriangleBvhTriSort (unsigned int axis) : axis(axis) {}
        bool operator() (TriangleBvhTri const &lhs,
                         TriangleBvhTri const &rhs) const
        {
                return lhs.boundingBox().center(axis)
                     < rhs.boundingBox().center(axis);
        }
};
struct TriangleBvhNode {
        TriangleBvhTri *from, *to;
        scoped_ptr<TriangleBvhNode> childA, childB;
        BoundingBox boundingBox;

        TriangleBvhNode (TriangleBvhTri *from, TriangleBvhTri *to)
        : from(from), to(to)
        {}

        size_t size() const {
                return to-from;
        }
        bool empty() const {
                return to == from;
        }

        void compile() {

                boundingBox.reset();
                for (TriangleBvhTri *it = from; it!=to; ++it) {
                        boundingBox = merge (boundingBox,
                                             it->boundingBox());
                }

                if (size() <= 5) {
                        return;
                }

                int splitAxis_ = 0;
                if (boundingBox.size(1) > boundingBox.size(splitAxis_)) splitAxis_ = 1;
                if (boundingBox.size(2) > boundingBox.size(splitAxis_)) splitAxis_ = 2;
                const int splitAxis = splitAxis_;
                const real_t center = boundingBox.center(splitAxis);

                std::sort (from, to, TriangleBvhTriSort(splitAxis));

                TriangleBvhTri* pivot = to;
                for (TriangleBvhTri* it = from; it != to; ++it) {
                        if (it->boundingBox().center(splitAxis) > center) {
                                pivot = it;
                                break;
                        }
                }

                childA.reset(new TriangleBvhNode(from, pivot));
                childB.reset(new TriangleBvhNode(pivot, to));

                if (childA->empty() || childB->empty()) {
                        childA.reset();
                        childB.reset();
                } else {
                        from = to = 0;
                        childA->compile();
                        childB->compile();
                }
        }

        bool doesIntersect (Ray const &ray) const {
                if (!does_intersect<false>(ray, boundingBox))
                        return false;
                if (childA && childA->doesIntersect(ray))
                        return true;
                if (childB && childB->doesIntersect(ray))
                        return true;

                for (TriangleBvhTri* it = from; it != to; ++it) {
                        if (it->doesIntersect((ray)))
                            return true;
                }


                return false;
        }

        bool intersectsBound(Ray const &ray) const {
                return does_intersect<false>(ray, boundingBox);
        }

        real_t intersectBound (Ray const &ray) const {
                const optional<tuple<real_t,real_t> > t =
                        kallisto::intersect<false>(ray, boundingBox);
                if (!t) return constants::infinity;
                return get<0>(*t);
        }

        bool hasChildren() const {
                return (bool)childA || (bool)childB;
        }

        optional<Intersection> intersect(Ray const &ray) const {

                real_t nearest = constants::real_max, tmp;
                optional<Intersection> nearestI, tmpI;

                /*for (CIt it=primitives.begin();
                        it!=primitives.end(); ++it
                ) {
                        tmpI = it->intersect (ray);
                        if (tmpI && (tmp=tmpI->getDistance()) < nearest) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }
                }*/
                for (TriangleBvhTri* it = from; it != to; ++it) {
                        tmpI = it->intersect (ray);
                        if (tmpI
                            && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }
                }

                if (hasChildren()) {
                        const real_t min_t[2] = {
                                childA->intersectBound(ray),
                                childB->intersectBound(ray)
                        };
                        const TriangleBvhNode* children[2] = {
                                childA.get(),
                                childB.get()
                        };
                        const int near = min_t[0] < min_t[1] ? 0 : 1;
                        const int far = 1 - near;

                        if (min_t[near] < nearest)
                        if ((tmpI = children[near]->intersect(ray))
                         && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }

                        if (min_t[far] < nearest)
                        if ((tmpI = children[far]->intersect(ray))
                         && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }
                }
                return nearestI;
        }

private:
        TriangleBvhNode (TriangleBvhNode const&);
        TriangleBvhNode& operator= (TriangleBvhNode const&);
};


//-----------------------------------------------------------------------------
// TriangleBvh
//-----------------------------------------------------------------------------
TriangleBvh::TriangleBvh(TriangleBvhTri *triangles,
                         shared_ptr<TriangleBvhNode> root
) : triangles(triangles), root(root) {
}



TriangleBvh::~TriangleBvh() {
        delete [] triangles;
}



BoundingBox TriangleBvh::boundingBox() const {
        return root->boundingBox;
}



bool TriangleBvh::doesIntersect (Ray const &ray) const {
        return root->doesIntersect(ray);
}



optional<Intersection> TriangleBvh::intersect(Ray const &ray) const {
        if (!root->intersectsBound(ray))
                return optional<Intersection>();
        return root->intersect(ray);
}



//-----------------------------------------------------------------------------
// Builder
//-----------------------------------------------------------------------------
TriangleBvhBuilder::TriangleBvhBuilder() {
}



void TriangleBvhBuilder::add (Triangle prim) {
        triangles.push_back(prim);
}


shared_ptr<TriangleBvh> TriangleBvhBuilder::toTriangleBvh() {
        // Generic triangles to specialized ones.
        TriangleBvhTri *triangles = new TriangleBvhTri[this->triangles.size()];
        for (unsigned int u=0; u<this->triangles.size(); ++u)
                triangles[u] = this->triangles[u];

        std::cout << "  triangles: " << this->triangles.size() << std::endl;

        // Generate and compile root-node.
        shared_ptr<TriangleBvhNode> root = shared_ptr<TriangleBvhNode>(
            new TriangleBvhNode(triangles, triangles+this->triangles.size()));
        root->compile();

        // Create the BVH and get off.
        shared_ptr<TriangleBvh> ret = shared_ptr<TriangleBvh>(
                new TriangleBvh(triangles, root));
        return ret;
}


} }

