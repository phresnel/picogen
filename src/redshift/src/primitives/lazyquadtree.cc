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
#include "kallisto/types/cache.hh"

#include <omp.h>

//#############################################################################
// LazyQuadtreeImpl
//#############################################################################
namespace redshift { namespace primitive {


namespace lazyquadtree {

        class Node;
        void clearNode (const Node *);
        typedef
                kallisto::cache::mru_cache_index<const lazyquadtree::Node*,
                                                 kallisto::cache::collect,
                                                 clearNode>
                NodeIndex;

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
            //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

            if (t < 0)
                return 0;
            normal_ = vector_cast<Normal>(normalize (normal));
            if (ray.direction * normal > 0.0)
                return -1;
            return 1;
        }



        struct NodeStaticParameters {
                NodeStaticParameters (const HeightFunction & fun, real_t lodFactor)
                : fun(fun), lodFactor(lodFactor) {}

                const HeightFunction &fun;
                const real_t lodFactor;
        };
        class Node {
                mutable Node *parent;
                mutable Node *children[4];

                BoundingBoxF aabb;
                mutable real_t min_h, max_h;

                Vector *vertices;

                real_t center_x, center_z;
                real_t diagonal;

                PointF cameraPosition; // TODO should be a reference or shared_ptr<>
                //mutable Mutex mute[4];
                mutable Mutex mute;

                mutable unsigned int lastUsedInScanline;

                struct {
                        bool isLeaf:1;
                        mutable bool hasExactBoundingBox:1 ; // once this is well defined, get rid of
                                                             // initializedChildCount
                        unsigned char maxRecursion : 6; // 0 = max recursion reached
                        unsigned char vertexCount : 4;
                };

                const NodeStaticParameters &staticParameters;



                real_t fun (real_t u, real_t v) const {
                        return staticParameters.fun (u, v);
                }

                pair<real_t,Normal> intersect_triangle (
                        Ray const & ray,
                        const Vector &a,
                        const Vector &b,
                        const Vector &c
                ) const {
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

                        ScopedLock sl (mute);
                        #pragma omp flush

                        const real_t left  = aabb.getMinimumX();
                        const real_t right = aabb.getMaximumX();
                        const real_t front = aabb.getMinimumZ();
                        const real_t back  = aabb.getMaximumZ();
                        const real_t bottom= aabb.getMinimumY();
                        const real_t top   = aabb.getMaximumY();

                        if (children[index])
                                return;
                        Node *tmp;
                        switch (index) {
                        case 0:
                                tmp = new Node (
                                        BoundingBoxF(
                                         PointF(left, bottom, front),
                                         PointF(center_x, top, center_z)
                                        ),
                                        maxRecursion-1,
                                        const_cast<Node*>(this),
                                        staticParameters
                                );
                                break;
                        case 1:
                                tmp = new Node (
                                        BoundingBoxF(
                                                PointF(center_x, bottom, front),
                                                PointF(right, top, center_z)
                                        ),
                                        maxRecursion-1,
                                        const_cast<Node*>(this),
                                        staticParameters
                                );
                                break;
                        case 2:
                                tmp = new Node (
                                        BoundingBoxF(
                                                PointF(left, bottom, center_z),
                                                PointF(center_x, top, back)
                                        ),
                                        maxRecursion-1,
                                        const_cast<Node*>(this),
                                        staticParameters
                                );
                                break;
                        case 3:
                                tmp = new Node (
                                        BoundingBoxF(
                                                PointF(center_x, bottom, center_z),
                                                PointF(right, top, back)
                                        ),
                                        maxRecursion-1,
                                        const_cast<Node*>(this),
                                        staticParameters
                                );
                                break;
                        };
                        tmp->prepare(cameraPosition, lastUsedInScanline);
                        children[index] = tmp;
                        #pragma omp flush
                        // TODO how to omp-flush children[i]?
                }

