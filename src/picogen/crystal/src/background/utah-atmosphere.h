#ifndef UTAH_ATMOSPHERE_H_20111209
#define UTAH_ATMOSPHERE_H_20111209

#include "atmosphere.h"
#include "utah-sky/sunsky.hh"

namespace crystal { namespace background {

class UtahAtmosphere : public Atmosphere {
public:
        UtahAtmosphere (std::shared_ptr<const crystal::redshift::background::PssSunSky>);
private:
        AtmosphericEffects atmosphericEffects_ (const Point &viewer,
                                                const Point &source) const;


private:
        std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky_;
};

} }

#endif // UTAH_ATMOSPHERE_H_20111209
