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
                       Intersection const &intersection,
                       Random &random,
                       unsigned int max_depth)
        {
                const Material &mat = intersection.material_ref();
                const Color::Optional &col_ =
                                mat.brdf(InDirection(ray.direction()),// TODO: todo
                                         OutDirection(-ray.direction()),
                                         random);
                assert (col_);
                const Color &col = col_.color();

                const Direction d = ray.direction();
                const Normal n    = intersection.normal();
                const Point  poi  = ray(intersection.distance()) + n * 0.0001;
                const Vector ref  = static_cast<Vector>(d)
                                    - real(2)*mixed_dot(d,n)*n;

                return col*whitterate(Ray(poi, static_cast<Direction>(normalize(ref))),
                                      scene,
                                      random,
                                      max_depth-1);
        }

        Color specular_or_diffuse (Ray const &ray,
                                   Scene const &scene,
                                   Random &random,
                                   unsigned int max_depth,
                                   Intersection const &i) {
                //const real distance   = i.distance();
                const Material &mat   = i.material_ref();

                if (mat.whittedMirror()) {
                        return specular(ray, scene, i,
                                        random, max_depth);
                } else if (const auto &col =
                           mat.brdf(InDirection(ray.direction()),// TODO: todo
                                    OutDirection(-ray.direction()),
                                    random))
                {
                        return col.color() *
                               scene.radiance(ray(i.distance()),
                                              static_cast<Direction>(i.normal()));
                }
                // no luck with brdf
                return Color::Black();
        }

        Color whitterate (Ray const &ray,
                          Scene const &scene,
                          Random &random,
                          unsigned int max_depth)
        {
                if (!max_depth)
                        return Color::Black();

                const Intersection::Optional pi = scene(ray);

                if (!pi) return Color::FromRgb(0.5,0.5,0.6);

                return specular_or_diffuse (ray, scene,
                                            random, max_depth,
                                            pi.intersection());
        }
}

Color WhittedIntegrator::operator () (Ray const &ray,
                                      Scene const &scene,
                                      Random &random) const
{
        return whitterate (ray, scene, random, 5);
}

} }
