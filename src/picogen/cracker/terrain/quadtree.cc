#include "quadtree.h"
#include "aabb.h"

#include <array>

#include <QDebug>

namespace picogen { namespace cracker {

namespace detail {

        inline std::array<BoundingBox,4> child_boxen (BoundingBox const &aabb) {
                const Point c = aabb.center();
                const real oldW = aabb.width(),
                           oldH = aabb.height(),
                           oldD = aabb.depth(),
                           newW = oldW / 2,
                           newH = oldH,
                           newD = oldD / 2
                          ;

                std::array<BoundingBox,4> ret;
                ret[0] = BoundingBox (c + Vector(-newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[1] = BoundingBox (c + Vector(+newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[2] = BoundingBox (c + Vector(+newW/2, 0, -newD/2),
                                      newW, newH, newD);
                ret[3] = BoundingBox (c + Vector(-newW/2, 0, -newD/2),
                                      newW, newH, newD);
                return ret;
        }

        class Node {
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                Node (unsigned int depth) {
                        create(depth, BoundingBox (Point(0,0,0),
                                                   128,
                                                   2,
                                                   123));
                }

                ~Node () {
                        delete [] children_;
                }

                Intersection::Optional operator() (Ray const &ray) const {
                        return Intersection::Optional();
                }

        private:
                Node() {}

                void create(unsigned int depth,
                            BoundingBox const &aabb
                ) {
                        const bool leaf = (depth==0);

                        if (leaf) makeLeaf(aabb);
                        else makeInner(depth, aabb);
                }
                void makeLeaf (BoundingBox const &aabb) {
                        children_ = 0;
                        aabb_ = aabb;
                }
                void makeInner(unsigned int depth, BoundingBox const &aabb) {
                        const auto childBoxes = child_boxen(aabb);
                        children_ = new Node[4];
                        children_[0].create (depth-1, childBoxes[0]);
                        children_[1].create (depth-1, childBoxes[1]);
                        children_[2].create (depth-1, childBoxes[2]);
                        children_[3].create (depth-1, childBoxes[3]);
                        aabb_ = aabb;
                }

        private:
                Node *children_;
                BoundingBox aabb_;
        };
}

Quadtree::Quadtree () : root_(new detail::Node (4)) {
}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        return (*root_)(ray);
}

} }
