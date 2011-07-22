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

} }
