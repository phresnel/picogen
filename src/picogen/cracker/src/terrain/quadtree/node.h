#ifndef NODE_H_INCLUDED_20111103
#define NODE_H_INCLUDED_20111103

#include <array>
#include <cassert>

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
                {}

                int deepnes (Point camera, Point p) const {
                        const real dist = length (p - camera);
                        const real l = std::max (real(0), dist - maxDetailRange)
                                       / (minDetailRange - maxDetailRange);
                        return maxRecursion - maxRecursion * l;
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
                        const Point center (0, -40, 50);
                        const real left = center.x()-100,
                                   right = center.x()+100,
                                   front = center.z()-100,
                                   back = center.z()+100;
                        left_ = left;
                        right_ = right;
                        front_ = front;
                        back_ = back;
                        create(detail.maxRecursion,
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
                        *top++ = Todo(minT_, maxT_, node);
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

                void create(unsigned int depth,
                            real left, real right, real front, real back,
                            Point cameraPosition,
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);
                        left_  = left;
                        right_ = right;
                        front_ = front;
                        back_  = back;

                        if (leaf)
                                makeLeaf(left, right,
                                         front, back,
                                         cameraPosition,
                                         height);
                        else makeInner(depth,
                                       left, right,
                                       front, back,
                                       cameraPosition,
                                       height);
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
                                            8,8,
                                            height,
                                            min_h_, max_h_,
                                            Patch::LodSmoothing::None
                                            );
                }
                void makeInner(unsigned int depth,
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
                                children_[i].create (depth-1,
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
