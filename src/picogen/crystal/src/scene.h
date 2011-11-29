#ifndef SCENE_H_INCLUDED_20111129
#define SCENE_H_INCLUDED_20111129

#include "crystal_math.h"
#include "geometry.h"

namespace crystal {

// >> forward >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Volume;



// >> Scene >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Scene {
public:
        Scene() = delete;

        Scene (shared_ptr<const Geometry> geometry)
                : geometry_(geometry)
        {}

        Geometry const& geometry() const {
                return *geometry_;
        }
private:
        shared_ptr<const Geometry>          geometry_;
        shared_ptr<const Volume>            volume_;
};

}

#endif // SCENE_H_INCLUDED_20111129
