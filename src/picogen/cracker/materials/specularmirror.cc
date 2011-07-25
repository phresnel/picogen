#include "specularmirror.h"

namespace picogen { namespace cracker {

Color::Optional SpecularMirrorMaterial::brdf_(const InDirection &,
                                              const OutDirection &,
                                              Random &) const
{
        return Color::White();
}


BsdfSample SpecularMirrorMaterial::sample_ (OutDirection const &,
                                            Random &rand) const
{
        return BsdfSample::Null();
}

} }
