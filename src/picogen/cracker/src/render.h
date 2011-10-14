#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace picogen { namespace cracker {

class Scene;
class RenderTarget;
class Random;
class Color;
class Ray;

class VolumeIntegrator;

class RendererBase {
public:
        RendererBase (std::shared_ptr<Scene>);
        virtual ~RendererBase() {}
        Color transmittance (Ray const &, Random &) const ;
protected:
        std::shared_ptr<VolumeIntegrator> volumeIntegrator_;
        std::shared_ptr<Scene> scene_;
};

template <typename SurfaceIntegrator, typename Camera>
class Renderer : public RendererBase {
public:
        Renderer (std::shared_ptr<Scene> scene,
                  SurfaceIntegrator integrator,
                  Camera camera)
        : RendererBase(scene), integrator_(integrator), camera_(camera)
        {}

        inline void render (std::shared_ptr<RenderTarget>) const;

private:
        SurfaceIntegrator integrator_;
        Camera camera_;
};

template <typename SurfaceIntegrator, typename Camera>
Renderer<SurfaceIntegrator, Camera>
createRenderer (std::shared_ptr<Scene> scene,
                SurfaceIntegrator integrator,
                Camera camera)
{
        return Renderer<SurfaceIntegrator,Camera>(scene, integrator, camera);
}

} }

#include "render.inc.h"

#endif // RENDERER_H
