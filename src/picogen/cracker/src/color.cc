#include "color.h"

namespace picogen { namespace cracker {

Color Color::FromRgb(real r, real g, real b)
{
        return Color (r,g,b);
}

Color Color::Black()
{
        return Gray(0);
}

Color Color::White()
{
        return Gray (1);
}

Color Color::Gray(real f)
{
        assert (f>=0 && f<= 1);
        return Color (f,f,f);
}

} }
