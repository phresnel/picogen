#ifndef UTAH_SUN_H_INCLUDED_20111013
#define UTAH_SUN_H_INCLUDED_20111013

#include "sun.h"
#include "utah-sky/sunsky.hh"

namespace crystal { namespace background { namespace {
        Direction redshift_vector_to_direction (
                        crystal::redshift::Vector const &v)
        {
                return normalize<Direction>(v.x, v.y, v.z);
        }
} } }

namespace crystal { namespace background {

class UtahSun : public Sun {
public:
        UtahSun (std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky)
                : pssSunSky_ (pssSunSky),
                  pPssSunSky_(pssSunSky.get()),
                  sunDirection_ (redshift_vector_to_direction (pPssSunSky_->
                                                            GetSunPosition())),
                  sunRadiance_ (GetSunRadiance (*pssSunSky))
        {
        }

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Radiance radiance_ () const ;

        static Radiance GetSunRadiance (
                        crystal::redshift::background::PssSunSky const &ss)
        {
                const auto rgb = ss.GetSunSpectralRadiance().toRGB();
                return Radiance::FromRgb (rgb.R, rgb.G, rgb.B);
        }
private:
        std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky_;
        crystal::redshift::background::PssSunSky const *pPssSunSky_; // < faster dispatch

        Direction sunDirection_;
        Radiance sunRadiance_;
};

} }
#endif // UTAH_SUN_H_INCLUDED_20111013
