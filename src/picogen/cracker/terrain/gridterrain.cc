#include "gridterrain.h"

#include "potentialintersection.h"
#include "ray.h"
#include "math3d.h"

#include <algorithm>
#include <cmath>

#include <QDebug>

namespace picogen { namespace cracker {

GridTerrain::GridTerrain()
: size_(200, 30, 200)
, heightfieldWidth_(128), heightfieldDepth_(heightfieldWidth_)
, heightfield_(heightfieldWidth_*heightfieldDepth_)

{
        for (unsigned int y=0; y<heightfieldDepth_; ++y) {
                for (unsigned int x=0; x<heightfieldWidth_; ++x) {
                        const real u = x / static_cast<real>(heightfieldWidth_);
                        const real v = y / static_cast<real>(heightfieldDepth_);

                        heightfield_[y*heightfieldWidth_+x] =
                                        -20 + 16*std::sin(u*16) * std::sin(v*16);
                }
        }
}

PotentialIntersection GridTerrain::operator() (Ray const &ray) const {

        const real &W = size_.x(),
                   &D = size_.z(),
                   &H = size_.y();

        const real step = 1;

        for (real f=0; f<300; f+=step) {
                const Point c = ray(f);
                const real u = (c.x()+W/2) / W,
                           v = (c.z()+D/2) / D;
                const int x = u*heightfieldWidth_,
                          y = v*heightfieldDepth_;

                if ( (x<0) | (static_cast<decltype(heightfieldWidth_)>(x)>=heightfieldWidth_)
                   | (y<0) | (static_cast<decltype(heightfieldDepth_)>(y)>=heightfieldDepth_))
                        continue;
                const real ch = heightfield_[y*heightfieldWidth_
                                            +x];
                if (c.y() < ch) {
                        return Intersection (f, normal(c.x(), c.z()));
                }
        }
        return PotentialIntersection();
}

Normal GridTerrain::normal (real u, real v) const {
        return Normal (0,1,0);
}

} }
