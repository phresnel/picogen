#ifndef SCENE_H_INCLUDED_20111129
#define SCENE_H_INCLUDED_20111129

#include "crystal_math.h"
#include "geometry.h"

#include "background/sun.h"
#include "background/sky.h"
#include "background/atmosphere.h"


namespace crystal {

// >> forward >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Volume;



// >> Scene >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Scene {
public:
        Scene() = delete;

        Scene (shared_ptr<const Geometry> geometry,
               shared_ptr<const background::Sun> sun,
               shared_ptr<const background::Sky> sky,
               shared_ptr<const background::Atmosphere> atmosphere
               )
                : geometry_(geometry),
                  sun_(sun),
                  sky_(sky),
                  atmosphere_(atmosphere)
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

        background::Atmosphere const& atmosphere() const {
                return *atmosphere_;
        }

        Radiance sunRadiance(const Point &pos, const Direction &n) const;
        Radiance estimateDirect (Point const &position,
                                 Normal const &normal,
                                 Direction const &wo,
                                 //Material const &mat,
                                 Radiance const &mat,
                                 Random &random) const;


private:
        shared_ptr<const Geometry>          geometry_;
        shared_ptr<const Volume>            volume_;
        shared_ptr<const background::Sun>   sun_;
        shared_ptr<const background::Sky>   sky_;
        shared_ptr<const background::Atmosphere> atmosphere_;

};

}

#endif // SCENE_H_INCLUDED_20111129
