#include "sun.h"
#include "ray.h"

namespace picogen { namespace cracker {

Sun::Sun (const Direction &dir)
        : direction_(dir)
{
}

Ray Sun::deterministicShadowRay(Point const &from) const {
        return Ray(from, -direction_);
}

Color Sun::radiance() const {
        return Color::FromRgb(6,4,2);
}

} }
