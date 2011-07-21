#include "lambertmaterial.h"

namespace picogen { namespace cracker {

Color::Optional LambertMaterial::brdf_(const InVector &,
                                       const OutVector &,
                                       Random &) const
{
        return Color::FromRgb(0.2, 0.1, 0.2);
}

} }
