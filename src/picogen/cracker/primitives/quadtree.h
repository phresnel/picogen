#ifndef QUADTREE_H_20110614
#define QUADTREE_H_20110614

#include <memory>

namespace picogen { namespace cracker {

class Ray;
class PotentialIntersection;

class QuadtreeImpl;

class Quadtree {
public:
        Quadtree ();
        PotentialIntersection operator() (Ray const &) const;

private:
        std::shared_ptr<QuadtreeImpl> impl_;
};

} }
#endif // QUADTREE_H_20110614
