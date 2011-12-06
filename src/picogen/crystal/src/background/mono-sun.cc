#include "mono-sun.h"

namespace crystal { namespace background {

MonoSun::MonoSun (const Radiance &color,
                  const Direction &dir)
: color_ (color), direction_(dir)
{
}

Ray MonoSun::deterministicShadowRay_(Point const &from) const {
        return Ray(from, -direction_);
}

Radiance MonoSun::radiance_() const {
        return color_;
}

} }

