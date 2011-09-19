#include "sky.h"
#include "ray.h"

//#include "


namespace picogen { namespace cracker {

Sky::Sky ()
{
}

Color Sky::radiance(Ray const &) const {
        return Color::FromRgb(0.9, 0.9, 1.2);
}

} }
