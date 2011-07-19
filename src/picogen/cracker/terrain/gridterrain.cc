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
, pixelWidth_(size_.x() / heightfieldWidth_)
, pixelDepth_(size_.z() / heightfieldDepth_)
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
                const real ch = height(u*heightfieldWidth_,
                                       v*heightfieldDepth_);
                if (c.y() < ch) {
                        return Intersection (f, normal(ch, c.x(), c.z()));
                }
        }
        return PotentialIntersection();
}

Normal GridTerrain::normal (real centerH, real x, real z) const {
        const real u = (x+size_.x()/2) / size_.x(),
                   v = (z+size_.z()/2) / size_.z();
        const int ix = u*heightfieldWidth_,
                  iy = v*heightfieldDepth_;

        const real rightDH = height(ix+1,iy) - centerH;
        const real bottomDH = height(ix,iy+1) - centerH;

        const Vector right (pixelWidth_, rightDH, 0);
        const Vector bottom (0, bottomDH, pixelDepth_);


        return normalize<Normal>(cross(bottom, right));
}

real GridTerrain::height(int x, int y) const {
        while (x<0)                       x+=heightfieldWidth_;
        while (x>=(int)heightfieldWidth_) x-=heightfieldWidth_;
        while (y<0)                       y+=heightfieldDepth_;
        while (y>=(int)heightfieldDepth_) y-=heightfieldDepth_;

        return heightfield_[y*heightfieldWidth_+x];
}

} }