                optional<pair<real_t,Normal> > traverse (
                        RayDifferential const &ray,
                        int child,
                        real_t t0, real_t t1,
                        unsigned int scanline
                ) const {
                        if (t0<=t1) {
                                if (!children[child]) {
                                        create_child (child); // assumed to be safe upon multiple calls
                                }
                                if (optional<pair<real_t,Normal> > dg =
                                        children[child]->intersect(ray, t0, t1, scanline)
                                ) {
                                        return dg;
                                }
                        }
                        return false;
                }

                optional<pair<real_t,Normal> > intersectLeaf (RayDifferential const &ray) const {
                        pair<real_t,Normal> tmp, nearest=make_pair(constants::real_max, Normal());
                        const Vector &A = vertices[0];
                        for (int i=1; i<vertexCount-1; ++i) {
                                const Vector &B = vertices[i];
                                const Vector &C = vertices[1+i];
                                tmp = intersect_triangle (ray, A, B, C);
                                if ((0 < tmp.first)  & (tmp.first < nearest.first))
                                        nearest = tmp;
                        }
                        if (nearest.first < constants::real_max) {
                                return nearest;
                        }
                        return false;
                }

                void initBoundingBox () {
                        //return;
                        min_h = aabb.getMinimumY();
                        max_h = aabb.getMaximumY();
                        //diagonal = length (aabb.getMaximum()-aabb.getMinimum());
                        diagonal = std::sqrt(aabb.getWidth()*aabb.getWidth()
                                 + aabb.getDepth()*aabb.getDepth());
                }

                void refineBoundingBox () {
                        if (isLeaf) {
                                min_h = constants::real_max;
                                max_h = -constants::real_max;

                                for (int i=0; i<vertexCount; ++i) {
                                        const real_t h = vertices[i].y;
                                        if (h < min_h) min_h = h;
                                        if (h > max_h) max_h = h;
                                }

                                hasExactBoundingBox = true;

                                aabb.setMinimumY(min_h);
                                aabb.setMaximumY(max_h);
                                diagonal = length (aabb.getMaximum()-aabb.getMinimum());

                                // propagate to parent
                                if (parent) parent->refineBoundingBox();

                        } else {

                                for (int i = 0; i<4; ++i) {
                                        if (!children[i])
                                                return;
                                        if (!children[i]->hasExactBoundingBox)
                                                return;
                                }


                                min_h = constants::real_max;
                                max_h = -constants::real_max;

                                for (int i=0; i<4; ++i) {
                                        if (!children[i]) exit(0);
                                        if (children[i]->min_h < min_h)
                                                min_h = children[i]->min_h;
                                        if (children[i]->max_h > max_h)
                                                max_h = children[i]->max_h;
                                }

                                hasExactBoundingBox = true;

                                aabb.setMinimumY(min_h);
                                aabb.setMaximumY(max_h);
                                diagonal = length (aabb.getMaximum()-aabb.getMinimum());

                                // propagate to parent
                                if (parent) parent->refineBoundingBox();
                        }
                }
        public:
                Node (
                        const BoundingBoxF &box,
                        const int maxRecursion_,
                        Node *parent_,
                        NodeStaticParameters const &staticParameters
                )
                : parent(parent_)
                , aabb(box)
                , vertices(0)
                , lastUsedInScanline(0)
                , hasExactBoundingBox(false)
                , maxRecursion(maxRecursion_)
                , vertexCount(0)
                , staticParameters(staticParameters)
                {
                        for (int i=0; i<4; ++i) {
                                children[i] = 0;
                        }
                }

                ~Node () {
                        if (parent) {
                                for (int i=0; i<4; ++i) {
                                        if (parent->children[i] == this)
                                                parent->children[i] = 0;
                                }
                        }
                        /*if (nodeIndex.cached (this)) {
                                nodeIndex.remove_index (this);
                        }*/
                        for (int i=0; i<4; ++i) {
                                if (children[i]) {
                                        children[i]->parent = 0;
                                        delete children[i];
                                }
                        }
                        delete [] vertices;
                }


