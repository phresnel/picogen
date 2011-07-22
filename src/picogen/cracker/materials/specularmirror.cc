#include "specularmirror.h"

namespace picogen { namespace cracker {

Color::Optional SpecularMirrorMaterial::brdf_(const InDirection &,
                                              const OutDirection &,
                                              Random &) const
{
        return Color::FromRgb(0.5, 0.5, 0.5);
}

} }
