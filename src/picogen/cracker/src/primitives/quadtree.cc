#include "quadtree.h"
#include "potentialintersection.h"
#include "ray.h"

namespace picogen { namespace cracker {

//== Implementation ============================================================
class QuadtreeImpl {
public:
        PotentialIntersection operator() (Ray const &ray) const;
};


PotentialIntersection QuadtreeImpl::operator () (Ray const &ray) const {
        return PotentialIntersection();
}

} }



//== Public Interface ==========================================================
namespace picogen { namespace cracker {

Quadtree::Quadtree() : impl_(new QuadtreeImpl) {
}

PotentialIntersection Quadtree::operator() (Ray const &ray) const {
        return (*impl_)(ray);
}

} }
