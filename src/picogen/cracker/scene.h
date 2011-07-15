#ifndef SCENE_H
#define SCENE_H

#include "potentialintersection.h"
#include "ray.h"
#include "primitives/primitive.h"
#include "terrain/gridterrain.h"

#include <list>
#include <memory>

namespace picogen { namespace cracker {



class Scene
{
public:
        Scene();

        PotentialIntersection operator() (Ray const &ray) const;

        // TODO: shall we use a builder pattern instead?
        // In the general case, we shouldn't be using generalized primitves in
        // cracker.
        // TODO: update: we should do this, as we can rid the virtual Primitive
        // interface away.
        template<typename T>
        void insertPrimitive (T const &prim) {
                // TODO: maybe it is a good idea to lump primitivs of the same type id
                // into the same list

                primitives_.emplace_back(new T(prim));
        }

private:
        std::list<std::shared_ptr<Primitive> > primitives_;
        GridTerrain terrain_;
};

} }

#endif // SCENE_H
