#include "whitted.h"
#include "scene.h"

namespace crystal { namespace surfaceintegrators {


Radiance Whitted::get (Ray const &ray, Scene const &scene) const {
        const PIntersection pinter = scene.geometry().intersect(ray);
        if (pinter) {
                Intersection const &i = *pinter;
                const Direction sunDirection (-3,-1,0);
                const real f = dot (-static_cast<Normal>(sunDirection),
                                    i.normal);

                const Ray shadowRay (ray(i.distance)+
                                        static_cast<Vector>(i.normal)*0.0001,
                                     -sunDirection);
                //return Radiance::Gray(f*0.5+0.5);
                const bool shadow = scene.geometry().intersect(shadowRay);

                const bool s = shadow ? real(0) : real(1);
                return Radiance::Gray(f*0.5+0.5) * s;

        } else {
                const Vector dir = ray.direction*1;
                return Radiance::FromRgb(0.5+dir.x,
                                         0.5+dir.y,
                                         0.5+dir.z);
        }
}

} }
