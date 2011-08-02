#include "quadtree.h"
#include "aabb.h"
#include "color.h"
#include "materials/lambertmaterial.h"

#include <array>
#include <functional>
#include <limits>

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
                Color color_;
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                // Creates a root node.
                Node (unsigned int depth,
                      std::function<real (real,real)> const & height)
                {
                        create(depth, BoundingBox (Point(0,-30,0),
                                                   128,
                                                   2,
                                                   123),
                               height);
                }

                ~Node () {
                        delete [] children_;
                }

                Intersection::Optional operator() (Ray const &ray) const {
                        if (Interval::Optional oi = intersect(ray, aabb_)) {

                                if (!children_) return Intersection(
                                                    oi.interval().min(),
                                                    Normal(0,1,0),
                                                    std::shared_ptr<Material>(new LambertMaterial(color_)),
                                                    DifferentialGeometry(Normal(0,1,0),
                                                                         Normal(0,1,0),
                                                                         Normal(1,0,0),
                                                                         Normal(0,0,1)));
                                Intersection::Optional ci[4] = {
                                        children_[0](ray),
                                        children_[1](ray),
                                        children_[2](ray),
                                        children_[3](ray)
                                };

                                int nearest = -1;
                                real nearest_dist = -1;
                                for (int i=0; i<4; ++i) {
                                        if (ci[i]) {
                                                if (nearest == -1
                                                  || ci[i].intersection().distance()<nearest_dist)
                                                {
                                                        nearest = i;
                                                        nearest_dist = ci[i].intersection().distance();
                                                }
                                        }
                                }
                                if (nearest >= 0) return ci[nearest];
                        }
                        return Intersection::Optional();
                }

        private:
                Node() {}

                void create(unsigned int depth,
                            BoundingBox const &aabb,
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);

                        if (leaf) makeLeaf(aabb, height);
                        else makeInner(depth, aabb, height);
                }
                void makeLeaf (BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        children_ = 0;
                        aabb_ = aabb;
                        color_ = Color::FromRgb(
                                                rand() / (float)RAND_MAX,
                                                rand() / (float)RAND_MAX,
                                                rand() / (float)RAND_MAX
                                            );

                        refineBoundingBox (height);
                }
                void makeInner(unsigned int depth, BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        const auto childBoxes = child_boxen(aabb);
                        children_ = new Node[4];
                        children_[0].create (depth-1, childBoxes[0], height);
                        children_[1].create (depth-1, childBoxes[1], height);
                        children_[2].create (depth-1, childBoxes[2], height);
                        children_[3].create (depth-1, childBoxes[3], height);
                        refineBoundingBox();
                }

                // BoundingBox refinement for leaf nodes
                void refineBoundingBox (std::function<real(real,real)> const &height)
                {
                        real min_h = std::numeric_limits<real>::max(),
                             max_h = -std::numeric_limits<real>::max();
                        const Point min = aabb_.min(),
                                    max = aabb_.max();
                        for (real z=min.z(); z<=max.z(); z+=0.1) {
                                for (real x=min.x(); x<=max.x(); x+=0.1) {
                                        const real h = height(x,z);
                                        if (h < min_h) {
                                                min_h = h;
                                        }
                                        if (h > max_h) {
                                                max_h = h;
                                        }
                                }
                        }

                        aabb_ = BoundingBox (Point(min.x(), min_h, min.z()),
                                             Point(max.x(), max_h, max.z()));
                }

                // Refinement for inner nodes.
                // Pre-condition: * child nodes are refined
                //                * has child nodes
                void refineBoundingBox () {
                        real min_h = children_[0].aabb_.min().y();
                        min_h = min(min_h, children_[1].aabb_.min().y());
                        min_h = min(min_h, children_[2].aabb_.min().y());
                        min_h = min(min_h, children_[3].aabb_.min().y());
                        real max_h = children_[0].aabb_.max().y();
                        max_h = max(max_h, children_[1].aabb_.max().y());
                        max_h = max(max_h, children_[2].aabb_.max().y());
                        max_h = max(max_h, children_[3].aabb_.max().y());

                        const Point &min = aabb_.min(),
                                    &max = aabb_.max();
                        aabb_ = BoundingBox (Point(min.x(), min_h, min.z()),
                                             Point(max.x(), max_h, max.z()));
                }

        private:
                Node *children_;
                BoundingBox aabb_;
        };
}

Quadtree::Quadtree ()
: root_(new detail::Node (3,
                          [](real x,real y) { return -10+5*cos(x*0.1)*cos(y*0.1); }))
{
}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        return (*root_)(ray);
}

} }
