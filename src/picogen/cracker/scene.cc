#include "scene.h"

namespace picogen { namespace cracker {

Scene::Scene()
{
}

PotentialIntersection Scene::operator () (Ray const &ray) const {

        PotentialIntersection nearest;

        nearest = terrain_(ray);
        for (auto it = primitives_.begin(),
                  end=primitives_.end();
             it!=end; ++it)
        {
                const PotentialIntersection pi = (**it)(ray);
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

} }