                /*void killChild (const Node *child) {
                        for (int i=0; i<4; ++i) {
                                if (child == children[i]) {
                                        delete children[i];
                                        children[i] = 0;
                                        break;
                                }
                        }
                }*/

                void prepare (Scene const &scene) {
                        if (!scene.getCamera()->hasCommonCenter()) {
                                std::cerr << "LazyQuadtreeImpl: Camera has no common center. Results undefined" << std::endl;
                        } else {
                                prepare (vector_cast<PointF>(scene.getCamera()->getCommonCenter())
                                        , scene.currentScanline()
                                );
                        }

                        lastUsedInScanline = scene.currentScanline();
                }

                void prune (unsigned int currentScanline, const unsigned int depth=0) {
                        for (int i=0; i<4; ++i) {
                                if (children[i]) {
                                        const unsigned int diff =
                                                currentScanline-children[i]->lastUsedInScanline;
                                        if (diff >= 10) {
                                                delete children[i];
                                                children[i] = 0;
                                        } else if (depth < 7) {
                                                children[i]->prune(currentScanline, depth+1);
                                        }
                                }
                        }
                }

                bool isALeaf (real_t diagonal, real_t cx, real_t cz, PointF const &cameraPosition) {
                        // Using maxRecursion and some reasoning about the camera position
                        // in relation to (cx,cz), we should be able to take the case of
                        // maxRecursion for neighbour nodes into account.
                        const real_t d = (length(PointF(cx,0,cz)-cameraPosition));
                        if (((diagonal/(1+d))<staticParameters.lodFactor)) {
                                return true;
                        } else {
                                return false;
                        }

                }
                void prepare (PointF const & cameraPosition, unsigned int currentScanline) {

                        this->cameraPosition = cameraPosition;
                        this->lastUsedInScanline = currentScanline;

                        // If we once re-use an existing quadtree, we should
                        // also save the original bounding box, as LOD's must be
                        // recalculated
                        initBoundingBox();

                        const real_t min_x = aabb.getMinimumX();
                        const real_t max_x = aabb.getMaximumX();
                        const real_t c_x   = (min_x + max_x) / 2;
                        const real_t min_z = aabb.getMinimumZ();
                        const real_t max_z = aabb.getMaximumZ();
                        const real_t c_z   = (min_z + max_z) / 2;
                        center_x = c_x;
                        center_z = c_z;

                        isLeaf = (maxRecursion==0) || isALeaf (this->diagonal, center_x, center_z, cameraPosition);

                        const int a = !isALeaf (this->diagonal, center_x, center_z+aabb.getDepth(), cameraPosition);
                        const int b = !isALeaf (this->diagonal, center_x+aabb.getWidth(), center_z, cameraPosition);
                        const int c = !isALeaf (this->diagonal, center_x, center_z-aabb.getDepth(), cameraPosition);
                        const int d = !isALeaf (this->diagonal, center_x-aabb.getWidth(), center_z, cameraPosition);


                        // Upon re-using LazyQuadtree, deletion shall only happen when the state of isLeaf
                        // changes.
                        delete [] vertices;
                        vertexCount = 0;
                        if (isLeaf) {
                                vertexCount = 6 + a+ b+ c+ d;
                                int i=0;
                                vertices = new Vector[vertexCount];
                                vertices[i++] = Vector (c_x,   fun(c_x,     c_z),  c_z); // 0

                                vertices[i++] = Vector (min_x, fun(min_x, max_z), max_z);  // 1
                                if (a) vertices[i++] = Vector (c_x, fun(c_x, max_z), max_z);
                                vertices[i++] = Vector (max_x, fun(max_x, max_z), max_z); // 2
                                if (b) vertices[i++] = Vector (max_x, fun(max_x, c_z), c_z);
                                vertices[i++] = Vector (max_x, fun(max_x, min_z), min_z); // 3
                                if (c) vertices[i++] = Vector (c_x, fun(c_x, min_z), min_z);
                                vertices[i++] = Vector (min_x, fun(min_x, min_z), min_z); // 4
                                if (d) vertices[i++] = Vector (min_x, fun(min_x, c_z), c_z);
                                vertices[i++] = vertices[1]; // 5
                        }


                        if (isLeaf) {
                                refineBoundingBox();
                        }

                        for (int i=0; i<4; ++i) {
                                if (children[i]) children[i]->prepare (cameraPosition, currentScanline);
                        }

                }

