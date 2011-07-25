#include "lambertmaterial.h"

namespace picogen { namespace cracker {

Color::Optional LambertMaterial::brdf_(const InDirection &,
                                       const OutDirection &,
                                       Random &) const
{
        return Color::Gray(0.8);
}

} }
