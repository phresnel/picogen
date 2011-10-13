#include "utah-sun.h"


namespace picogen { namespace cracker {

Ray UtahSun::deterministicShadowRay_(const Point &from) const {
        const auto pos = pPssSunSky_->GetSunPosition();
        const auto dir = normalize<Direction>(pos.x, pos.y, pos.z);
        return Ray (from, dir);
}

Color UtahSun::radiance_() const {
        const auto rgb = pPssSunSky_->GetSunSpectralRadiance ().toRGB();
        return Color::FromRgb (rgb.R, rgb.G, rgb.B);
}

} }

