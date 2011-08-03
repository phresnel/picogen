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

        class Patch {
        public:
                Patch ()                         = delete;
                Patch (Patch const &)            = delete;
                Patch& operator= (Patch const &) = delete;


                Patch (real left, real right,
                       real front, real back,
                       unsigned int res_x,
                       unsigned int res_z,
                       std::function<real (real, real)> h,
                       BoundingBox &exactBB)
                        : left_(left), right_(right), front_(front), back_(back)
                        , res_x_(res_x)
                        , h_(new real [res_x*res_z])
                {
                        assert (res_x != 0);
                        assert (res_z != 0);
                        assert (left < right);
                        assert (front < back);

                        real y_min = std::numeric_limits<real>::max(),
                             y_max = -y_min;
                        for (unsigned int uz=0; uz<res_z; ++uz) {
                                for (unsigned int ux=0; ux<res_x; ++ux) {
                                        const real u = ux/static_cast<real>(res_x-1);
                                        const real v = ux/static_cast<real>(res_x-1);

                                        const real x = (1-u)*left + u*right;
                                        const real z = (1-v)*front + v*back;
                                        const real y = h(x,z);

                                        set(ux,uz, y);
                                }
                        }

                        exactBB = BoundingBox (Point(left, y_min, front),
                                               Point(right, y_max, back));
                }

                ~Patch() {
                        delete [] h_;
                }

                Intersection::Optional operator() (Ray const &ray) const {
                        return Intersection::Optional();
                }

                /*BoundingBox exactBoundingBox () const {
                        real min_y,
                }*/

        private:
                real left_, right_, front_, back_;
                unsigned int res_x_; // res_z_ is not needed after construction
                real *h_;

                real h (unsigned int x, unsigned z) const {
                        return h_[x + z*res_x_];
                }
                void set (unsigned int x, unsigned z, real value) {
                        h_[x + z*res_x_] = value;
                }
        };
}

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
                        Point center (0, -30, 0);
                        create(depth, center, BoundingBox (center,
                                                           128,
                                                           2,
                                                           123),
                               height);
                }

                ~Node () {
                        delete [] children_;
                        delete patch_;
                }

                Intersection::Optional operator() (Ray const &ray) const {
                        if (Interval::Optional oi = intersect(ray, aabb_)) {

                                if (!children_) {
                                        return (*patch_)(ray);
                                }
//                                        return Intersection(
//                                                    oi.interval().min(),
//                                                    Normal(0,1,0),
//                                                    std::shared_ptr<Material>(new LambertMaterial(color_)),
//                                                    DifferentialGeometry(Normal(0,1,0),
//                                                                         Normal(0,1,0),
//                                                                         Normal(1,0,0),
//                                                                         Normal(0,0,1)));
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
                            Point const &center,
                            BoundingBox const &aabb,
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);
                        //const real distance = length (aabb.center() - center);
                        //qDebug() << distance << " <- " <<
                        //            (13/(1+0.02*distance));

                        if (leaf) makeLeaf(center, aabb, height);
                        else makeInner(center, depth, aabb, height);
                }
                void makeLeaf (Point const &center,
                               BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        children_ = 0;
                        //aabb_ = aabb;
                        color_ = Color::FromRgb(
                                                rand() / (float)RAND_MAX,
                                                rand() / (float)RAND_MAX,
                                                rand() / (float)RAND_MAX
                                            );
                        patch_ = new Patch (aabb.min().x(), aabb.max().x(),
                                            aabb.min().z(), aabb.max().z(),
                                            8,8,
                                            height,
                                            aabb_);
                        //patch_->exactBoundingBox ();
                        //refineBoundingBox (height);
                }
                void makeInner(Point const &center,
                               unsigned int depth, BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        patch_ = 0;
                        const auto childBoxes = child_boxen(aabb);
                        children_ = new Node[4];
                        children_[0].create (depth-1, center, childBoxes[0], height);
                        children_[1].create (depth-1, center, childBoxes[1], height);
                        children_[2].create (depth-1, center, childBoxes[2], height);
                        children_[3].create (depth-1, center, childBoxes[3], height);
                        refineBoundingBox();
                }

                // BoundingBox refinement for leaf nodes
                /*void refineBoundingBox (std::function<real(real,real)> const &height)
                {
                        real min_h = std::numeric_limits<real>::max(),
                             max_h = -std::numeric_limits<real>::max();
                        const Point min = aabb_.min(),
                                    max = aabb_.max();
                        for (real z=min.z(); z<=max.z(); z+=0.1) {
                                for (real x=min.x(); x<=max.x(); x+=0.1) {
                                        const real h = height(x,z);
                                        if (h < min_h) min_h = h;
                                        if (h > max_h) max_h = h;
                                }
                        }

                        aabb_ = BoundingBox (Point(min.x(), min_h, min.z()),
                                             Point(max.x(), max_h, max.z()));
                }*/

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
                Patch *patch_;
                BoundingBox aabb_;
        };
}

Quadtree::Quadtree ()
: root_(new detail::Node (2,
                          [](real x,real y) { return -10+5*cos(x*0.1)*cos(y*0.1); }))
{
}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        return (*root_)(ray);
}

} }
