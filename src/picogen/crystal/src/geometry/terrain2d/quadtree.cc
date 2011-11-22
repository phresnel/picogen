#include "quadtree.h"
#include "deepness.h"

#include <stdexcept>
#include <iostream>

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
        const BoundingBox aabb (Point (rect_.left, min_h_, rect_.front),
                                Point (rect_.right, max_h_, rect_.back));
        if (!crystal::intersect (ray, aabb))
                return PIntersection();

        if (leaf_) return patch_->intersect (ray);

        for (auto it = children_, end = children_+4; it!=end; ++it) {
                if (auto ret = it->intersect_ (ray)) return ret;
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
        min_h_ = -1000;
        max_h_ = 1000;
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
}

void Quadtree::make_leaf  (std::function<real(real,real)> fun,
                           int patchRes)
{
        patch_ = new Patch (rect_.left, rect_.right,
                            rect_.front, rect_.back,
                            fun, patchRes);
}

} } }
