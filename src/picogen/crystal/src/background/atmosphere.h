#ifndef ATMOSPHERE_H_20111209
#define ATMOSPHERE_H_20111209

#include "crystal_math.h"

namespace crystal { class Ray; }

namespace crystal { namespace background {

class AtmosphericEffects {
public:
        AtmosphericEffects (Radiance const &att,
                            Radiance const &ins)
                : attenuation(att), inScatter (ins)
        {}

        AtmosphericEffects() = default;

        Radiance apply (Radiance const &in) const {
                return inScatter + attenuation * in;
        }

private:
        Radiance attenuation;
        Radiance inScatter;

};

class Atmosphere {
public:
        Atmosphere ();
        virtual ~Atmosphere() {}

        AtmosphericEffects atmosphericEffects (const Point &viewer,
                                               const Point &source) const
        {
                return this->atmosphericEffects_ (viewer, source);
        }
private:
        virtual AtmosphericEffects atmosphericEffects_ (const Point &,
                                                        const Point &)
                                                        const = 0;
};

} }

#endif // ATMOSPHERE_H_20111209
