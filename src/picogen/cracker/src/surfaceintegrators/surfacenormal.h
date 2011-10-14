#ifndef SURFACENORALINTEGRATOR_H_20110719
#define SURFACENORALINTEGRATOR_H_20110719

#include "ray.h"
#include "scene.h"
#include "real.h"
#include "color.h"

#include<cassert>

namespace picogen { namespace cracker {

class Random;
class RendererBase;

class SurfaceNormalIntegrator {
public:
        SurfaceNormalIntegrator () : min_(0), range_(100) {}

        SurfaceNormalIntegrator (real min, real max)
                : min_(min), range_(max-min)
        {
                assert(min<max);
        }

        Color operator() (Ray const &,
                          Intersection const &i,
                          Scene const &,
                          RendererBase const &,
                          Random &) const
        {
                const Normal &n = i.differentialGeometry().geometricNormal();
                const real
                      r = n.x()+0.5,
                      g = n.y()+0.5,
                      b = n.z()+0.5;
                return Color::FromRgb (r, g, b);
        }

private:
        real min_, range_;
};

} }

#endif // SURFACENORALINTEGRATOR_H_20110719
