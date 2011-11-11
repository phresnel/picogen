#include "render.h"
#include "color.h"
#include "ray.h"
#include "random.h"
#include "volumeintegrators/volumeintegrator.h"

int globalTravCounter;

namespace picogen { namespace cracker {

RendererBase::RendererBase (std::shared_ptr<Scene> scene)
        : volumeIntegrator_(new VolumeIntegrator())
        , scene_(scene)
{
}

Color RendererBase::transmittance (Ray const &ray, Random &random) const {
        // PBRT:
        //  return volumeIntegrator->Transmittance(scene, this, ray, sample,
        //                                         rng, arena);
        return volumeIntegrator_->transmittance (*scene_,
                                                 *this,
                                                 ray,
                                                 random);
        //return Color::FromRgb (1,1,1);
}

} }
