#ifndef TERRAIN_H_INCLUDED_20111223
#define TERRAIN_H_INCLUDED_20111223

#include "crystal_math.h"
#include "geometry/terrain2d/deepness.h"

#include <string>
#include <functional>

namespace crystal {
        class Geometry;
}

namespace crystal { namespace api {

        shared_ptr<Geometry const> create_quatsch_terrain_2d (
                std::string const                     &terrain_function,
                std::function<Radiance(Point const&)> texture,
                geometry::terrain2d::Deepness const   &deepness,
                int patch_resolution
        );
} }

#endif // TERRAIN_H_INCLUDED_20111223
