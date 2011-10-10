#ifndef SCENE_H
#define SCENE_H

#include "intersection.h"
//#include "ray.h"
#include "primitives/primitive.h"
#include "terrain/quadtree.h"
#include "sun.h"
#include "sky.h"

#include <list>
#include <memory>

namespace picogen { namespace cracker {

class Ray;
class Color;
class Point;
class Direction;
class Random;

class Scene
{
public:
        Scene();

        Intersection::Optional operator() (Ray const &ray) const;
        Color radiance (Point const&, Direction const &) const;


        Color estimateDirect (Point const &position,
                              Normal const &normal,
                              Direction const &wo,
                              Material const &mat,
                              Random &rand) const;
        Color background (Ray const &ray) const;

        void setSun (std::shared_ptr<Sun> sun);
        void setSky (std::shared_ptr<Sky> sky);

        const Sun &sun_ref() const { return *sun_; }
        const Sky &sky_ref() const { return *sky_; }

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
        Quadtree terrain_;
        std::shared_ptr<Sun> sun_;
        std::shared_ptr<Sky> sky_;
};

} }

#endif // SCENE_H
