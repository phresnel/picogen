#ifndef GAMMA_H_INCLUDED_20111223
#define GAMMA_H_INCLUDED_20111223
#include "crystal_math.h"
namespace crystal { namespace pixel_shaders {
class Gamma {
public:
        Gamma (real g) :
                g(g),
                ig(g!=0 ? real(1)/g : 0)
        {}

        Radiance operator() (Radiance const &rad) const {
                return pow (rad, ig);
        }
private:
        real g, ig;
};
} }
#endif
