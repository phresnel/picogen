#ifndef UTAH_SUN_H_INCLUDED_20111013
#define UTAH_SUN_H_INCLUDED_20111013

#include "sun.h"

namespace crystal { namespace redshift { namespace background {
        class PssSunSky;
} } }


namespace crystal { namespace background {

class UtahSun : public Sun {
public:
        UtahSun (std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky);

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Radiance radiance_ () const ;

        static Radiance GetSunRadiance (crystal::redshift::background::PssSunSky const &ss);
private:
        std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky_;
        crystal::redshift::background::PssSunSky const *pPssSunSky_; // < faster dispatch

        Direction sunDirection_;
        Radiance sunRadiance_;
};

} }
#endif // UTAH_SUN_H_INCLUDED_20111013
