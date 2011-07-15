#include "gridterrain.h"

#include "potentialintersection.h"
#include "ray.h"

#include <algorithm>
#include <cmath>

namespace picogen { namespace cracker {

GridTerrain::GridTerrain()
: heightfieldWidth_(128), heightfieldDepth_(heightfieldWidth_)
, heightfield_(heightfieldWidth_*heightfieldDepth_)
{
        for (int y=0; y<heightfieldDepth_; ++y) {
                for (int x=0; x<heightfieldWidth_; ++x) {
                        const real u = x / static_cast<real>(heightfieldWidth_);
                        const real v = y / static_cast<real>(heightfieldDepth_);

                        heightfield_[y*heightfieldWidth_+x] =
                                        16*std::sin(u*3) * std::cos(v*3);
                }
        }
}

PotentialIntersection GridTerrain::operator() (Ray const &ray) const {
        const Vector step = static_cast<Vector>(ray.direction());

        //return Intersection(std::max((real)0, ray.direction().y()*20));
}


} }
