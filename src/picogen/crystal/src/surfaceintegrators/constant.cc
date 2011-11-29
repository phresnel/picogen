#include "constant.h"

namespace crystal { namespace surfaceintegrators {

Constant::Constant()
{
}

Radiance Constant::get (Ray const &) const {
        return Radiance::FromRgb (1, 0.5, 0.25);
}

} }
