#ifndef MONO_SKY_H_INCLUDED_20111010
#define MONO_SKY_H_INCLUDED_20111010

#include "sky.h"

namespace picogen { namespace cracker {

class MonoSky : public Sky {
public:
        MonoSky (Color const &col) : color_(col) {}
private:
        Color radiance_ (Ray const &) const { return color_; }
        Color color_;
};

} }

#endif // MONO_SKY_H_INCLUDED_20111010
