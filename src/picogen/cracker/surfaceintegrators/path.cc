#include "path.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
#include "scene.h"
#include "random.h"
#include "material.h"

namespace picogen { namespace cracker {

namespace detail {
        Color path (Ray const &ray,
                    Scene const &scene,
                    Random &random,
                    unsigned int rec)
        {
                if (rec==0) return Color::Black();

                const Intersection::Optional PI = scene(ray);
                if (!PI) return Color::Gray(0.4);

                const Intersection &I = PI.intersection();
                const Point      &POI = ray(I.distance()) + I.normal()*0.0001;
                const DifferentialGeometry &DG = I.differentialGeometry();

                const Material &mat = I.material_ref();

                const Direction in_dir_local = DG.worldToLocal(-ray.direction());
                const BsdfSample sample = mat.sample(in_dir_local, random);

                if (sample.pdf() == 0) return Color::Black();

                const Direction out_dir_local = sample.incident().direction();
                const Direction out_dir_world = DG.localToWorld (out_dir_local);

                //TODO -> need to transform ray into surface space
                /*
                return Color::FromRgb(0.5+d.x(),
                                      0.5+d.y(),
                                      0.5+d.z());
                                      */
                const Color& rad = scene.radiance(POI, out_dir_world);
                const Color::Optional& brdf = mat.brdf(InDirection(in_dir_local),
                                                       OutDirection(out_dir_local),
                                                       random);
                if (!brdf) return Color::Black();

                const real pdf = mat.pdf(in_dir_local, out_dir_local);
                if (pdf == 0) return Color::Black();

                const real dot = fabs(mixed_dot(out_dir_world, I.normal()));
                const Color& c = rad * brdf.color() * (dot/pdf);
                return c + path (Ray(POI, out_dir_world), scene, random, rec-1);
        }
}

Color PathIntegrator::operator() (Ray const &ray,
                                  Scene const &scene,
                                  Random &random) const
{
        return detail::path (ray, scene, random, 3);
}

} }
