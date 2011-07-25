#ifndef SCENE_H
#define SCENE_H

#include "intersection.h"
//#include "ray.h"
#include "primitives/primitive.h"
#include "terrain/gridterrain.h"
#include "sun.h"

#include <list>
#include <memory>

namespace picogen { namespace cracker {

class Ray;
class Color;
class Point;
class Normal;

class Scene
{
public:
        Scene();

        Intersection::Optional operator() (Ray const &ray) const;
        Color radiance (Point const&, Normal const &) const;

        void setSun (Sun const &sun);

        const Sun &sun() const { return sun_; }

        // TODO: shall we use a builder pattern instead?
        // In the general case, we shouldn't be using generalized primitves in
        // cracker.
        // TODO: update: we should do this, as we can rid the virtual Primitive
        // interface away.
        template<typename T>
        void insertGenericPrimitive (T const &prim) {
                // TODO: maybe it is a good idea to lump primitivs of the same type id
                // into the same list

                genericPrimitives_.emplace_back(new T(prim));
        }

private:
        std::list<std::shared_ptr<Primitive> > genericPrimitives_;
        GridTerrain terrain_;
        Sun sun_;
};

} }

#endif // SCENE_H
