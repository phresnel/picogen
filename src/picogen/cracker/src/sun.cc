#include "sun.h"
#include "ray.h"

#include "utah-sky/sunsky.hh"

namespace picogen { namespace cracker {

Sun::Sun (const Direction &dir)
        : direction_(dir)
{
}

Ray Sun::deterministicShadowRay(Point const &from) const {
        return Ray(from, -direction_);
}

Color Sun::radiance() const {
        static picogen::redshift::background::PssSunSky sunSky(
                                picogen::redshift::Vector(1,1,1),
                                3, 0, false);
        //const auto rad = sunSky.GetSkySpectralRadiance
        return Color::FromRgb(3,2,1);
}

} }
