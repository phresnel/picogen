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


// TODO outsource this to auxiliary
template <typename T, size_t CAPACITY, bool NEED_DESTRUCTION = true>
class Chunk {
        union Proxy { unsigned char raw[sizeof(T)]; };
        Proxy data[CAPACITY];

        size_t size;
public:
        Chunk() : size(0) {}
        ~Chunk() {
                if (NEED_DESTRUCTION) for (size_t i=0; i<size; ++i)
                        ((T*)(void*)data[i].raw)->~T();
        }
        bool full() const { return size >= CAPACITY; }
        void *alloc() {
                const size_t s = size;
                #pragma omp atomic
                size++;
                return data[s].raw;
        }
};


template <typename T, size_t CHUNK_CAPACITY, bool NEED_DESTRUCTION = true>
class MemoryPool {
        typedef ::Chunk<T, CHUNK_CAPACITY, NEED_DESTRUCTION> Chunk;
        std::list<Chunk> chunks;
        redshift::Mutex mutex;
public:
        MemoryPool() {
                chunks.push_back(Chunk());
        }

        void* alloc() {
                void *ret = 0;
                if (chunks.back().full()) {
                        redshift::ScopedLock lock(mutex);
                        #pragma omp flush
                        if (chunks.back().full()) {
                                chunks.push_back(Chunk());
                        }
                }
                ret = chunks.back().alloc();
                return ret;
        }
};


//#############################################################################
// LazyQuadtreeImpl
//#############################################################################
namespace redshift { namespace primitive {


namespace lazyquadtree {

        class Node;
        typedef ::MemoryPool<Node, 1024, true> NodePool;

        /*void clearNode (const Node *);
        typedef
                kallisto::cache::mru_cache_index<const lazyquadtree::Node*,
                                                 kallisto::cache::collect,
                                                 clearNode>
                NodeIndex;*/

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
                NodeStaticParameters (
                        const HeightFunction & fun, real_t lodFactor, NodePool *pools, Mutex *mutexes,
                        real_t rootWidth, unsigned int maxRecursion
                )
                : fun(fun), lodFactor(lodFactor), pools(pools), mutexes(mutexes) {
                        real_t width = rootWidth;
                        const size_t s = maxRecursion+1;
                        widths = new real_t[s];
                        for (unsigned int i=0; i<s; ++i, width/=2) {
                                widths[(s-1)-i] = width;
                        }
                        /*for (unsigned int i=0; i<s; ++i) {
                                std::cout << i << ":" << widths[(s-1)-i] << std::endl;
                        }*/
                }

                ~NodeStaticParameters() {
                        delete [] widths;
                }

                const HeightFunction &fun;
                const real_t lodFactor;
                NodePool *pools;
                Mutex *mutexes;
                PointF cameraPosition;

                real_t *widths;
        };



        class Node {
                mutable Node *parent;
                mutable Node *children[4];

                float bbMinX_;
                mutable float bbMinY, bbMaxY;
                float bbMinZ_;

                Vector *vertices;

                const NodeStaticParameters &staticParameters;

                float bbMinX() const { return bbMinX_; }
                float bbMaxX() const { return bbMinX() + staticParameters.widths[maxRecursion]; }
                float bbMinZ() const { return bbMinZ_; }
                float bbMaxZ() const { return bbMinZ() + staticParameters.widths[maxRecursion]; }
                float centerX() const { return 0.5f*bbMinX() + 0.5f*bbMaxX(); }
                float centerZ() const { return 0.5f*bbMinZ() + 0.5f*bbMaxZ(); }
                float bbWidth() const { return staticParameters.widths[maxRecursion]; }
                float bbDepth() const { return staticParameters.widths[maxRecursion]; }

                struct {
                        bool isLeaf:1;
                        mutable bool hasExactBoundingBox:1 ; // once this is well defined, get rid of
                                                             // initializedChildCount
                        unsigned char maxRecursion : 6; // 0 = max recursion reached
                        unsigned char vertexCount : 4;
                };


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

                        const real_t
                                bbMinX = this->bbMinX(),
                                bbMinZ = this->bbMinZ(),
                                bbMaxX = this->bbMaxX(),
                                bbMaxZ = this->bbMaxZ();
                        const real_t center_x = centerX();
                        const real_t center_z = centerZ();

                        Node *tmp;

                        BoundingBoxF box;
                        switch (index) {
                        case 0: box = BoundingBoxF(PointF(bbMinX,   bbMinY, bbMinZ),
                                                   PointF(center_x, bbMaxY, center_z));
                                break;
                        case 1: box = BoundingBoxF(PointF(center_x, bbMinY, bbMinZ),
                                                   PointF(bbMaxX,   bbMaxY, center_z));
                                break;
                        case 2: box = BoundingBoxF(PointF(bbMinX,   bbMinY, center_z),
                                                   PointF(center_x, bbMaxY, bbMaxZ));
                                break;
                        case 3: box = BoundingBoxF(PointF(center_x, bbMinY, center_z),
                                                   PointF(bbMaxX,   bbMaxY, bbMaxZ));
                                break;
                        };

