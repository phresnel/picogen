#ifndef GAMMARADIANCE_H_INCLUDED_20111223
#define GAMMARADIANCE_H_INCLUDED_20111223
#include "crystal_math.h"
namespace crystal { namespace pixel_shaders {
class GammaRadiance {
public:
        GammaRadiance (Radiance g) :
                g(g),
                ig(real(1)/g)
        {}

        Radiance operator() (Radiance const &rad) const {
                return pow (rad, ig);
        }

private:
        Radiance g, ig;
};
} }
#endif
