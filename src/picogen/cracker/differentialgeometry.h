#ifndef DIFFERENTIALGEOMETRY_H_20110726
#define DIFFERENTIALGEOMETRY_H_20110726

#include "math3d.h"

namespace picogen { namespace cracker {
class DifferentialGeometry {
public:
        DifferentialGeometry() = delete;

        DifferentialGeometry (Normal const & geometricNormal,
                              Normal const & shadingNormal,
                              Normal const & dndu,
                              Normal const & dndv)
                : geometricNormal_(geometricNormal)
                , shadingNormal_ (shadingNormal)
                , dpdu_ (dndu), dpdv_(dndv)
        {}

        Normal geometricNormal() const { return geometricNormal_; }
        Normal shadingNormal() const { return shadingNormal_; }

        // "right" in the local geometry of the intersection.
        Normal dpdu() const { return dpdu_; }

        // "back" in the local geometry of the intersection.
        Normal dpdv() const { return dpdv_; }

private:
        Normal geometricNormal_, shadingNormal_;
        Normal dpdu_, dpdv_;
};
} }

#endif // DIFFERENTIALGEOMETRY_H_20110726
