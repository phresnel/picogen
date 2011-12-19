#include "alpine.h"

namespace crystal {
std::function<real(real,real)> quatsch_function_2d(std::string const &program);
}

namespace crystal { namespace builtin { namespace terrain_tex {

Radiance alpine (Point const &p)
{
        static auto noise = quatsch_function_2d(
                            "(* 575 ([LibnoisePerlin   "
                            "         frequency{0.01}   "
                            "         octave-count{12} "
                            "         persistence{0.5} "
                            "         ] x y))          ");

        const real f = ilerp_sat(p.y+noise(p.x,p.z),
                                 real(75), real(80));

        return lerp (f,
                     Radiance::FromRgb(0.6,0.6,0.6),
                     Radiance::FromRgb(3,3,3));
}

std::function<Radiance (Point const &p)> create_alpine () {
        return alpine;
}

} } }
