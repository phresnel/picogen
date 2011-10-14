#include "render.h"
#include "color.h"
#include "ray.h"
#include "random.h"

namespace picogen { namespace cracker {

Color RendererBase::transmittance (Ray const &, Random &) const {
        // PBRT:
        //  return volumeIntegrator->Transmittance(scene, this, ray, sample,
        //                                         rng, arena);

        return Color::FromRgb (1,1,1);
}

} }
