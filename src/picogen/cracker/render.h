#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace picogen { namespace cracker {

class Scene;
class RenderTarget;

template <typename SurfaceIntegrator,
          typename Camera>
inline void render(std::shared_ptr<Scene>,
                   SurfaceIntegrator,
                   Camera,
                   std::shared_ptr<RenderTarget>
                  );

} }

#include "render.inc.h"

#endif // RENDERER_H
