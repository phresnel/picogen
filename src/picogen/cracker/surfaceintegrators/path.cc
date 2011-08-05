#include "path.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
#include "scene.h"
#include "random.h"
#include "material.h"
#include "constants.h"

namespace picogen { namespace cracker {

namespace detail {
        Color path_recursive (Ray const &ray,
                              Scene const &scene,
                              Random &random,
                              unsigned int rec)
        {
                if (rec==0) return Color::Black();

                const Intersection::Optional PI = scene(ray);
                if (!PI) return Color::Gray(0.4);

                const Intersection &I = PI.intersection();
                const DifferentialGeometry &DG = I.differentialGeometry();
                const Normal &normal  = DG.geometricNormal();
                const Point      &POI = ray(I.distance()) + normal*0.0001;
                const Material &mat = I.material_ref();

                const Direction in_dir_local = DG.worldToLocal(-ray.direction());
                const BsdfSample sample = mat.sample(in_dir_local, random);

                if (sample.pdf() == 0) return Color::Black();

                const Direction out_dir_local = sample.incident().direction();
                const Direction out_dir_world = DG.localToWorld (out_dir_local);

                const Color& rad = scene.radiance(POI, out_dir_world);
                const Color::Optional& brdf = mat.brdf(InDirection(in_dir_local),
                                                       OutDirection(out_dir_local),
                                                       random);
                if (!brdf) return Color::Black();

                const real pdf = mat.pdf(in_dir_local, out_dir_local);
                if (pdf == 0) return Color::Black();

                const real dot = fabs(mixed_dot(out_dir_world, normal));
                const Color& c = rad * brdf.color() * (dot/pdf);
                return c + c*path_recursive (Ray(POI, out_dir_world), scene, random, rec-1);
        }

        Color sampleLights (Scene const &scene,
                            Point const &position,
                            Normal const &normal,
                            Direction const &wo,
                            Material const &mat,
                            Random &random)
        {
                //Color col = scene.radiance(position, wo);
                //const real dot = mixed_dot (wo, normal);
                //const Color brdf =
                //return col * dot;
                Color Li = scene.estimateDirect(position,
                                                normal,
                                                wo,
                                                mat,
                                                random);
                return Li;
        }

        Color path_iterative (Ray ray,
                              Scene const &scene,
                              Random &random,
                              const unsigned int rec)
        {
                Color ret = Color::Black();
                Color throughput = Color::White();
                for (unsigned int depth=0; depth!=rec; ++depth) {
                        //if (random() <= 0.8) {
                        //        throughput /= 0.8;
                        //} else break;

                        const Intersection::Optional PI = scene(ray);
                        if (!PI) {
                                ret += Color::FromRgb(0.5,0.5,0.8) * throughput;
                                break;
                        }

                        const Intersection &I = PI.intersection();
                        const DifferentialGeometry &DG = I.differentialGeometry();
                        const Normal &normal = DG.geometricNormal();
                        const Point      &POI = ray(I.distance()) + normal*0.0001;

                        const Material &mat = I.material_ref();

                        // -- calculate wi, wo, sample -------------------------
                        const Direction wo_world = -ray.direction();
                        const Direction wo_local = DG.worldToLocal(wo_world);
                        const BsdfSample sample = mat.sample(wo_local, random);
                        if (sample.pdf() == 0) return Color::Black();
                        const Direction wi_local = sample.incident().direction();
                        const Direction wi_world = DG.localToWorld (wi_local);


                        // -- direct lighting ----------------------------------
                        ret += sampleLights (scene, POI, normal, wi_world, mat, random)
                               * throughput;


                        // -- scale throughput ---------------------------------
                        const Color::Optional& brdf = mat.brdf(InDirection(wi_local),
                                                               OutDirection(wo_local),
                                                               random);
                        if (!brdf) break;

                        const real pdf = mat.pdf(InDirection(wi_local),
                                                 OutDirection(wo_local));
                        if (pdf == 0) break;

                        const real dot = fabs(mixed_dot(wi_world, normal));

                        throughput *= brdf.color() * (dot / pdf);

                        // -- next vertex --------------------------------------
                        ray = Ray(POI, wi_world);
                }
                return ret;
        }
}

Color PathIntegrator::operator() (Ray const &ray,
                                  Scene const &scene,
                                  Random &random) const
{
        return detail::path_iterative (ray, scene, random, 2);
}

} }
