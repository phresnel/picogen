#ifndef NODE_H_INCLUDED_20111103
#define NODE_H_INCLUDED_20111103

#include <array>
#include <cassert>

#include <iostream>

#include "patch.h"
#include "aabb.h"
#include "real.h"

namespace picogen { namespace cracker { namespace detail {

        // Except for debugging, the literals below don't have a meaning.
        //enum class XDirection { Left = -1,     Right = 1 };
        //enum class ZDirection { Backward = -1, Forward = 1 };
        // currently declared in patch.h

        struct BoundingQuad { real left, right, front, back; };
        typedef std::array<BoundingQuad,4> ChildBoundingQuads;
        inline ChildBoundingQuads child_boxen (
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
                std::array<BoundingQuad,4> ret;
                ret [0] = {cx - newW, cx       ,  cz     , cz + newD};
                ret [1] = {cx       , cx + newW,  cz     , cz + newD};
                ret [2] = {cx       , cx + newW,  cz-newD, cz       };
                ret [3] = {cx - newW, cx       ,  cz-newD, cz       };
                return ret;
        }

        template <ZDirection d_forward, XDirection d_right> struct TravOrder;

        template <> struct TravOrder<ZDirection::Forward, XDirection::Left> {
                enum {
                        child_a = 2,
                        child_b = 3,
                        child_c = 1,
                        child_d = 0
                };
        };
        template <> struct TravOrder<ZDirection::Forward, XDirection::Right> {
                enum {
                        child_a = 3,
                        child_b = 2,
                        child_c = 0,
                        child_d = 1
                };
        };
        template <> struct TravOrder<ZDirection::Backward, XDirection::Right> {
                enum {
                        child_a = 0,
                        child_b = 1,
                        child_c = 3,
                        child_d = 2
                };
        };
        template <> struct TravOrder<ZDirection::Backward, XDirection::Left> {
                enum {
                        child_a = 1,
                        child_b = 0,
                        child_c = 2,
                        child_d = 3
                };
        };

        struct NodeDetail {
                int maxRecursion;
                real maxDetailRange, minDetailRange;

                NodeDetail (int r, real maxD, real minD)
                        : maxRecursion (r),
                          maxDetailRange(maxD), minDetailRange(minD)
                {
                        assert (maxD < minD);
                }

                int deepness (Point camera,
                              real left, real right,
                              real front, real back
                ) const {
                        assert (left <= right);
                        assert (front <= back);
                        const real x = camera.x(),
                                   z = camera.z();
                        real dist;
                        if (x>=left && x<=right &&
                            z>=front && z<=back)
                        {
                                dist = 0;
                        } else if (x>=left && x<=right) {
                                dist = std::min(std::fabs(z-front),
                                                std::fabs(z-back));
                        } else if (z>=front && z<=back) {
                                dist = std::min(std::fabs(x-left),
                                                std::fabs(x-right));
                        } else {
                                const real xl = std::fabs (x-left),
                                           xr = std::fabs (x-right),
                                           zf = std::fabs (z-front),
                                           zb = std::fabs (z-back);
                                const real a = xl*xl + zb*zb,
                                           b = xl*xl + zf*zf,
                                           c = xr*xr + zb*zb,
                                           d = xr*xr + zf*zf;
                                real min = a;
                                if (b < min) min = b;
                                if (c < min) min = c;
                                if (d < min) min = d;

                                dist = std::sqrt(min);
                        }

                        //const real dist = length (p - camera);
                        const real l_ = std::max (real(0), dist - maxDetailRange)
                                       / (minDetailRange - maxDetailRange);
                        const real l = l_ < 0 ? 0 : l_ > 1 ? 1 : l_;

                        const int ret = int(maxRecursion - real(maxRecursion) * l);
                        /*qDebug() << "[" << camera.z() << ", l:" << l << l_ << ", dparams:"
                                 << maxRecursion << maxDetailRange << minDetailRange << "]";*/
                        /*qDebug() //<< dist << " --> " << l << " --> "
                                 << ret
                                    << "(" << 0.5*(left+right)
                                           << 0.5*(front+back) << ")"
                                    << "|" << dist << "|";*/
                        return ret;
                }
        };

