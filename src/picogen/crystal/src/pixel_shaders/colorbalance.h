#ifndef COLORBALANCE_H_INCLUDED_20111223
#define COLORBALANCE_H_INCLUDED_20111223
#include "crystal_math.h"
namespace crystal { namespace pixel_shaders {
class ColorBalance {
public:
        ColorBalance (Radiance const & factor) : factor_(factor) {}
        Radiance operator() (Radiance const &rad) const {
                return rad * factor_;
        }
private:
        Radiance factor_;
};
} }
#endif // COLORBALANCE_H_INCLUDED_20111223
