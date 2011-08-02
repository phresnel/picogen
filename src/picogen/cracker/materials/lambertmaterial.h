#ifndef LAMBERT_H_20110720
#define LAMBERT_H_20110720

#include "material.h"
#include "constants.h"
#include "color.h"

namespace picogen { namespace cracker {

class LambertMaterial : public Material {
public:
        LambertMaterial () : color_(Color::White() / constants::pi)
        {}

        LambertMaterial (Color color) // TODO: this needs to be reworked in the base-class so we also take textures etc.
                : color_(color / constants::pi)
        {
        }
private:
        Color::Optional brdf_(const InDirection &in,
                              const OutDirection &out,
                              Random &rand) const;

        BsdfSample sample_ (OutDirection const &,
                            Random &rand) const;

        real pdf_ (const InDirection &, const OutDirection &) const;
private:
        Color color_;
};

} }

#endif // LAMBERT_H_20110720
