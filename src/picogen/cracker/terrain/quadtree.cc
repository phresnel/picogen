#include "quadtree.h"

namespace picogen { namespace cracker {

namespace {

class Node {
public:
        Node()                         = delete;
        Node& operator= (Node const &) = delete;
        Node(Node const &)             = delete;
private:

        Node *children_;
};

}

Quadtree::Quadtree () {
}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        return Intersection::Optional();
}

} }
