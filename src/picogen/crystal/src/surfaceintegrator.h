#ifndef SURFACEINTEGRATOR_H_INCLUDED_20111129
#define SURFACEINTEGRATOR_H_INCLUDED_20111129

#include "crystal_math.h"

namespace crystal {

        class Scene;

        class SurfaceIntegrator {
        public:
                virtual ~SurfaceIntegrator() {}
                Radiance operator() (Ray const &ray, Scene const &scene) const {
                        return get (ray, scene);
                }
        private:
                virtual Radiance get(Ray const &, Scene const &) const = 0;
        };

}

#endif // SURFACEINTEGRATOR_H_INCLUDED_20111129
