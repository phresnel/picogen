#include "whitted.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
#include "scene.h"
#include "random.h"
#include "material.h"

namespace picogen { namespace cracker {

namespace {
        Color whitterate (Ray const &ray,
                          Scene const &scene,
                          Random &random,
                          unsigned int max_depth);



        Color specular(Ray const &ray,
                       Scene const &scene,
                       Random &random,
                       unsigned int max_depth)
        {
                const Color::Optional &col_ =
                                mat.brdf(InDirection(ray.direction()),// TODO: todo
                                         OutDirection(-ray.direction()),
                                         random);
                assert (col_);
                const Color &col = col_.color();

                const Direction d = ray.direction();
                const Normal n    = i.normal();
                const Point  poi  = ray(distance) + n * 0.0001;
                const Vector ref  = static_cast<Vector>(d)
                                    - real(2)*mixed_dot(d,n)*n;

                return col+col*whitterate(Ray(poi, static_cast<Direction>(normalize(ref))),
                                          scene,
                                          random,
                                          max_depth-1);
        }



        Color whitterate (Ray const &ray,
                          Scene const &scene,
                          Random &random,
                          unsigned int max_depth)
        {
                if (!max_depth)
                        return Color::Black();

                const Intersection::Optional pi = scene(ray);
                if (pi) {
                        const Intersection &i = pi.intersection();
                        const real distance   = i.distance();
                        const Material &mat   = i.material_ref();

                        if (mat.whittedMirror()) {
                                return specular(ray, scene, random, max_depth);
                        } else if (const auto &col =
                                   mat.brdf(InDirection(ray.direction()),// TODO: todo
                                            OutDirection(-ray.direction()),
                                            random))
                        {
                                return col.color();
                        }

                }
                return Color::FromRgb(0.5,0.5,0.6);
        }
}

Color WhittedIntegrator::operator () (Ray const &ray,
                                      Scene const &scene,
                                      Random &random) const
{
        return whitterate (ray, scene, random, 5);
}

} }
