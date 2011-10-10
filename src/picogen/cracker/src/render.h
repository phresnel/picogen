#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace picogen { namespace cracker {

class Scene;
class RenderTarget;

template <typename SurfaceIntegrator, typename Camera>
class Renderer {
public:
        Renderer (std::shared_ptr<Scene> scene,
                  SurfaceIntegrator integrator,
                  Camera camera)
        : scene_(scene), integrator_(integrator), camera_(camera)
        {}

        inline void render (std::shared_ptr<RenderTarget>) const;

private:
        std::shared_ptr<Scene> scene_;
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
