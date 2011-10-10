#include "render.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"
#include "ray.h"
#include "random.h"
#include "kallisto/rng/kiss.hh"

#include <QDebug>

namespace picogen  { namespace cracker {

namespace {
        inline Random createRandom(
                uint32_t a, uint32_t b, uint32_t c, uint32_t d
        ) {
                using namespace kallisto::random::marsaglia;
                MWC A(a,b);          // a, b
                MWC B(c,d);          // c, d
                for (uint32_t warm=0; warm<4; ++warm) {  // warm up
                        A(); B();
                }
                MWC mwc = MWC(A(), B()); // A+B
                mwc.skip(16);      // warm up

                // IMPORTANT: using variables instead of passing
                // as between the ( and the ) of a call, there is no sequence point
                // (and thus it'd be not portable)
                const uint32_t e=mwc(), f=mwc(), g=mwc(), h=mwc();
                Random ret (e, f, g, h);
                ret.skip(16);
                return ret;
        }
}

template <typename SurfaceIntegrator,
          typename Camera>
inline void Renderer<SurfaceIntegrator, Camera>::render (
        std::shared_ptr<RenderTarget> target
) const
{
        RenderTarget &t = *target;
        Scene &scene = *scene_;
        const unsigned int width = t.width(),
                           height = t.height();

        for (unsigned int y=0; y<height; ++y) {
                const real v = 1 - y/static_cast<real>(height-1);
                RenderTargetRow row = t.row(y);

                #pragma omp parallel for schedule(dynamic)
                for (unsigned int x=0; x<width; ++x) {
                        Random random = createRandom(x,y,row[x].numSamples(),0);

                        const real u = x/static_cast<real>(width);

                        const Ray primary = camera_ (u, v);
                        const Color color = integrator_(primary, scene, random);

                        row[x].add (color);
                }
        }
}

} }
