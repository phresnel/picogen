#ifndef QUADTREE_H_INCLUDED_20110801
#define QUADTREE_H_INCLUDED_20110801

#include "intersection.h"
#include "aabb.h"

#include <memory>

namespace picogen { namespace cracker {

namespace detail {
        class Node;
}

class Ray;
class Normal;

class Quadtree {
public:
        Quadtree () = delete;
        Quadtree (Point cameraPosition,
                  int maxRecursion,          // maximum recursion depth
                  real distanceOfMaxDetail,  // range to which max detail is used
                  real distanceOfMinDetail); // range beyond which min detail is used
        Intersection::Optional operator() (Ray const &ray) const;
private:
        std::shared_ptr<detail::Node> root_;
        BoundingBox aabb_;
};

} }

#endif // QUADTREE_H_INCLUDED_20110801
