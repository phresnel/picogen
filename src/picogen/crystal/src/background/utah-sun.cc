#include "utah-sun.h"
#include "background/utah-sky/sunsky.hh"



namespace crystal { namespace background { namespace {
        Direction redshift_vector_to_direction (
                        crystal::redshift::Vector const &v)
        {
                return normalize<Direction>(v.x, v.y, v.z);
        }
} } }



namespace crystal { namespace background {


Radiance UtahSun::GetSunRadiance (crystal::redshift::background::PssSunSky const &ss)
{
        const auto rgb = ss.GetSunSpectralRadiance().toRGB();
        return Radiance::FromRgb (rgb.R, rgb.G, rgb.B);
}



UtahSun::UtahSun (std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky)
: pssSunSky_ (pssSunSky),
  pPssSunSky_(pssSunSky.get()),
  sunDirection_ (redshift_vector_to_direction (pPssSunSky_->
                                            GetSunPosition())),
  sunRadiance_ (GetSunRadiance (*pssSunSky))
{
}

Ray UtahSun::deterministicShadowRay_(const Point &from) const {
        return Ray (from, sunDirection_);
}

Radiance UtahSun::radiance_() const {
        return sunRadiance_;
}

} }

