#ifndef NOATMOSPHERE_H_20111213
#define NOATMOSPHERE_H_20111213

#include "atmosphere.h"

namespace crystal { class Ray; }

namespace crystal { namespace background {

class NoAtmosphere : public Atmosphere {
private:
        AtmosphericEffects atmosphericEffects_ (const Point &, const Point &)
        const {
                return AtmosphericEffects();
        }
};

} }

#endif // NOATMOSPHERE_H_20111213
