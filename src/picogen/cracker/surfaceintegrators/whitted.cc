#include "whitted.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
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
                const Material &mat   = i.material_ref();

                if (mat.whittedMirror()) {
                        const Point  poi  = ray(distance);
                        const Normal n    = i.normal();
                        const Point  orig = poi + n * 0.0001;
                        //const auto rd =
                        return (*this)(Ray(orig,
                                           Direction(n.x(), n.y(), n.z())),
                                       scene,
                                       random);
                } else if (const auto &col =
                           mat.brdf(InDirection(ray.direction()),// TODO: todo
                                    OutDirection(-ray.direction()),
                                    random))
                {
                        return col.color();
                }

        }
        return Color::FromRgb(0.75,0.5,0.5);
}
} }
