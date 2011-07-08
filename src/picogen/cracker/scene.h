#ifndef SCENE_H
#define SCENE_H

#include "potentialintersection.h"
#include "ray.h"

namespace picogen { namespace cracker {

class Scene
{
public:
        Scene();

        PotentialIntersection operator() (Ray const &ray) const;
};

} }

#endif // SCENE_H
