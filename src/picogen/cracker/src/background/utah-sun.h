#ifndef UTAH_SUN_H_INCLUDED_20111013
#define UTAH_SUN_H_INCLUDED_20111013

#include "sun.h"
#include "utah-sky/sunsky.hh"

namespace picogen { namespace cracker {

class UtahSun : public Sun {
public:
        UtahSun (std::shared_ptr<picogen::redshift::background::PssSunSky> pssSunSky)
                : pssSunSky_ (pssSunSky), pPssSunSky_(pssSunSky.get()) {}
private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Color radiance_ () const ;
private:
        std::shared_ptr<picogen::redshift::background::PssSunSky> pssSunSky_;
        picogen::redshift::background::PssSunSky
                                             *pPssSunSky_; // < faster dispatch

};

} }
#endif // UTAH_SUN_H_INCLUDED_20111013
