#include "lambertmaterial.h"
#include "sampling.h"
#include "math3d.h"
#include "constants.h"

namespace picogen { namespace cracker {

Color::Optional LambertMaterial::brdf_(const InDirection &,
                                       const OutDirection &,
                                       Random &) const
{
        return Color::Gray(0.25) / constants::pi;
}

BsdfSample LambertMaterial::sample_ (OutDirection const &out,
                                     Random &rand) const
{
        using namespace std;
        const tuple<real,real,real> sphere = cosineHemisphereR(rand);
        const Direction in (get<0>(sphere), get<1>(sphere), get<2>(sphere));
        //const Direction in (0,1,0);
        const InDirection &ind (in);
        const Color::Optional optc = brdf(ind, out, rand);
        assert (optc);
        return BsdfSample(ind,
                          fabs(in.y() / constants::pi),
                          optc.color());
}

real LambertMaterial::pdf_ (const InDirection &in, const OutDirection &out) const {
        if (sameHemisphere (out.direction(),
                            in.direction())) {
                return fabs(in.direction().y() / constants::pi);
        }
        return 0;
}

} }
