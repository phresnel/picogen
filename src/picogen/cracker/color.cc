#include "color.h"

namespace picogen { namespace cracker {

Color Color::FromRgb(real r, real g, real b)
{
        return Color (r,g,b);
}

Color Color::Black()
{
        return Color (0,0,0);
}

} }
