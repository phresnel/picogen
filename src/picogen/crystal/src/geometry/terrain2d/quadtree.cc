#include "quadtree.h"
#include "deepness.h"

#include <stdexcept>
#include <iostream>
#include <limits>

namespace crystal { namespace geometry { namespace terrain2d {

struct EdgeDepths {
        int left, right, front, back;
};

Quadtree::Quadtree(Deepness const &deepness,
                   std::function<real(real,real)> fun,
                   int patchResolution)
{
        EdgeDepths ed;
        create (deepness,
                Rect (-15000,15000, -15000,15000),
                fun,
                patchResolution,
                0,
                ed);
}

Quadtree::~Quadtree() {
        if (leaf_) delete patch_;
        else delete [] children_;
}

PIntersection Quadtree::intersect_ (const Ray &ray) const {
        PInterval interval = crystal::intersect (ray,
                                                 rect_.left, min_h_, rect_.front,
                                                 rect_.right, max_h_, rect_.back);
        if (!interval) return PIntersection();

#if 1
        const real minT = interval->min,
                   maxT = interval->max;

        if (ray.direction.x()>=0) {
                if (ray.direction.z() >= 0) {
                        return intersect_<XDirection::Right, ZDirection::Forward>
                                (ray, minT, maxT);
                } else {
                        return intersect_<XDirection::Right, ZDirection::Backward>
                                (ray, minT, maxT);
                }
        } else {
                if (ray.direction.z() >= 0) {
                        return intersect_<XDirection::Left, ZDirection::Forward>
                                (ray, minT, maxT);
                } else {
                        return intersect_<XDirection::Left, ZDirection::Backward>
                                (ray, minT, maxT);
                }
        }
#else
        int order[4];

        const bool pdx = ray.direction.x() >= 0,
                   pdz = ray.direction.z() >= 0;
        if (pdx && pdz) {
                order[0] = 3;
                order[1] = 2;
                order[2] = 0;
                order[3] = 1;
        }
        else if (!pdx && pdz) {
                order[0] = 2;
                order[1] = 3;
                order[2] = 1;
                order[3] = 0;
        }
        else if (pdx && !pdz) {
                order[0] = 0;
                order[1] = 1;
                order[2] = 3;
                order[3] = 2;
        }
        else if (!pdx && !pdz) {
                order[0] = 1;
                order[1] = 0;
                order[2] = 2;
                order[3] = 3;
        }

        return intersect_slow (ray, order);
#endif
}

PIntersection Quadtree::intersect_slow (const Ray &ray, int *ordering) const
{
        if (!crystal::does_intersect (ray, rect_.left, min_h_, rect_.front,
                                      rect_.right, max_h_, rect_.back))
                return PIntersection();

        if (leaf_) return patch_->intersect (ray);


        for (int i=0; i<4; ++i) {
                Quadtree const &child = children_[ordering[i]];
                if (auto i = child.intersect_slow (ray, ordering))
                        return i;
        }
        return PIntersection();
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
struct Todo {
        Todo() : minT(-1), maxT(-2), node(0) {}
        Todo (real minT, real maxT, Quadtree const *node)
                : minT(minT), maxT(maxT), node(node) {}
        real minT, maxT;
        Quadtree const *node;
};

template <XDirection d_right, ZDirection d_up>
static void determine (Todo*& top,
                       real Min, real Max, real t_x, real t_z,
                       Quadtree *children) {

        // +----+----+
        // | 0  | 1  |
        // -----+-----
        // | 3  | 2  |
        // +----+----+
        typedef TravOrder<d_up, d_right> order;

        real from[4] = {0,0,0,0}, to[4] = {-1,-1,-1,-1}; // TODO: initialization prolly not needed
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
}

template <XDirection d_right, ZDirection d_up>
PIntersection Quadtree::intersect_ (const Ray &ray, real minT_, real maxT_) const
{
        const Direction &dir  = ray.direction;
        const real d_x = dir.x();
        const real d_y = dir.y();
        const real d_z = dir.z();
        const real id_x = 1 / d_x;
        const real id_z = 1 / d_z;
        const real o_y = ray.origin.y;
        const real o_x = ray.origin.x;
        const real o_z = ray.origin.z;

        //if (0 == dir.x()) return Intersection::Optional();
        //if (0 == dir.z()) return Intersection::Optional();

        //std::stack<Todo> todo; § replace me with somethint stack-framable
        Todo stack[128]; // TODO: rid magic number
        Todo *top = stack;
        *top++ = Todo(minT_, maxT_, this);
        while (top != stack) {
                const Todo curr = *--top;
                const Quadtree &node = *curr.node;

                //++globalTravCounter;

                // But we got to check for vertical intersection now.

                if (1) { // TODO: re-enable
                        const real min_h = o_y + curr.minT * d_y;
                        const real max_h = o_y + curr.maxT * d_y;

                        if (((min_h < node.min_h_) & (max_h < node.min_h_))
                           |((min_h > node.max_h_) & (max_h > node.max_h_))
                           //|(curr.minT>curr.maxT)
                           ) {
                                continue;
                        }
                }
                assert (curr.minT >= 0);

                if (node.leaf_) {
                        /*const auto i = (*node.patch_)
                                .fast_intersect<d_up, d_right>
                                (ray, curr.minT, curr.maxT,
                                 o_x, o_z, d_x, d_z, id_x, id_z);


                        //globalTravCounter = 100*(curr.maxT-curr.minT);
                        globalTravCounter = 100*(curr.minT);
                        if (i) return i;*/
                        const PIntersection i = node.patch_->intersect(ray);
                        if (i) return i;
                } else {
                        // Find out which ones to traverse.
                        const real c_x = 0.5*node.rect_.left + 0.5*node.rect_.right,
                                   c_z = 0.5*node.rect_.front+ 0.5*node.rect_.back,
                                   d_x = (c_x - o_x) * id_x,
                                   d_z = (c_z - o_z) * id_z;
                        determine<d_right, d_up> (top,
                                                  curr.minT, curr.maxT,
                                                  d_x, d_z,
                                                  node.children_);
                }
        }
        return PIntersection();
}


//-- creation ------------------------------------------------------------------

// note: To prevent LOD-differences of more than 1, we should build the tree
//       front-to-back, each recursion notifies
//       the parent about its four edge detail level. The builder can then
//       inform the other siblings about the maximum recursion allowed.

void Quadtree::create  (terrain2d::Deepness const &deepness,
                        Rect const &rect,
                        std::function<real(real,real)> fun,
                        int patchRes,
                        int depth,
                        EdgeDepths &ed)
{
        leaf_ = depth>=deepness.deepness(Point(0,0,0),
                                         rect.left, rect.right,
                                         rect.front, rect.back);
        rect_ = rect;
        if (leaf_) {
                make_leaf (deepness, fun, patchRes, depth);
        } else {
                make_inner (deepness, fun, patchRes, depth, ed);
        }
}

void Quadtree::make_inner  (terrain2d::Deepness const &deepness,
                            std::function<real(real,real)> fun,
                            int patchRes,
                            int depth,
                            EdgeDepths &
                            )
{
        const Point camera (0,0,0);
        ChildRects cr = child_boxen (rect_.left, rect_.right,
                                     rect_.front, rect_.back);

        children_ = new Quadtree[4];

        struct Child {
                Rect rect;
                Quadtree *child;
        } crr[4];
        for (int i=0; i<4; ++i) {
                crr[i].rect = cr[i];
                crr[i].child = children_+i;
        }

        // sort crr by distance
        /*std::sort (crr+0, crr+4,
                   [&](Child const &lhs, Child const &rhs) {
                        return deepness.deepness(camera, lhs.rect)
                             > deepness.deepness(camera, rhs.rect);
                   });*/

        /*for (int i=0; i<4; ++i) {
                std::cout << deepness.deepness(camera, crr[i].rect) << " " ;
        }
        std::cout << std::endl;*/

        for (int i=0; i<4; ++i) {
                EdgeDepths ced;
                crr[i].child->create (deepness, crr[i].rect,
                                      fun, patchRes, depth+1,
                                      ced);
                //ed.back
        }

        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();
        for (int i=0; i<4; ++i) {
                min_h_ = min (min_h_, children_[i].min_h_);
                max_h_ = max (max_h_, children_[i].max_h_);
        }
}

void Quadtree::make_leaf  (terrain2d::Deepness const &deepness,
                           std::function<real(real,real)> fun,
                           int patchRes,
                           int depth)
{
        Transition transition = Transition::None();

        const Point camera(0,0,0);

        if (depth < deepness.deepness (camera, rect_.front_neighbour()))
                transition |= Transition::Front();
        if (depth < deepness.deepness (camera, rect_.back_neighbour()))
                transition |= Transition::Back();
        if (depth < deepness.deepness (camera, rect_.left_neighbour()))
                transition |= Transition::Left();
        if (depth < deepness.deepness (camera, rect_.right_neighbour()))
                transition |= Transition::Right();

        patch_ = new Patch (rect_.left, rect_.right,
                            rect_.front, rect_.back,
                            fun, patchRes,
                            transition);
        min_h_ = patch_->min_h();
        max_h_ = patch_->max_h();
}

} } }
