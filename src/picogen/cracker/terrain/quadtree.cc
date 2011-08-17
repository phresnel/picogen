#include "quadtree.h"
#include "color.h"
#include "math3d.h"
#include "materials/lambertmaterial.h"

#include <array>
#include <stack>
#include <functional>
#include <limits>

#include <QDebug>

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
                                        h(ux,uz) = Vector(x,y,z);                                }
                        }

                        /*exactBB = BoundingBox (Point(left, y_min, front),
                                               Point(right, y_max, back));*/

                        material_.reset (new LambertMaterial(Color::FromRgb(
                                                                     0.3,0.8,0.4)));
                }

                ~Patch() {
                        delete [] h_;
                }

                Intersection::Optional operator() (Ray const &ray) const {
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
                                return intersect<-1,1> (ray, res_x_-1, -1,
                                                             0,        res_z_);
                        }
                        return intersect<-1,-1> (ray, res_x_-1, -1,
                                                      res_z_-1, -1);
                }


        private:
                real left_, right_, front_, back_;
                unsigned int res_x_, res_z_, stride_;
                //real *h_;
                Vector *h_;

                std::shared_ptr<Material> material_;


                template <int step_x, int step_z>
                Intersection::Optional intersect (
                        Ray const &ray,
                        int left, int right,
                        int front, int back
                ) const
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


                /*
                Vector const & vertex(int ux, int uz, real u, real v) const {
                        //const real u = ux/static_cast<real>(res_x_);
                        //const real v = uz/static_cast<real>(res_z_);

                        const real x = (1-u)*left_ + u*right_;
                        const real z = (1-v)*front_ + v*back_;
                        const real y = h(ux,uz);

                        return Vector(x,y,z);
                }
                real h (unsigned int x, unsigned z) const {
                        return h_[x + z*stride_];
                }
                real& h (unsigned int x, unsigned z) {
                        return h_[x + z*stride_];
                }
                */
                Vector const * ph (unsigned int x, unsigned z) const {
                        return h_ + x + z*stride_;
                }
                Vector& h (unsigned int x, unsigned z) {
                        return h_[x + z*stride_];
                }
        };
}

namespace detail {

        inline std::vector<std::vector<real> > child_boxen (
                real left, real right, real front, real back)
        {
                using std::array;
                using std::vector;

                //const Point c = aabb.center();
                const real cx = 0.5*right + 0.5*left,
                           cz = 0.5*front + 0.5*back;
                const real oldW = right-left,
                           //oldH = aabb.height(),
                           oldD = back-front,
                           newW = oldW / 2,
                           //newH = oldH,
                           newD = oldD / 2
                          ;

                std::vector<std::vector<real> > ret =
                        {{cx - newW, cx       ,  cz     , cz + newD},
                         {cx       , cx + newW,  cz     , cz + newD},
                         {cx       , cx + newW,  cz-newD, cz       },
                         {cx - newW, cx       ,  cz-newD, cz       }};

                /*
                ret[0] = BoundingBox (c + Vector(-newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[1] = BoundingBox (c + Vector(+newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[2] = BoundingBox (c + Vector(+newW/2, 0, -newD/2),
                                      newW, newH, newD);
                ret[3] = BoundingBox (c + Vector(-newW/2, 0, -newD/2),
                                      newW, newH, newD);*/
                return ret;
        }

        template <bool d_right, bool d_up>
        struct ChildIndices ;

        template <> struct ChildIndices<true, true> {
                enum { a = 3, upper_b = 0, lower_b = 2, c = 1 };
        };
        template <> struct ChildIndices<false, true> {
                enum { a = 2, upper_b = 1, lower_b = 3, c = 0 };
        };
        template <> struct ChildIndices<true, false> {
                enum { a = 0, upper_b = 3, lower_b = 1, c = 2 };
        };
        template <> struct ChildIndices<false, false> {
                enum { a = 1, upper_b = 2, lower_b = 0, c = 3 };
        };

        class Node {
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                // Creates a root node.
                Node (unsigned int depth,
                      std::function<real (real,real)> const & height,
                      BoundingBox rootBB)
                {
                        qDebug() << "creating Quadtree";
                        const Point center (0, -40, 100);
                        const real left = center.x()-100,
                                   right = center.x()+100,
                                   front = center.z()-100,
                                   back = center.z()+100;
                        left_ = left;
                        right_ = right;
                        front_ = front;
                        back_ = back;
                        create(depth, center,
                               left, right,
                               front, back,
                               height);
                        rootBB = BoundingBox (Point(left, min_h_, front),
                                              Point(right, max_h_, back));
                        qDebug() << "Quadtree created.";
                }

