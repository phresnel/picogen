#include "scene.h"
#include "color.h"

#include "ray.h"
#include "math3d.h"

namespace picogen { namespace cracker {

Scene::Scene()
: sun_(Direction(normalize<Direction>(-1,-1,0)))
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

} }
