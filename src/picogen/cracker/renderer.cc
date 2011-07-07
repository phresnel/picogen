#include "renderer.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"

namespace picogen  { namespace cracker {


void render (std::shared_ptr<Scene> scene,
             std::shared_ptr<RenderTarget> target)
{
        RenderTarget &t = *target;
        const unsigned int width = t.width(),
                           height = t.height();

        for (unsigned int y=0; y<height; ++y) {
                const real v = 1 - y/static_cast<real>(height-1);
                RenderTargetRow row = t.row(y);
                for (unsigned int x=0; x<width; ++x) {
                        const real u = x/static_cast<real>(width);
                        row[x].setColor (Color::FromRgb(u,v,u*v));
                }
        }
}

} }
