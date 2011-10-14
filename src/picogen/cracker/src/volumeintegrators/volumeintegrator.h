#ifndef VOLUMEINTEGRATOR_H_INCLUDED_20111014
#define VOLUMEINTEGRATOR_H_INCLUDED_20111014

#include "interval.h"
#include "color.h"

namespace picogen { namespace cracker {

class Color;
class Scene;
class RendererBase;
class Ray;
class Random;
class Interval;

class VolumeRegion {
public:
        Interval::Optional intersect (Ray const &ray) const {
                return intersect_ (ray);
        }
private:
        virtual Interval::Optional intersect_ (Ray const &ray) const;
};

class VolumeIntegrator {
public:
        virtual ~VolumeIntegrator () {
        }

        Color transmittance (Scene const &scene,
                             RendererBase const &renderer,
                             Ray const &ray,
                             Random &random) const
        {
                return this->transmittance_ (scene,
                                             renderer,
                                             ray,
                                             random);
        }

private:
        virtual Color transmittance_ (Scene const &,
                                      RendererBase const &,
                                      Ray const &,
                                      Random const &) const;
        VolumeRegion reg;
};

} }

#endif // VOLUMEINTEGRATOR_H_INCLUDED_20111014
