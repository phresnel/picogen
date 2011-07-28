#include "scene.h"
#include "color.h"

#include "ray.h"
#include "math3d.h"

#include "material.h"
#include "random.h"

namespace picogen { namespace cracker {

Scene::Scene()
: sun_(Direction(normalize<Direction>(0,-1,0)))
{
}

Intersection::Optional Scene::operator () (Ray const &ray) const {

        Intersection::Optional nearest;

        nearest = terrain_(ray);
        for (auto it = genericPrimitives_.begin(),
                  end=genericPrimitives_.end();
             it!=end; ++it)
        {
                const Intersection::Optional pi = (**it)(ray);
                if (!pi) continue;
                const bool first_or_nearest = !nearest
                                            || nearer (pi.intersection(),
                                                       nearest.intersection());
                if (first_or_nearest)
                        nearest = pi;
        }
        // Terrain
        // Water
        // Sky <-- really? <-- maybe instead provide preetham in terms of
        //                     volume integration?
        //                 <-- but that again would be against the performance
        //                     argument
        return nearest;
}

Color Scene::radiance(const Point &pos, const Direction &n) const {
        Ray const &sr = sun_.deterministicShadowRay(pos);
        if ((*this)(sr))
                return Color::FromRgb(0,0,0);
        return mixed_dot(n, sr.direction()) * sun_.radiance();
}

Color Scene::estimateDirect (Point const &position,
                             Normal const &normal,
                             Direction const &wo,
                             Material const &mat,
                             Random &random) const
{
        const Ray &sr = sun_.deterministicShadowRay (position);
        const Direction &wi = sr.direction();
        const bool occluded = static_cast<bool>((*this)(sr));
        if (occluded) return Color::Black();

        const real absDot = fabs (mixed_dot(wi, normal));
        const real pdf    = 1;

        const Color::Optional f = mat.brdf(InDirection(wo),
                                           OutDirection(wi),
                                           random);
        if (!f) return Color::Black();

        return f.color() * sun_.radiance() * (absDot / pdf);
}


} }
