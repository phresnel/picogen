#include "whitted.h"

#include "color.h"
#include "ray.h"
#include "scene.h"
#include "random.h"
#include "material.h"

namespace picogen { namespace cracker {

Color WhittedIntegrator::operator () (Ray const &ray,
                                      Scene const &scene,
                                      Random &random) const
{
        /*Direction d = ray.direction();
        return Color::FromRgb(
                d.x()*0.5+0.5,
                d.y()*0.5+0.5,
                d.z()*0.5+0.5
        );*/

        const Intersection::Optional pi = scene(ray);
        if (pi) {
                const Intersection &i = pi.intersection();
                const real distance   = i.distance();
                const Point poi       = ray(distance);
                const Material &mat   = i.material_ref();

                if (auto col = mat.brdf(InDirection(ray.direction()),// TODO: todo
                                        OutDirection(-ray.direction()),
                                        random))
                {
                        return col.color();
                }

        }
        return Color::FromRgb(0.75,0.5,0.5);
}
} }