                ~Node () {
                        if (leaf_) delete patch_;
                        else delete [] children_;
                }


                Intersection::Optional intersect (
                        Ray const &ray,
                        real minT, real maxT
                ) const {

                        if (true) {
                                // We can assume minT and maxT to be a correct interval on the xz plane.
                                // But we got to check for vertical intersection now.
                                const real p_h = ray.origin().y();
                                const real min_h = p_h + minT * ray.direction().y();
                                const real max_h = p_h + maxT * ray.direction().y();


                                // BOTTLENECK when this->aabb.get...?
                                if (((min_h < min_h_) & (max_h < min_h_))
                                   |((min_h > max_h_) & (max_h > max_h_)))
                                        return Intersection::Optional();
                        }

                        if (leaf_) {
                                return (*patch_)(ray);
                        }


                        // Find out which ones to traverse.
                        const real c_x = 0.5*left_+0.5*right_;
                        const real c_z = 0.5*front_+0.5*back_;
                        const bool d_right = ray.direction().x() >= 0;
                        const bool d_up    = ray.direction().z() >= 0;
                        const real d_x = (c_x - ray.origin().x()) / ray.direction().x();
                        const real d_z = (c_z - ray.origin().z()) / ray.direction().z();
                        const bool upper_three = d_x > d_z;

                        // +----+----+
                        // | 0  | 1  |
                        // -----+-----
                        // | 3  | 2  |
                        // +----+----+

                        struct C {
                                int child;
                                real t0, t1;
                        } t[3];

                        if (d_right & d_up) {
                                if (upper_three) {
                                        // 0, 2, 3
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 0; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 1; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 0, 1, 3
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 2; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 1; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else if (!d_right & d_up) {
                                if (upper_three) {
                                        // 1, 3, 2
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 1; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 0; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 1, 0, 2
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 3; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 0; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        // TODO: implement other cases
                        else if (d_right & !d_up) {
                                if (upper_three) {
                                        // 2, 0, 1
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 3; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 2; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 2, 3, 1
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 1; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 2; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else if (!d_right & !d_up) {
                                if (upper_three) {
                                        // 3, 1, 0
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 2; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 3; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 3, 2, 0
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 0; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 3; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else {
                                return Intersection::Optional();
                        }

                        for (int i=0; i<3; ++i) {
                                const C& c = t[i];
                                if (c.t0 > c.t1) continue;
                                const Intersection::Optional I =
                                                children_[c.child].intersect(
                                                        ray, c.t0, c.t1);
                                if (I) return I.intersection();
                        }
                        return Intersection::Optional();
                }


                template <bool d_right, bool d_up>
                static Intersection::Optional intersect_iter_directed (
                        Node *node,
                        Ray const &ray,
                        real minT_, real maxT_
                ) {
                        struct Todo {
                                Todo() {}
                                Todo (real minT, real maxT, Node *node)
                                        : minT(minT), maxT(maxT), node(node) {}
                                real minT, maxT;
                                Node *node;
                        };

                        const Direction dir  = ray.direction();
                        //const Vector idir    = real(1) / dir;
                        //const Point  &origin = ray.origin();

                        const real d_y = dir.y();
                        const real o_y = ray.origin().y();
                        const real o_x = ray.origin().x();
                        const real o_z = ray.origin().z();

                        const real id_x = 1 / dir.x();
                        const real id_z = 1 / dir.z();


                        //std::stack<Todo> todo; § replace me with somethint stack-framable
                        Todo stack[128];
                        Todo *top = stack;
                        *top++ = Todo({minT_, maxT_, node});
                        while (top != stack) {
                                const Todo curr = *--top;
                                const Node &node = *curr.node;

                                //if (minT > maxT) continue;

                                // We can assume minT and maxT to be a correct interval on the xz plane.
                                // But we got to check for vertical intersection now.
                                const real min_h = o_y + curr.minT * d_y;
                                const real max_h = o_y + curr.maxT * d_y;


                                // BOTTLENECK when this->aabb.get...?
                                if (((min_h < node.min_h_) & (max_h < node.min_h_))
                                   |((min_h > node.max_h_) & (max_h > node.max_h_))
                                   |(curr.minT>curr.maxT)
                                   )
                                        continue;

                                if (node.leaf_) {
                                        const auto i = (*node.patch_)(ray);
                                        if (i) return i;
                                } else {
                                        // Find out which ones to traverse.
                                        const real c_x = 0.5*node.left_ + 0.5*node.right_;
                                        const real c_z = 0.5*node.front_+ 0.5*node.back_;
                                        const real d_x = (c_x - o_x) * id_x;
                                        const real d_z = (c_z - o_z) * id_z;

                                        const bool upper_three = d_x > d_z;

                                        // +----+----+
                                        // | 0  | 1  |
                                        // -----+-----
                                        // | 3  | 2  |
                                        // +----+----+

                                        typedef ChildIndices<d_right, d_up> indices;

                                        const real a = upper_three ? d_x : d_z;
                                        const real b = upper_three ? d_z : d_x;
                                        const int index_b = upper_three ? indices::upper_b : indices::lower_b;

                                        *top++ = Todo ({a, curr.maxT, node.children_+indices::c});
                                        *top++ = Todo ({b, a,    node.children_+index_b});
                                        *top++ = Todo ({curr.minT, b, node.children_+indices::a});
                                }
                        }
                        return Intersection::Optional();
                }

                static Intersection::Optional intersect_iter (
                        Node *node,
                        Ray const &ray,
                        real minT, real maxT
                ) {
                        if (ray.direction().x()>=0) {
                                if (ray.direction().z() >= 0) {
                                        return intersect_iter_directed<1,1>(
                                                node, ray, minT, maxT);
                                } else {
                                        return intersect_iter_directed<1,0>(
                                                node, ray, minT, maxT);
                                }
                        } else {
                                if (ray.direction().z() >= 0) {
                                        return intersect_iter_directed<0,1>(
                                                node, ray, minT, maxT);
                                } else {
                                        return intersect_iter_directed<0,0>(
                                                node, ray, minT, maxT);
                                }
                        }
                }

                Node() {}

                void create(unsigned int depth,
                            Point const &center,
                            real left, real right, real front, real back,
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);
                        //const real distance = length (aabb.center() - center);
                        //qDebug() << distance << " <- " <<
                        //            (13/(1+0.02*distance));
                        left_ = left;
                        right_ = right;
                        front_ = front;
                        back_ = back;

                        if (leaf)
                                makeLeaf(center,
                                         left, right,
                                         front, back,
                                         height);
                        else makeInner(center, depth,
                                       left, right,
                                       front, back, height);
                }
                void makeLeaf (Point const &center,
                               //BoundingBox const &aabb,
                               real left, real right,
                               real front, real back,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = true;
                        //aabb_ = aabb;
                        patch_ = new Patch (left, right,
                                            front, back,
                                            4,4,
                                            height,
                                            min_h_, max_h_);
                        //patch_->exactBoundingBox ();
                        //refineBoundingBox (height);
                }
                void makeInner(Point const &center,
                               unsigned int depth,
                               real left, real right,
                               real front, real back,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = false;
                        const auto childBoxes = child_boxen(left, right,
                                                            front, back);
                        children_ = new Node[4];
                        for (size_t i=0; i<4; ++i) {
                                children_[i].create (depth-1, center,
                                                     childBoxes[i][0],
                                                     childBoxes[i][1],
                                                     childBoxes[i][2],
                                                     childBoxes[i][3],
                                                     height);
                        }
                        refineBoundingBox();
                }

                // Refinement for inner nodes.
                // Pre-condition: * child nodes are refined
                //                * has child nodes
                void refineBoundingBox () {
                        real min_h = children_[0].min_h_;
                        min_h = min(min_h, children_[1].min_h_);
                        min_h = min(min_h, children_[2].min_h_);
                        min_h = min(min_h, children_[3].min_h_);
                        real max_h = children_[0].max_h_;
                        max_h = max(max_h, children_[1].max_h_);
                        max_h = max(max_h, children_[2].max_h_);
                        max_h = max(max_h, children_[3].max_h_);

                        this->min_h_ = min_h;
                        this->max_h_ = max_h;
                }

        private:
                union {
                        Node *children_;
                        Patch *patch_;
                };
                bool leaf_;
                real min_h_, max_h_;
                real left_, right_, front_, back_;
        };
}

Quadtree::Quadtree ()
{
        const auto fun = [](real x,real y) {
                return -30+15 * cos(y*0.1) * cos(x*0.1);
        };

        qDebug() << "sizeof(Node) =" << sizeof(detail::Node);

        root_.reset (new detail::Node (4, fun, aabb_));

}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        Interval::Optional oi = intersect (ray, aabb_);
        if (!oi) return Intersection::Optional();

        if (1) {
                return detail::Node::intersect_iter(&*root_, ray,
                                                    oi.interval().min(),
                                                    oi.interval().max());
        } else {
                return root_->intersect(ray, oi.interval().min(),
                                             oi.interval().max());
        }
}

} }
