#include "quadtree.h"
#include "quadtree/patch.h"
#include "quadtree/node.h"
#include "color.h"
#include "math3d.h"
#include "materials/lambertmaterial.h"

#include <array>
#include <stack>
#include <functional>
#include <limits>
#include <sstream>

#include <QDebug>




namespace picogen { namespace cracker {

Quadtree::Quadtree (Point cameraPosition,
                    int maxRecursion,
                    real distanceOfMaxDetail, real distanceOfMinDetail
) {
        assert (maxRecursion >= 0);
        assert (distanceOfMinDetail > distanceOfMaxRecursion);

        const auto fun = [](real x,real y) {
                return -25 + 15 * cos(y*0.1) * /**/ cos(x*0.1);
        };

        qDebug() << "sizeof(Node) =" << sizeof(detail::Node);
        root_.reset (new detail::Node (
                             detail::NodeDetail (maxRecursion,
                                                 distanceOfMinDetail,
                                                 distanceOfMaxDetail),
                             fun, cameraPosition, aabb_));
}

Intersection::Optional Quadtree::operator() (Ray const &ray_) const {
        Ray ray (ray_.origin(),
                 normalize<Direction>(
                         ray_.direction().x()==0 ? 0.00001 : ray_.direction().x(),
                         ray_.direction().y()==0 ? 0.00001 : ray_.direction().y(),
                         ray_.direction().z()==0 ? 0.00001 : ray_.direction().z()
                 ));
        Interval::Optional oi = intersect (ray, aabb_);
        if (!oi) return Intersection::Optional();

        real min = oi.interval().min(),
             max = oi.interval().max();
        if (min < 0) min = 0;
        if (min > max) return Intersection::Optional();

        return detail::Node::intersect_iter(&*root_, ray,
                                            min,
                                            max);
}

} }
