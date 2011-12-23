#ifndef SCALE_H_INCLUDED_20111223
#define SCALE_H_INCLUDED_20111223
#include "crystal_math.h"
namespace crystal { namespace pixel_shaders {
class Scale {
public:
        Scale() : factor_ (1) {}
        Scale (real factor) : factor_ (factor) {}

        Radiance operator() (Radiance const &rad) const {
                return rad * factor_;
        }
private:
        real factor_;
};
} }
#endif