                optional<pair<real_t,Normal> > intersect (
                        RayDifferential const &ray,
                        real_t minT, real_t maxT,
                        unsigned int currentScanline
                ) const {

                        lastUsedInScanline = currentScanline;

                        struct Triangle {
                                Vertex &a, &b, &c;
                        };
                        // We can assume minT and maxT to be a correct interval on the xz plane.
                        // But we got to check for vertical intersection now.
                        const real_t p_h = scalar_cast<real_t>(ray.position.y);
                        const real_t min_h = p_h + minT * ray.direction.y;
                        const real_t max_h = p_h + maxT * ray.direction.y;
                        if (((min_h < this->min_h) & (max_h < this->min_h))
                           |((min_h > this->max_h) & (max_h > this->max_h)))
                                return false;

                        if (isLeaf) {
                                /*#pragma omp master
                                if (parent) nodeIndex.load (this);*/
                                return intersectLeaf (ray);
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
                        optional<pair<real_t,Normal> > dg;

                        {
                                for (int i=0; i<3; ++i) {
                                        dg = traverse (ray, t[i].child, t[i].t0, t[i].t1, currentScanline);
                                        if (dg) {
                                                break;
                                        }
                                }
                        }
                        /*#pragma omp master
                        if (parent) nodeIndex.load (this);*/

                        return dg;
                }


                friend void clearNode (const Node *);
        };



        void clearNode (const lazyquadtree::Node* p) {
                delete p;
        }
}






class LazyQuadtreeImpl {
public:
        LazyQuadtreeImpl (
                shared_ptr<HeightFunction const> fun,
                real_t size,
                unsigned int maxRecursion,
                real_t lodFactor,
                Color color
        )
        : fun(fun)
        , primaryBB(initBB (
                size,
                static_cast<unsigned int>(
                        max(real_t(1000),min(real_t(250000), (size*size*size)/1000.f))
                )))
        , primaryFixpBB(
                vector_cast<Point>(primaryBB.getMinimum()),
                vector_cast<Point>(primaryBB.getMaximum()))
        , staticParameters(*fun.get(), lodFactor)
        , nodeIndex(0)//(1024*13) * 4)
        , primaryNode(new lazyquadtree::Node(
                primaryBB,
                maxRecursion,
                0,
                staticParameters))
                                // for benchmarking, depth was 4, AAx4, no diffuse queries, 512x512
                                // //"(+ -150 (* 500 (^ (- 1 (abs ([LayeredNoise2d filter{cosine} seed{13} frequency{0.001} layercount{8} persistence{0.45} levelEvaluationFunction{(abs h)}] x y))) 2 )))"
                                // horizonPlane y 25
                                // shared_ptr<Camera> camera (new Pinhole(renderBuffer, vector_cast<Point>(Vector(390,70,-230))));
        , color(color)
        {
        }


        ~LazyQuadtreeImpl() {
                delete primaryNode;
        }



        void prepare (const Scene &scene) {
                primaryNode->prepare (scene);
        }



        bool doesIntersect (RayDifferential const &ray) const {
                //return does_intersect<false> (ray, primaryBB);
                return intersect(ray);
        }



