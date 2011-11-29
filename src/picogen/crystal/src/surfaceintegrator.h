#ifndef SURFACEINTEGRATOR_H_INCLUDED_20111129
#define SURFACEINTEGRATOR_H_INCLUDED_20111129

#include "crystal_math.h"

namespace crystal {

        class SurfaceIntegrator {
        public:
                virtual ~SurfaceIntegrator() {}
                Radiance operator() (Ray const &ray) const {
                        return get (ray);
                }
        private:
                virtual Radiance get(Ray const &) const = 0;
        };

}

#endif // SURFACEINTEGRATOR_H_INCLUDED_20111129
