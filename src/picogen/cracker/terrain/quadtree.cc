#include "quadtree.h"
#include "quadtree/patch.h"
#include "color.h"
#include "math3d.h"
#include "materials/lambertmaterial.h"

#include <array>
#include <stack>
#include <functional>
#include <limits>

#include <QDebug>

namespace picogen { namespace cracker { namespace detail {

        // Except for debugging, the literals below don't have a meaning.
        enum class XDirection { Left = -1,     Right = 1 };
        enum class ZDirection { Backward = -1, Forward = 1 };

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

        template <ZDirection d_forward, XDirection d_right> struct TravOrder;
        template <> struct TravOrder<ZDirection::Forward, XDirection::Left> {
                enum {
                        child_a = 3,
                        child_b = 2,
                        child_c = 0,
                        child_d = 1
                };
        };
        template <> struct TravOrder<ZDirection::Forward, XDirection::Right> {
                enum {
                        child_a = 2,
                        child_b = 3,
                        child_c = 1,
                        child_d = 0
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

        class Node {
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                // Creates a root node.
                Node (unsigned int depth,
                      std::function<real (real,real)> const & height,
                      BoundingBox &rootBB)
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
                        create(depth,
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

                struct Todo {
                        Todo() {}
                        Todo (real minT, real maxT, Node *node)
                                : minT(minT), maxT(maxT), node(node) {}
                        real minT, maxT;
                        Node *node;
                        int debug;
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
                        real from[4] = {0,0,0,0}, to[4] = {-1,-1,-1,-1};

                        {
                                from[order::child_a] = Min;
                                to  [order::child_a] = std::min(std::min(t_x, t_z), Max);

                                //qDebug() << "child_b-min" << std::max(t_x, Min) << "child_b" << order::child_b;
                                from[order::child_b] = std::max(t_x, Min);
                                to  [order::child_b] = std::min(t_z, Max);

                                from[order::child_c] = std::max(t_z, Min);
                                to  [order::child_c] = std::min(t_x, Max);

                                from[order::child_d] = std::max(std::max(t_x, t_z), Min);
                                to  [order::child_d] = Max;

                                //if (from[order::child_a] < 0) qDebug () << order::child_a << from[order::child_a] << ":" << to[order::child_a] << ", " << t_x << t_z << Min;

                        }
                        //qDebug() << Min;
                        for (int i=0; i<4; ++i) {
                                if (from[i] < 0) qDebug () << i << from[i] << ":" << to[i] << ", " << t_x << t_z << Min;

                        }

                        top->debug = order::child_d;
                        *top++ = Todo(from[order::child_d],
                                      to  [order::child_d],
                                      children+order::child_d);

                        top->debug = order::child_c;
                        *top++ = Todo(from[order::child_c],
                                      to  [order::child_c],
                                      children+order::child_c);

                        top->debug = order::child_b;
                        *top++ = Todo(from[order::child_b],
                                      to  [order::child_b],
                                      children+order::child_b);

                        top->debug = order::child_a;
                        *top++ = Todo(from[order::child_a],
                                      to  [order::child_a],
                                      children+order::child_a);
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
                        const real d_y = dir.y();
                        const real o_y = ray.origin().y();
                        const real o_x = ray.origin().x();
                        const real o_z = ray.origin().z();

                        //if (0 == dir.x()) return Intersection::Optional();
                        //if (0 == dir.z()) return Intersection::Optional();

                        const real id_x = 1 / dir.x();
                        const real id_z = 1 / dir.z();


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
                                   |(curr.minT>curr.maxT)
                                   ) {
                                        continue;
                                }

                                if (curr.minT < 0) {
                                        qDebug() << "??" << curr.minT << curr.maxT
                                                 << "rayd" << ray.direction().x() << ray.direction().z()
                                                 << "-- right"<<(int)d_right
                                                 << "up"<<(int)d_up
                                                 << "-- child" << curr.debug;
                                        break;
                                }
                                //return Intersection::Optional();


                                if (node.leaf_) {
                                        const auto i = (*node.patch_)(ray,
                                                                      curr.minT,
                                                                      curr.maxT);
                                        if (i) return i;
                                } else {
                                        // Find out which ones to traverse.
                                        const real c_x = 0.5*node.left_ + 0.5*node.right_;
                                        const real c_z = 0.5*node.front_+ 0.5*node.back_;
                                        const real d_x = (c_x - o_x) * id_x;
                                        const real d_z = (c_z - o_z) * id_z;
//qDebug() << "!!!" << minT_ << maxT_ << d_x << d_z << "rdx" << ray.direction().x(); // try to find nan
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
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);
                        //const real distance = length (aabb.center() - center);
                        //qDebug() << distance << " <- " <<
                        //            (13/(1+0.02*distance));
                        left_  = left;
                        right_ = right;
                        front_ = front;
                        back_  = back;

                        if (leaf)
                                makeLeaf(left, right,
                                         front, back,
                                         height);
                        else makeInner(depth,
                                       left, right,
                                       front, back, height);
                }
                void makeLeaf (real left, real right,
                               real front, real back,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = true;
                        patch_ = new Patch (left, right,
                                            front, back,
                                            4,4,
                                            height,
                                            min_h_, max_h_);
                }
                void makeInner(unsigned int depth,
                               real left, real right,
                               real front, real back,
                               std::function<real (real,real)> const & height)
                {
                        leaf_ = false;
                        const auto childBoxes = child_boxen(left, right,
                                                            front, back);
                        children_ = new Node[4];
                        for (size_t i=0; i<4; ++i) {
                                children_[i].create (depth-1,
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
} } }



namespace picogen { namespace cracker {

Quadtree::Quadtree ()
{
        const auto fun = [](real x,real y) {
                return -30 + 15 * cos(y*0.1) * cos(x*0.1);
        };

        qDebug() << "sizeof(Node) =" << sizeof(detail::Node);
        root_.reset (new detail::Node (4, fun, aabb_));
}

Intersection::Optional Quadtree::operator() (Ray const &ray_) const {
        Ray ray (ray_.origin(),
                 normalize<Direction>(
                         ray_.direction().x()==0 ? 0.00001 : ray_.direction().x(),
                         ray_.direction().y()==0 ? 0.00001 : ray_.direction().y(),
                         ray_.direction().z()==0 ? 0.00001 : ray_.direction().z()
                 ));
        Interval::Optional oi = intersect (ray, aabb_);
        if (!oi) return Intersection::Optional();

        real min = oi.interval().min(),
             max = oi.interval().max();
        if (min < 0) min = 0;
        if (min > max) return Intersection::Optional();

        return detail::Node::intersect_iter(&*root_, ray,
                                            min,
                                            max);
}

} }
