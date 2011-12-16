#ifndef TERRAIN2D_H_20111118
#define TERRAIN2D_H_20111118

#include "geometry.h"
#include "terrain2d/quadtree.h"
#include "terrain2d/deepness.h"
#include <functional>

namespace crystal { namespace geometry {

class Terrain2d : public Geometry {
public:
        Terrain2d (std::function<Radiance(Point const &p)> color,
                   terrain2d::Deepness const &deepness, int patchResolution,
                   std::function<real(real,real)> fun);
private:
        PIntersection intersect_ (Ray const &ray) const ;

private:
        std::function<Radiance(Point const &p)> color_;
        terrain2d::Quadtree quadtree_;
};

} }

#endif // TERRAIN2D_H_20111118
