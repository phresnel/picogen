#include "gridterrain.h"

#include "potentialintersection.h"
#include "ray.h"
#include "math3d.h"

#include <algorithm>
#include <cmath>

#include <QDebug>

namespace picogen { namespace cracker {

GridTerrain::GridTerrain()
: heightfieldWidth_(128), heightfieldDepth_(heightfieldWidth_)
, heightfield_(heightfieldWidth_*heightfieldDepth_)
{
        for (unsigned int y=0; y<heightfieldDepth_; ++y) {
                for (unsigned int x=0; x<heightfieldWidth_; ++x) {
                        const real u = x / static_cast<real>(heightfieldWidth_);
                        const real v = y / static_cast<real>(heightfieldDepth_);

                        heightfield_[y*heightfieldWidth_+x] = -10;
                                        16*std::sin(u*3) * std::cos(v*3);
                }
        }
}

PotentialIntersection GridTerrain::operator() (Ray const &ray) const {

        const real W = 100.,
                   D = W,
                   H = 10.;

        const real step = 3;

        for (real f=0; f<1000; f+=step) {
                const Point c = ray(f);
                const real u = c.x() / W + W/2,
                           v = c.z() / D + D/2;
                const int x = u*heightfieldWidth_,
                          y = v*heightfieldDepth_;
                //qDebug() << f << ":" << c.x() << ":" << c.y();
                //break;
                /*
                if ((x<0)
                   | (static_cast<decltype(heightfieldWidth_)>(x)>=heightfieldWidth_)
                   | (y<0)
                   | (static_cast<decltype(heightfieldDepth_)>(y)>=heightfieldDepth_))
                        continue;
                const real ch = heightfield_[y*heightfieldWidth_
                                            +x];
                */
                if (c.y() < -20) { //ch) {
                        return Intersection (f);
                }
        }
        /*
        using std::pow; using std::sqrt;
        const real hSize = sqrt(pow(heightfieldWidth_,2)
                              + pow(heightfieldDepth_,2));

        const Vector step = ray.direction() / hSize;
        Point current = ray(0);
        for (unsigned int s=0; s<100; ++s, current+=step) {
                const unsigned int x = current
                const real h =
                if (current.y() <
        }*/

        return PotentialIntersection();
        //Intersection(std::max((real)0, ray.direction().y()*20));
}


} }
