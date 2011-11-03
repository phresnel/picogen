#include "volumeintegrator.h"
#include "color.h"
#include "scene.h"
#include "render.h"
#include "ray.h"
#include "random.h"

namespace picogen { namespace cracker {

Interval::Optional VolumeRegion::intersect_ (const Ray &ray) const {
        return Interval::Optional();
}

Color VolumeIntegrator::transmittance_ (const Scene    &scene,
                                        const RendererBase &rendererBase,
                                        const Ray      &ray,
                                        const Random   &random
                                       ) const
{
        return Color::FromRgb(1,1,1);
}

} }