                        ScopedLock sl (staticParameters.mutexes[index]);
                        #pragma omp flush

                        if (children[index])
                                return;

                        tmp = new //(staticParameters.pools[index].alloc())
                                  Node (box,
                                        maxRecursion-1,
                                        const_cast<Node*>(this),
                                        staticParameters);
                        tmp->prepare();

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

                void refineBoundingBox () {
                        if (isLeaf) {
                                bbMinY = constants::real_max;
                                bbMaxY = -constants::real_max;

                                for (int i=0; i<vertexCount; ++i) {
                                        const real_t h = vertices[i].y;
                                        if (h < bbMinY) bbMinY = h;
                                        if (h > bbMaxY) bbMaxY = h;
                                }

                                hasExactBoundingBox = true;

                                // propagate to parent
                                if (parent) parent->refineBoundingBox();

                        } else {

                                for (int i = 0; i<4; ++i) {
                                        if (!children[i])
                                                return;
                                        if (!children[i]->hasExactBoundingBox)
                                                return;
                                }


                                bbMinY = constants::real_max;
                                bbMaxY = -constants::real_max;

                                for (int i=0; i<4; ++i) {
                                        if (children[i]->bbMinY < bbMinY)
                                                bbMinY = children[i]->bbMinY;
                                        if (children[i]->bbMaxY > bbMaxY)
                                                bbMaxY = children[i]->bbMaxY;
                                }

                                hasExactBoundingBox = true;

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
                , bbMinX_(box.getMinimumX())//, bbMaxX_(box.getMaximumX())
                , bbMinY(box.getMinimumY()), bbMaxY(box.getMaximumY())
                , bbMinZ_(box.getMinimumZ())//, bbMaxZ_(box.getMaximumZ())
                , vertices(0)
                , hasExactBoundingBox(false)
                , maxRecursion(maxRecursion_)
                , vertexCount(0)
                , staticParameters(staticParameters)
                {
                        for (int i=0; i<4; ++i) {
                                children[i] = 0;
                        }
                        //std::cout << "[" << (int)maxRecursion << "] " << bbMinX() << ":" << bbMaxX() << std::endl;
                }

                ~Node () {
                        if (parent) {
                                for (int i=0; i<4; ++i) {
                                        if (parent->children[i] == this)
                                                parent->children[i] = 0;
                                }
                        }
                        for (int i=0; i<4; ++i) {
                                if (children[i]) {
                                        //children[i]->parent = 0;
                                        delete children[i];
                                }
                        }
                        delete [] vertices;
                }

                void prepare (Scene const &scene) {
                        if (!scene.getCamera()->hasCommonCenter()) {
                                std::cerr << "LazyQuadtreeImpl: Camera has no common center. Results undefined" << std::endl;
                        } else {
                                prepare ();
                        }
                }

                void prune (unsigned int currentScanline, const unsigned int depth=0) {
                }

                bool isALeaf (real_t diagonal, real_t cx, real_t cz) {
                        // Using maxRecursion and some reasoning about the camera position
                        // in relation to (cx,cz), we should be able to take the case of
                        // maxRecursion for neighbour nodes into account.
                        //const real_t d = (length(PointF(cx,0,cz)-staticParameters.cameraPosition));

                        const real_t
                                x  = cx - staticParameters.cameraPosition.x,
                                z  = cz - staticParameters.cameraPosition.z,
                                ds = x*x
                                   + staticParameters.cameraPosition.y*staticParameters.cameraPosition.y
                                   + z*z,
                                d = std::sqrt(ds)
                        ;
                        if ((diagonal/(1+d)) < staticParameters.lodFactor) {
                                return true;
                        } else {
                                return false;
                        }

                }
                void prepare () {

                        const PointF &cameraPosition = staticParameters.cameraPosition;
                        // If we once re-use an existing quadtree, we should
                        // also save the original bounding box, as LOD's must be
                        // recalculated

                        const real_t
                                c_x   = centerX(),
                                c_z   = centerZ(),
                                bbDepth = this->bbDepth(),
                                bbWidth = this->bbWidth(),
                                diagonal = std::sqrt(bbWidth*bbWidth + bbDepth*bbDepth)
                        ;

                        isLeaf = (maxRecursion==0) || isALeaf (diagonal, c_x, c_z);

                        // remove diagonal entirely from class-def?
                        const int a = !isALeaf (diagonal, c_x,         c_z+bbDepth);
                        const int b = !isALeaf (diagonal, c_x+bbWidth, c_z        );
                        const int c = !isALeaf (diagonal, c_x,         c_z-bbDepth);
                        const int d = !isALeaf (diagonal, c_x-bbWidth, c_z        );


                        // Upon re-using LazyQuadtree, deletion shall only happen when the state of isLeaf
                        // changes.
                        delete [] vertices;
                        vertexCount = 0;
                        if (isLeaf) {
                                const real_t
                                        bbMinX = this->bbMinX(),
                                        bbMinZ = this->bbMinZ(),
                                        bbMaxX = this->bbMaxX(),
                                        bbMaxZ = this->bbMaxZ();

                                vertexCount = 6 + a+ b+ c+ d;
                                int i=0;
                                vertices = new Vector[vertexCount];
                                vertices[i++] = Vector (c_x,   fun(c_x,     c_z),  c_z); // 0

                                vertices[i++] = Vector (bbMinX, fun(bbMinX, bbMaxZ), bbMaxZ);  // 1
                                if (a) vertices[i++] = Vector (c_x, fun(c_x, bbMaxZ), bbMaxZ);
                                vertices[i++] = Vector (bbMaxX, fun(bbMaxX, bbMaxZ), bbMaxZ); // 2
                                if (b) vertices[i++] = Vector (bbMaxX, fun(bbMaxX, c_z), c_z);
                                vertices[i++] = Vector (bbMaxX, fun(bbMaxX, bbMinZ), bbMinZ); // 3
                                if (c) vertices[i++] = Vector (c_x, fun(c_x, bbMinZ), bbMinZ);
                                vertices[i++] = Vector (bbMinX, fun(bbMinX, bbMinZ), bbMinZ); // 4
                                if (d) vertices[i++] = Vector (bbMinX, fun(bbMinX, c_z), c_z);
                                vertices[i++] = vertices[1]; // 5
                        }


                        if (isLeaf) {
                                refineBoundingBox();
                        }

                        for (int i=0; i<4; ++i) {
                                if (children[i]) children[i]->prepare ();
                        }

                }

                optional<pair<real_t,Normal> > intersect (
                        RayDifferential const &ray,
                        real_t minT, real_t maxT,
                        unsigned int currentScanline
                ) const {

                        // We can assume minT and maxT to be a correct interval on the xz plane.
                        // But we got to check for vertical intersection now.
                        const real_t p_h = scalar_cast<real_t>(ray.position.y);
                        const real_t min_h = p_h + minT * ray.direction.y;
                        const real_t max_h = p_h + maxT * ray.direction.y;


                        // BOTTLENECK when this->aabb.get...?
                        if (((min_h < bbMinY) & (max_h < bbMinY))
                           |((min_h > bbMaxY) & (max_h > bbMaxY)))
                                return false;

                        if (isLeaf) {
                                /*#pragma omp master
                                if (parent) nodeIndex.load (this);*/
                                return intersectLeaf (ray);
                        }

                        // Find out which ones to traverse.
                        const bool d_right = ray.direction.x >= 0;
                        const bool d_up    = ray.direction.z >= 0;
                        const real_t d_x = (centerX() - scalar_cast<real_t>(ray.position.x)) / ray.direction.x;
                        const real_t d_z = (centerZ() - scalar_cast<real_t>(ray.position.z)) / ray.direction.z;
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
        , staticParameters(*fun.get(), lodFactor, pools, mutexes, size, maxRecursion)
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
                staticParameters.cameraPosition =
                        vector_cast<PointF>(scene.getCamera()->getCommonCenter());
                primaryNode->prepare ();
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
        }



        void setCurrentScanline (unsigned int s) {
                currentScanline = s;
        }

private:

        shared_ptr<HeightFunction const> fun;
        BoundingBoxF primaryBB;
        BoundingBox primaryFixpBB;

        lazyquadtree::NodeStaticParameters staticParameters;
        lazyquadtree::Node *primaryNode;
        Color color;
        unsigned int currentScanline;

        lazyquadtree::NodePool pools[4];
        Mutex mutexes[4];

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
        /*
        std::cout << "sizeof(Node)==" << sizeof(lazyquadtree::Node) << " (was 216)\n"
                  << "sizeof(Node)/8==" << sizeof(lazyquadtree::Node)/8. << " (was 27)" << std::endl;
        std::cout << "sizeof(Node*)==" << sizeof(lazyquadtree::Node*) << std::endl;
        std::cout << "sizeof(Mutex)==" << sizeof(Mutex) << std::endl;
        */
}



LazyQuadtree::~LazyQuadtree () {
}



bool LazyQuadtree::doesIntersect (Ray const &ray) const {
        return impl->doesIntersect (ray);
}



optional<Intersection>
 LazyQuadtree::intersect(Ray const &ray) const {
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

} }
