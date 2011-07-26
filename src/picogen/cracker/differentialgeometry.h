#ifndef DIFFERENTIALGEOMETRY_H_20110726
#define DIFFERENTIALGEOMETRY_H_20110726

#include "math3d.h"

namespace picogen { namespace cracker {
class DifferentialGeometry {
public:
        DifferentialGeometry() = delete;

        DifferentialGeometry (Normal const & geometricNormal,
                              Normal const & shadingNormal,
                              Normal const & dpdu,
                              Normal const & dpdv)
                : geometricNormal_(geometricNormal)
                , shadingNormal_ (shadingNormal)
                , dpdu_ (dpdu), dpdv_(dpdv)
                //, s (dpdu())
                , t (cross (shadingNormal, dpdu_))

        {}

        Normal geometricNormal() const { return geometricNormal_; }
        Normal shadingNormal() const { return shadingNormal_; }

        // "right" in the local geometry of the intersection.
        Normal dpdu() const { return dpdu_; }

        // "back" in the local geometry of the intersection.
        Normal dpdv() const { return dpdv_; }

        Normal worldToLocal (Normal const &v) const {
                return normalize<Normal> (
                        dot (v, s()),
                        dot (v, shadingNormal_),
                        dot (v, t)
                );
        }

        Normal localToWorld (Normal const &v) const {
                return normalize<Normal> (
                        s().x()*v.x() + shadingNormal_.x()*v.y() + t.x()*v.z(),
                        s().y()*v.x() + shadingNormal_.y()*v.y() + t.y()*v.z(),
                        s().z()*v.x() + shadingNormal_.z()*v.y() + t.z()*v.z()
                );
        }

private:
        Normal geometricNormal_, shadingNormal_;
        Normal dpdu_, dpdv_;

        const Normal& s() const {
                return dpdu_;
        }
        Normal t;
};
} }

#endif // DIFFERENTIALGEOMETRY_H_20110726
