
#include "scene.h"
#include "crystal_math.h"

namespace crystal {

Radiance Scene::sunRadiance(const Point &pos, const Direction &n) const {
        Ray const &sr = sun_->deterministicShadowRay(pos);
        if (geometry_->intersect(sr))
                return Radiance::FromRgb(0,0,0);
        return dot(static_cast<Vector>(n),
                   static_cast<Vector>(sr.direction))
                * sun_->radiance();
}

Radiance Scene::estimateDirect (Point const &position,
                                Normal const &normal,
                                Direction const &wo,
                                //Material const &mat,
                                Radiance const &mat,
                                Random &random) const
{
        const Ray &sr = sun_->deterministicShadowRay (position);
        const Direction &wi = sr.direction;
        const bool occluded = static_cast<bool>(geometry_->intersect(sr));
        if (occluded) return Radiance::Black();

        const real absDot = fabs (dot(wi, static_cast<Direction>(normal)));
        const real pdf    = 1;

        const Radiance::Optional f = mat.brdf(InDirection(wo),
                                           OutDirection(wi),
                                           random);
        if (!f) return Radiance::Black();

        return f.Radiance() * sun_->radiance() * (absDot / pdf);
}

}