        class Node {
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                // Creates a root node.
                Node (NodeDetail detail,
                      std::function<real (real,real)> const & height,
                      Point cameraPosition,
                      BoundingBox &rootBB)
                {
                        qDebug() << "creating Quadtree";
                        const real c_x = 0, c_z = 0;
                        const real left = c_x-300,
                                   right = c_x+300,
                                   front = c_z-300,
                                   back = c_z+300;
                        left_ = left;
                        right_ = right;
                        front_ = front;
                        back_ = back;
                        qDebug() << "root-node c: " << cameraPosition.z();
                        create(0, detail,
                               left, right,
                               front, back,
                               cameraPosition,
                               height);
                        rootBB = BoundingBox (Point(left, min_h_, front),
                                              Point(right, max_h_, back));
                        qDebug() << "Quadtree created.";
                }

                ~Node () {
                        if (leaf_) delete patch_;
                        else delete [] children_;
                }

                struct Todo {
                        Todo() {}
                        Todo (real minT, real maxT, Node *node)
                                : minT(minT), maxT(maxT), node(node) {}
                        real minT, maxT;
                        Node *node;
                };

                template <XDirection d_right, ZDirection d_up>
                static void determine (Todo*& top,
                                       real Min, real Max, real t_x, real t_z,
                                       Node *children) {

                        // +----+----+
                        // | 0  | 1  |
                        // -----+-----
                        // | 3  | 2  |
                        // +----+----+
                        typedef TravOrder<d_up, d_right> order;

                        #if 0
                        auto add = [&] (real from, real to, int order) {
                                if (from<=to) {
                                        *top++ = Todo(from, to,
                                                      children+order);
                                }
                        };
                        add (std::max(std::max(t_x, t_z), Min), Max,
                             order::child_d);
                        add (std::max(t_z, Min), std::min(t_x, Max),
                             order::child_c);
                        add (std::max(t_x, Min), std::min(t_z, Max),
                             order::child_b);
                        add (Min, std::min(std::min(t_x, t_z), Max),
                             order::child_a);

                        #else
                        real from[4] = {0,0,0,0}, to[4] = {-1,-1,-1,-1};
                        {
                                from[order::child_a] = Min;
                                to  [order::child_a] = std::min(std::min(t_x, t_z), Max);

                                from[order::child_b] = std::max(t_x, Min);
                                to  [order::child_b] = std::min(t_z, Max);

                                from[order::child_c] = std::max(t_z, Min);
                                to  [order::child_c] = std::min(t_x, Max);

                                from[order::child_d] = std::max(std::max(t_x, t_z), Min);
                                to  [order::child_d] = Max;
                        }

                        auto push = [&] (int order) {
                                if (from[order] <= to[order])
                                        *top++ = Todo(from[order],
                                                      to  [order],
                                                      children+order);
                        };

                        push (order::child_d);
                        push (order::child_c);
                        push (order::child_b);
                        push (order::child_a);
                        #endif
                }

                template <XDirection d_right, ZDirection d_up>
                static Intersection::Optional intersect_iter_directed (
                        Node *node,
                        Ray const &ray,
                        real minT_, real maxT_

                ) {
                        assert(  (d_right == XDirection::Right && ray.direction().x()>=0)
                              || (d_right == XDirection::Left  && ray.direction().x()<=0));
                        assert(  (d_up == ZDirection::Forward  && ray.direction().z()>=0)
                              || (d_up == ZDirection::Backward && ray.direction().z()<=0));

                        const Direction dir  = ray.direction();
                        const real d_x = dir.x();
                        const real d_y = dir.y();
                        const real d_z = dir.z();
                        const real id_x = 1 / d_x;
                        const real id_z = 1 / d_z;
                        const real o_y = ray.origin().y();
                        const real o_x = ray.origin().x();
                        const real o_z = ray.origin().z();

                        //if (0 == dir.x()) return Intersection::Optional();
                        //if (0 == dir.z()) return Intersection::Optional();

                        //std::stack<Todo> todo; § replace me with somethint stack-framable
                        Todo stack[128];
                        Todo *top = stack;
                        *top++ = Todo(minT_, maxT_, node); //wrong state restore?
                        while (top != stack) {
                                const Todo curr = *--top;
                                const Node &node = *curr.node;

                                // But we got to check for vertical intersection now.
                                const real min_h = o_y + curr.minT * d_y;
                                const real max_h = o_y + curr.maxT * d_y;


                                // BOTTLENECK when this->aabb.get...?
                                if (((min_h < node.min_h_) & (max_h < node.min_h_))
                                   |((min_h > node.max_h_) & (max_h > node.max_h_))
                                   //|(curr.minT>curr.maxT)
                                   ) {
                                        continue;
                                }

                                assert (curr.minT >= 0);

                                if (node.leaf_) {
                                        const auto i = (*node.patch_)
                                                .fast_intersect<d_up,
                                                                d_right>
                                                (ray, curr.minT, curr.maxT,
                                                 o_x, o_z, d_x, d_z, id_x, id_z);
                                        if (i) return i;
                                } else {
                                        // Find out which ones to traverse.
                                        const real c_x = 0.5*node.left_ + 0.5*node.right_;
                                        const real c_z = 0.5*node.front_+ 0.5*node.back_;
                                        const real d_x = (c_x - o_x) * id_x;
                                        const real d_z = (c_z - o_z) * id_z;
                                        determine<d_right, d_up> (top,
                                                                  curr.minT, curr.maxT,
                                                                  d_x, d_z,
                                                                  node.children_);
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
                                        return intersect_iter_directed<XDirection::Right,
                                                                       ZDirection::Forward>
                                                (node, ray, minT, maxT);
                                } else {
                                        return intersect_iter_directed<XDirection::Right,
                                                                       ZDirection::Backward>
                                                (node, ray, minT, maxT);
                                }
                        } else {
                                if (ray.direction().z() >= 0) {
                                        return intersect_iter_directed<XDirection::Left,
                                                                       ZDirection::Forward>
                                                (node, ray, minT, maxT);
                                } else {
                                        return intersect_iter_directed<XDirection::Left,
                                                                       ZDirection::Backward>
                                                (node, ray, minT, maxT);
                                }
                        }
                }

