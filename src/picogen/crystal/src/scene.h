#ifndef SCENE_H_INCLUDED_20111129
#define SCENE_H_INCLUDED_20111129

#include "crystal_math.h"
#include "geometry.h"

#include "background/sun.h"
#include "background/sky.h"


namespace crystal {

// >> forward >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Volume;



// >> Scene >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Scene {
public:
        Scene() = delete;

        Scene (shared_ptr<const Geometry> geometry,
               shared_ptr<const background::Sun> sun,
               shared_ptr<const background::Sky> sky)
                : geometry_(geometry),
                  sun_(sun),
                  sky_(sky)
        {}

        Geometry const& geometry() const {
                return *geometry_;
        }

        background::Sun const& sun() const {
                return *sun_;
        }

        background::Sky const& sky() const {
                return *sky_;
        }
private:
        shared_ptr<const Geometry>          geometry_;
        shared_ptr<const Volume>            volume_;
        shared_ptr<const background::Sun>   sun_;
        shared_ptr<const background::Sky>   sky_;

};

}

#endif // SCENE_H_INCLUDED_20111129
