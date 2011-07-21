#ifndef SPECULARMIRROR_H_20110721
#define SPECULARMIRROR_H_20110721

#include "material.h"

namespace picogen { namespace cracker {

class SpecularMirrorMaterial : public Material {
public:
        SpecularMirrorMaterial() : Material(true) {}
private:
        Color::Optional brdf_(const InDirection &in,
                              const OutDirection &out,
                              Random &rand) const;
};

} }

#endif // SPECULARMIRROR_H_20110721
