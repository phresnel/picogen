#include "scene.h"

namespace picogen { namespace cracker {

Scene::Scene()
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

} }
