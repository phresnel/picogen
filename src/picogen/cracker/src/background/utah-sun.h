#ifndef UTAH_SUN_H_INCLUDED_20111013
#define UTAH_SUN_H_INCLUDED_20111013

#include "sun.h"
#include "utah-sky/sunsky.hh"

namespace picogen { namespace cracker { namespace {
        Direction redshift_vector_to_direction (
                        picogen::redshift::Vector const &v)
        {
                return normalize<Direction>(v.x, v.y, v.z);
        }
} } }

namespace picogen { namespace cracker {

class UtahSun : public Sun {
public:
        UtahSun (std::shared_ptr<picogen::redshift::background::PssSunSky> pssSunSky)
                : pssSunSky_ (pssSunSky), pPssSunSky_(pssSunSky.get())
                , sunDirection_ (redshift_vector_to_direction (pPssSunSky_->
                                                            GetSunPosition()))
        {
                const auto rgb = pPssSunSky_->GetSunSpectralRadiance ().toRGB();
                sunRadiance_ = Color::FromRgb (rgb.R, rgb.G, rgb.B);
        }

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Color radiance_ () const ;
private:
        std::shared_ptr<picogen::redshift::background::PssSunSky> pssSunSky_;
        picogen::redshift::background::PssSunSky
                                             *pPssSunSky_; // < faster dispatch

        Color sunRadiance_;
        Direction sunDirection_;

};

} }
#endif // UTAH_SUN_H_INCLUDED_20111013
