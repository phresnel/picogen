#include "utah-sky.h"
#include "utah-sky/sunsky.hh"
#include "crystal_math.h"

namespace crystal { namespace background {

UtahSky::UtahSky (std::shared_ptr<crystal::redshift::background::PssSunSky> pssSunSky)
        : pssSunSky_(pssSunSky)
{
}

Radiance UtahSky::radiance_(Ray const &ray) const {
        /*static picogen::redshift::background::PssSunSky sunSky(
                                picogen::redshift::Vector(1,0.2,1),
                                3, 0, false);*/
        const Direction dir (ray.direction.x(),
                             std::max(real(0), ray.direction.y()),
                             ray.direction.z());
        const auto rad = pssSunSky_->GetSkySpectralRadiance (crystal::redshift::Vector
                                                        (dir.x(),
                                                         dir.y(),
                                                         dir.z()));
        const auto rgb = rad.toRGB();

        if (ray.direction.y() < 0) {
                // This hack has no physical motivation.
                const real f = std::fabs (ray.direction.y());
                return (1-f)*Radiance::FromRgb(rgb.R, rgb.G, rgb.B);
        }
        return Radiance::FromRgb(rgb.R, rgb.G, rgb.B);
        //return Radiance::FromRgb(0.9, 0.9, 1.2);
}

} }
