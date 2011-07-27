#ifndef LAMBERT_H_20110720
#define LAMBERT_H_20110720

#include "material.h"

namespace picogen { namespace cracker {

class LambertMaterial : public Material {
private:
        Color::Optional brdf_(const InDirection &in,
                              const OutDirection &out,
                              Random &rand) const;

        BsdfSample sample_ (OutDirection const &,
                            Random &rand) const;

        real pdf_ (const Direction &out, const Direction &in) const;
};

} }

#endif // LAMBERT_H_20110720