        optional<DifferentialGeometry> intersect (RayDifferential const &ray) const {
                const optional<tuple<real_t,real_t> > i =
                                        redshift::intersect<false>(ray,primaryFixpBB);
                if (!i) return false;

                const real_t minT = get<0>(*i);
                const real_t maxT = get<1>(*i);

                const optional<pair<real_t,Normal> > dg = primaryNode->intersect (ray, minT, maxT, currentScanline);
                if (!dg)
                        return false;

                // Use height function as normal map.

                const Point poi = ray(dg->first);//dg->getCenter();
                const Vector voi = vector_cast<Vector>(poi);

                // For some reason I fail to see in this dull moment, I had to flip
                // u x v with v x u.
                const real_t s = 0.001f;
                const real_t h =  (*fun)(voi.x,voi.z);
                const Vector u = normalize (Vector(s, (*fun)(voi.x+s,voi.z) - h, 0));
                const Vector v = normalize (Vector(0, (*fun)(voi.x,voi.z+s) - h, s));
                /*const tuple<Vector,Vector,Vector> lcs = coordinateSystem(dg->second);
                const Vector u = get<2>(lcs);
                const Vector v = get<0>(lcs);*/

                return DifferentialGeometry (
                        dg->first,
                        poi,
                        dg->second, // geometric normal
                        v, u,// partial derivative of position
                        Vector(), Vector()
                );
        }



        shared_ptr<Bsdf> getBsdf(
                const DifferentialGeometry & dgGeom
        ) const {
                shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));
                bsdf->add (shared_ptr<Bxdf>(new bsdf::Lambertian (color)));
                return bsdf;
        }



        void prune () {
                primaryNode->prune (currentScanline);
                //std::cout << nodeIndex.stride() << std::endl;
                //nodeIndex.collect();
        }



        void setCurrentScanline (unsigned int s) {
                currentScanline = s;
        }

private:

        shared_ptr<HeightFunction const> fun;
        BoundingBoxF primaryBB;
        BoundingBox primaryFixpBB;

        lazyquadtree::NodeStaticParameters staticParameters;
        lazyquadtree::NodeIndex nodeIndex;
        lazyquadtree::Node *primaryNode;
        Color color;
        unsigned int currentScanline;

        BoundingBoxF initBB(const real_t size, const unsigned int numSamples) const {
                real_t minh = constants::real_max , maxh = -constants::real_max;
                Random rand (123,45678,91011,121314);
                const real_t size05 = size/2;

                for (unsigned int x=0; x<numSamples; ++x) {
                        const real_t uv[] = {
                                rand()*size-size05,
                                rand()*size-size05
                        };
                        {
                                const real_t h = (*fun)(uv[0],uv[1]);
                                if (h < minh) minh = h;
                                if (h > maxh) maxh = h;
                        }
                        // reusing random numbers:
                        {
                                const real_t h = (*fun)(uv[1],uv[0]);
                                if (h < minh) minh = h;
                                if (h > maxh) maxh = h;
                        }
                }
                return BoundingBoxF (
                        PointF(-size05,minh,-size05),
                        PointF(size05,maxh,size05)
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
        unsigned int maxRecursion,
        real_t lodFactor,
        Color color
)
: impl (new LazyQuadtreeImpl (fun, size, maxRecursion, lodFactor, color))
, heightFun(fun)
{
        std::cout << "sizeof(Node)==" << sizeof(lazyquadtree::Node) << " (was 216)\n"
                  << "sizeof(Node)/8==" << sizeof(lazyquadtree::Node)/8. << " (was 27)" << std::endl;
        std::cout << "sizeof(Node*)==" << sizeof(lazyquadtree::Node*) << std::endl;
        std::cout << "sizeof(Mutex)==" << sizeof(Mutex) << std::endl;
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
        optional<DifferentialGeometry> const dg = impl->intersect (ray);
        if (!dg)
                return false;
        return Intersection (
                shared_from_this(),
                *dg
        );
}



shared_ptr<Bsdf> LazyQuadtree::getBsdf(
        const DifferentialGeometry & dgGeom
) const {
        return impl->getBsdf (dgGeom);
}



void LazyQuadtree::prepare (const Scene &scene) {
        impl->prepare (scene);
}



void LazyQuadtree::prune () {
        impl->prune ();
}



void LazyQuadtree::setCurrentScanline (unsigned int s) {
        impl->setCurrentScanline (s);
}

} }
