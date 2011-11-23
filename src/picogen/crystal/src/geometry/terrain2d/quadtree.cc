#include "quadtree.h"
#include "deepness.h"

#include <stdexcept>
#include <iostream>
#include <limits>

namespace crystal { namespace geometry { namespace terrain2d {

Quadtree::Quadtree(Deepness const &deepness,
                   std::function<real(real,real)> fun,
                   int patchResolution)
{
        create (deepness,
                Rect (-500,500, -500,500),
                fun,
                patchResolution,
                0);
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

void Quadtree::create  (terrain2d::Deepness const &deepness,
                        Rect const &rect,
                        std::function<real(real,real)> fun,
                        int patchRes,
                        int depth)
{
        leaf_ = depth>=deepness.deepness(Point(0,0,0),
                                         rect.left, rect.right,
                                         rect.front, rect.back);
        rect_ = rect;
        if (leaf_) {
                make_leaf (fun, patchRes);
        } else {
                make_inner (deepness, fun, patchRes, depth);
        }
}

void Quadtree::make_inner  (terrain2d::Deepness const &deepness,
                            std::function<real(real,real)> fun,
                            int patchRes,
                            int depth)
{
        ChildRects cr = child_boxen (rect_.left, rect_.right,
                                     rect_.front, rect_.back);
        children_ = new Quadtree[4];
        children_[0].create (deepness, cr[0], fun, patchRes, depth+1);
        children_[1].create (deepness, cr[1], fun, patchRes, depth+1);
        children_[2].create (deepness, cr[2], fun, patchRes, depth+1);
        children_[3].create (deepness, cr[3], fun, patchRes, depth+1);

        min_h_ =  std::numeric_limits<real>::max();
        max_h_ = -std::numeric_limits<real>::max();
        for (int i=0; i<4; ++i) {
                if (children_[i].min_h_ < min_h_) min_h_ = children_[i].min_h_;
                if (children_[i].max_h_ > max_h_) max_h_ = children_[i].max_h_;
        }
}

void Quadtree::make_leaf  (std::function<real(real,real)> fun,
                           int patchRes)
{
        patch_ = new Patch (rect_.left, rect_.right,
                            rect_.front, rect_.back,
                            fun, patchRes,
                            Transition::Front()
                           );
        min_h_ = patch_->min_h();
        max_h_ = patch_->max_h();
}

} } }
