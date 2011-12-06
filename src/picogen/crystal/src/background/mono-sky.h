#ifndef MONO_SKY_H_INCLUDED_20111010
#define MONO_SKY_H_INCLUDED_20111010

#include "sky.h"

namespace crystal { namespace background {

class MonoSky : public Sky {
public:
        MonoSky (Radiance const &col) : color_(col) {}
private:
        Radiance radiance_ (Ray const &) const { return color_; }
        Radiance color_;
};

} }

#endif // MONO_SKY_H_INCLUDED_20111010
