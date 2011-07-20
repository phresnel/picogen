#include "lambertmaterial.h"

namespace picogen { namespace cracker {

Color::Optional LambertMaterial::brdf_(const InVector &in,
                                       const OutVector &out,
                                       Random &rand) const
{
        return Color::Optional();
}

} }
