#include "scene.h"

namespace picogen { namespace cracker {

Scene::Scene()
{
}

PotentialIntersection Scene::operator () (Ray const &ray) const {

        // Terrain
        // Water
        // Sky <-- really? <-- maybe instead provide preetham in terms of
        //                     volume integration?
        //                 <-- but that again would be against the performance
        //                     argument
        return PotentialIntersection();
}

} }
