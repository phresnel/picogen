#include "whitted.h"
#include "scene.h"

namespace crystal { namespace surfaceintegrators {


Radiance Whitted::get (Ray const &ray, Scene const &scene) const {
        const PIntersection pinter = scene.geometry().intersect(ray);
        if (pinter) {
                Intersection const &i = *pinter;

                const Point poi       = ray(i.distance) + static_cast<Vector>(i.normal)*0.0001;
                const Ray   shadowRay = scene.sun().deterministicShadowRay(poi);
                const real  f_        = dot (static_cast<Normal>(shadowRay.direction), i.normal);
                const real  f         = f_ < 0 ? 0 : f_;

                const bool  shadow    = scene.geometry().intersect(shadowRay);
                const real  s         = shadow ? real(0) : real(1);
                //return Radiance::Gray(f*0.5+0.5) * s;
                return scene.sun().radiance()*f*s;

        } else {
                const Radiance sky = scene.sky().radiance(ray);
                //const Radiance sun = scene.sun().radiance(ray);
                return sky;
        }
}

} }
