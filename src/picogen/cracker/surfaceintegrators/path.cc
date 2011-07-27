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

                const Intersection::Optional pi = scene(ray);
                if (!pi) return Color::Gray(0.4);

                const Intersection &i = pi.intersection();
                const Point &poi = ray(i.distance()) + i.normal()*0.0001;
                const DifferentialGeometry &dg = i.differentialGeometry();

                const Material &mat = i.material_ref();
                const BsdfSample sample = mat.sample(dg.worldToLocal(-ray.direction()),
                                                     random);

                if (sample.pdf() == 0) return Color::Black();

                const Direction d = dg.localToWorld (sample.incident().direction());

                //TODO -> need to transform ray into surface space
                /*
                return Color::FromRgb(0.5+d.x(),
                                      0.5+d.y(),
                                      0.5+d.z());
                                      */
                const Color& rad = scene.radiance(ray(i.distance()),
                                                  d);
                const Color::Optional& brdf = mat.brdf(InDirection(-ray.direction()),
                                                       OutDirection(d),
                                                       random);
                if (!brdf) return Color::Black();

                const Color& c = rad * brdf.color();
                return c;// + path (Ray(poi, d), scene, random, rec-1);
        }
}

Color PathIntegrator::operator() (Ray const &ray,
                                  Scene const &scene,
                                  Random &random) const
{
        return detail::path (ray, scene, random, 3);
}

} }
