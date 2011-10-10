#ifndef GRID_TERRAIN_H_20110715
#define GRID_TERRAIN_H_20110715

#include "real.h"
#include "intersection.h"
#include "math3d/vector.h"
#include "aabb.h"
#include <vector>

namespace picogen { namespace cracker {

class Ray;
class Normal;

class GridTerrain {
public:
        GridTerrain ();
        Intersection::Optional operator() (Ray const &ray) const;
private:
        Normal normal_above(real centerH, real x, real z) const;
        Normal normal_below(real centerH, real x, real z) const;
        real height (int x, int y) const;
private:
        Vector size_;
        unsigned int heightfieldWidth_, heightfieldDepth_;
        std::vector<real> heightfield_;
        real pixelWidth_, pixelDepth_;
        BoundingBox aabb_;
};

} }

#endif // GRID_TERRAIN_H_20110715
