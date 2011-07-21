#include "render.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"
#include "ray.h"
#include "random.h"

namespace picogen  { namespace cracker {


template <typename SurfaceIntegrator,
          typename Camera>
void render (std::shared_ptr<Scene> scene_,
             SurfaceIntegrator surfaceIntegrator,
             Camera camera,
             std::shared_ptr<RenderTarget> target
             )
{
        Random random;
        RenderTarget &t = *target;
        Scene &scene = *scene_;
        const unsigned int width = t.width(),
                           height = t.height();

        for (unsigned int y=0; y<height; ++y) {
                const real v = 1 - y/static_cast<real>(height-1);
                RenderTargetRow row = t.row(y);

                #pragma omp parallel for schedule(dynamic)
                for (unsigned int x=0; x<width; ++x) {
                        const real u = x/static_cast<real>(width);

                        const Ray primary = camera (u, v);
                        const Color color = surfaceIntegrator(primary, scene,
                                                              random);

                        row[x].setColor (color);
                }
        }
}

} }