                Node() {}

                static Intersection::Optional intersect_rec (Node *node,
                                                             Ray const &ray,
                                                             real minT, real maxT)
                {
                        BoundingBox aabb (Point(node->left_, node->min_h_, node->front_),
                                          Point(node->right_, node->max_h_, node->back_));
                        if (auto in = intersect (ray, aabb)) {
                                if (node->leaf_) {
                                        return node->patch_->slow_intersect(ray, minT, maxT);
                                }
                                real nearest = -1;
                                Intersection::Optional n;
                                for (int i=0; i<4; ++i) {
                                        const Intersection::Optional op
                                                = intersect_rec(node->children_+i,
                                                                ray, minT, maxT);
                                        if (op) {
                                                const Intersection& i = op.intersection();
                                                if (i.distance() < nearest || nearest<0) {
                                                        n = op;
                                                        nearest = i.distance();
                                                }
                                        }
                                }
                                return n;
                        }
                        return Intersection::Optional();
                }

                void create(int depth,
                            NodeDetail detail,
                            real left, real right, real front, real back,
                            Point cameraPosition,
                            std::function<real (real,real)> const & height
                ) {
                        const Point center (
                                0.5*right + 0.5*left,
                                0,
                                0.5*back + 0.5*front
                        );
                        const bool leaf = (depth>=detail.deepness(cameraPosition,
                                                                  left, right,
                                                                  front, back));

                        /*if (leaf) {
                                qDebug() << cameraPosition.x() << cameraPosition.y() << cameraPosition.z()
                                        << ":" << center.x() << center.y() << center.z()
                                        << ":" << detail.deepness(cameraPosition,
                                                                  center);

                        }*/
                        left_  = left;
                        right_ = right;
                        front_ = front;
                        back_  = back;

                        if (leaf)
                                makeLeaf(left, right,
                                         front, back,
                                         cameraPosition,
                                         height);
                        else makeInner(depth, detail,
                                       left, right,
                                       front, back,
                                       cameraPosition,
                                       height);

                        for (int i=0; i<depth; ++i)
                                std::cout << "  ";
                        std::cout << "  {" << depth << "} "
                                  << this->min_h_ << ".." << this->max_h_
                                  //<< " [" << left_ << " " << right_ << " "
                                  //<< front_ << " " << back_ << "]"
                                  << std::endl;;

                }
                void makeLeaf (real left, real right,
                               real front, real back,
                               Point cameraPosition,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = true;
                        patch_ = new Patch (left, right,
                                            front, back,
                                            cameraPosition,
                                            4,4,//TODO: 32 is good
                                            height,
                                            min_h_, max_h_,
                                            Patch::LodSmoothing::None
                                            );
                }
                void makeInner(int depth, NodeDetail detail,
                               real left, real right,
                               real front, real back,
                               Point cameraPosition,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = false;
                        const ChildBoundingQuads childBoxes =
                                        child_boxen(left, right,
                                                    front, back);
                        children_ = new Node[4];
                        for (size_t i=0; i<4; ++i) {
                                children_[i].create (depth+1, detail,
                                                     childBoxes[i].left,
                                                     childBoxes[i].right,
                                                     childBoxes[i].front,
                                                     childBoxes[i].back,
                                                     cameraPosition,
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
} } }

#endif // NODE_H_INCLUDED_20111103
