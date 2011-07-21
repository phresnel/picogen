#include "lambertmaterial.h"

namespace picogen { namespace cracker {

Color::Optional LambertMaterial::brdf_(const InDirection &,
                                       const OutDirection &,
                                       Random &) const
{
        return Color::FromRgb(0.9, 0.1, 0.2);
}

} }
