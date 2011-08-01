#ifndef QUADTREE_H_INCLUDED_20110801
#define QUADTREE_H_INCLUDED_20110801

#include "intersection.h"

namespace picogen { namespace cracker {

class Ray;
class Normal;

class Quadtree {
public:
        Quadtree ();
        Intersection::Optional operator() (Ray const &ray) const;
};

} }

#endif // QUADTREE_H_INCLUDED_20110801
