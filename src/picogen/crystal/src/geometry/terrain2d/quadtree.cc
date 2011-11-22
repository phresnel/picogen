#include "quadtree.h"
#include <stdexcept>

namespace crystal { namespace geometry { namespace terrain2d {

Quadtree::Quadtree(std::function<real(real,real)> fun, int resolution)
        : patch_(new Patch (fun, resolution)), leaf_(true)
{
}

Quadtree::~Quadtree() {
        if (leaf_) delete patch_;
        else delete [] children_;
}

PIntersection Quadtree::intersect_ (const Ray &ray) const {
        if (leaf_)
                return patch_->intersect (ray);
        throw std::runtime_error ("inner nodes not implemented");
}

} } }
