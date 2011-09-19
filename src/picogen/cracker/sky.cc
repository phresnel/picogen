#include "sky.h"
#include "ray.h"

namespace picogen { namespace cracker {

Sky::Sky ()
{
}

Color Sky::radiance(Ray const &) const {
        return Color::FromRgb(0.9, 0.9, 1.2);
}

} }
