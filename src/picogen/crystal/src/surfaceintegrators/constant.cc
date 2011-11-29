#include "constant.h"
#include "scene.h"

namespace crystal { namespace surfaceintegrators {

Constant::Constant(Radiance const &surf, Radiance const &sky)
        : surfaceColor_(surf), skyColor_ (sky)
{
}

Radiance Constant::get (Ray const &ray, Scene const &scene) const {
        const PIntersection pinter = scene.geometry().intersect(ray);

        return pinter ? surfaceColor_ : skyColor_;
        /*
        if (pinter) {
                //Intersection const &i = *pinter;
                //return Radiance::Gray(0.5+0.5*i.normal.x());
                return color_;
        } else {
                const Vector dir = ray.direction*1;
                return Radiance::FromRgb(0.5+dir.x,
                                         0.5+dir.y,
                                         0.5+dir.z);
        }*/
}

} }
