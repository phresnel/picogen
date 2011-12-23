
#include "terrain.h"
#include "geometry/terrain2d.h"

namespace crystal {
        std::function<real(real,real)> quatsch_function_2d(std::string const &program);
}


namespace crystal { namespace api {

shared_ptr<Geometry const> create_quatsch_terrain_2d (
        std::string const                     &terrain_function,
        std::function<Radiance(Point const&)> texture,
        geometry::terrain2d::Deepness const   &deepness,
        int patch_resolution
) {

        auto height_fun = quatsch_function_2d(terrain_function);

        shared_ptr<const Geometry> geometry (new geometry::Terrain2d(
                                                texture,
                                                deepness,
                                                patch_resolution,
                                                height_fun
                                            ) );
        return geometry;
}


} }
