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
                Rect (-50000,50000, -50000,50000),
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
        const bool pdx = ray.direction.positive_x(),
                   pdz = ray.direction.positive_z();
        int order[4];

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

        return intersect_ (ray, order);
}

PIntersection Quadtree::intersect_ (const Ray &ray, int *ordering) const
{
        if (!crystal::does_intersect (ray, rect_.left, min_h_, rect_.front,
                                      rect_.right, max_h_, rect_.back))
                return PIntersection();

        if (leaf_) return patch_->intersect (ray);


        for (int i=0; i<4; ++i) {
                Quadtree const &child = children_[ordering[i]];
                if (auto i = child.intersect_ (ray, ordering))
                        return i;
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
