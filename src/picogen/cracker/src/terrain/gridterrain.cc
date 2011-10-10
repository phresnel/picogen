#include "gridterrain.h"

#include "intersection.h"
#include "ray.h"
#include "math3d.h"
#include "algorithm.h"
#include "materials/lambertmaterial.h"
#include "materials/specularmirror.h"

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

                        const Point p((u-0.5)*heightfieldWidth_,
                                      -8 + 4*std::sin(u*24) * std::sin(v*24),
                                      (v-0.5)*heightfieldDepth_);
                        aabb_ = merge (aabb_, p);
                        heightfield_[y*heightfieldWidth_+x] =p.y();
                }
        }
}

Intersection::Optional GridTerrain::operator() (Ray const &ray) const {

        const real &W = size_.x(),
                   &D = size_.z();
                   //&H = size_.y();

        const Interval::Optional ointerval = intersect (ray, aabb_);
        if (!ointerval)
                return Intersection::Optional();
        const Interval interval (ointerval.interval().min(),
                                 ointerval.interval().max());

        const Direction& direction = ray.direction();
        real step ;

        const real seps = 1;
        if(direction.x()>direction.y()) {
                step = seps * (1/sqrt(2)) * direction.x() * pixelWidth_;
        } else {
                step = seps * (1/sqrt(2)) * direction.z() * pixelDepth_;
        }
        if (step<seps) step=seps;

        const Vector dstep = direction * step;
        Point dcurr = ray(step+interval.min()); // <-- we need to push back ...
                // ... once we intersect, so initialize to 'step' instead of '0'

        const real u = 0.5 + dcurr.x() / W,
                   v = 0.5 + dcurr.z() / D;
        const bool above = dcurr.y() > height(u*heightfieldWidth_,
                                              v*heightfieldDepth_);

        for (real f=step; f<interval.max(); f+=step, dcurr+=dstep) {
                const real u = 0.5 + dcurr.x() / W,
                           v = 0.5 + dcurr.z() / D;
                const real ch = height(u*heightfieldWidth_,
                                       v*heightfieldDepth_);
                if ((dcurr.y() > ch) != above) {
                        const Normal &normal = normal_above(ch,
                                                            dcurr.x(),
                                                            dcurr.z());
                        const std::tuple<Normal,Normal,Normal>
                                 cs = coordinateSystem(normal);
                        const DifferentialGeometry dg(
                                normal, normal,
                                std::get<0>(cs),
                                std::get<2>(cs)
                        );
                        return Intersection (f-step,
                                             std::shared_ptr<Material>(
                                                     new LambertMaterial),
                                             dg
                                             );
                }
        }
        return Intersection::Optional();
}

Normal GridTerrain::normal_above (real centerH, real x, real z) const {
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
