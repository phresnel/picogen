//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../../include/integrators/path.hh"

namespace redshift {



PathIntegrator::PathIntegrator ()
{
}




tuple<real_t,Color,real_t> PathIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        Random &rand/*,
        const bool doMirror // TODO: I think that one can die*/
) const {
        const optional<Intersection> I (scene.intersect (raydiff));

        if (I) {
                const DifferentialGeometry gd = I->getDifferentialGeometry();
                const shared_ptr<Bsdf> bsdf = I->getPrimitive()->getBsdf (gd);
                const real_t       distance = I->getDistance();
                const shared_ptr<Sky> bg (scene.getBackground());
                const Normal normalG = gd.getGeometricNormal();
                const Normal normalS = gd.getShadingNormal();
                const Point poi = gd.getCenter()+
                        vector_cast<PointCompatibleVector>(normalG*real_t(0.001));


                if (bsdf->is (Bsdf::reflection, Bsdf::specular)) {
                        Color spec = Color(0);

                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector,real_t> > v_ = bsdf->sample_f (
                                -ray.direction,
                                Bsdf::reflection, Bsdf::specular,
                                rand);
                        if (v_) {
                                const tuple<Color,Vector,real_t> v = *v_;
                                ray.direction = get<1>(v);
                                Sample r = sample;
                                r.primaryRay = ray;
                                spec = spec + get<1>(scene.Li (r, rand)) * get<0>(v) * (1/get<2>(v));
                        }

                        return make_tuple(1.0f, spec, gd.getDistance());
                } else if (bsdf->is (Bsdf::reflection, Bsdf::diffuse)) {

                        const Ray ray (poi, raydiff.direction);
                        Color ret = Color(0);

                        // Sunlight.
                        if (bg->sun()) {
                                const Sun& sun = *bg->sun();
                                const Vector sunDir = sun.direction();
                                const Ray sunRay (poi,sunDir);
                                const Color surfaceColor = bsdf->f(
                                        -ray.direction,
                                        sunDir,
                                        Bsdf::reflection, Bsdf::diffuse,
                                        rand
                                );

                                if (!scene.doesIntersect (sunRay)) {
                                        Sample sunSample = sample;
                                        sunSample.primaryRay = sunRay;

                                        const real_t d = max(
                                            real_t(0),
                                            dot(sunDir,vector_cast<Vector>(normalS))
                                        );
                                        const tuple<real_t,Color> volumeLi = scene.Li(sunSample,rand,Scene::volume_only);
                                        const Color color = get<1>(volumeLi);
                                        ret += surfaceColor * color * d;
                                }
                        }

                        const optional<tuple<Color,Vector,real_t> > bsdfSample_ =
                                bsdf->sample_f (
                                        -ray.direction,
                                        Bsdf::reflection, Bsdf::diffuse,
                                        rand);

                        // NOTE: no need to test whether ray is in sun solid angle,
                        //       as incomingLight will be zero for all rays that
                        //       don't intersect.
                        if (bsdfSample_) {

                                const Color surfaceColor = get<0>(*bsdfSample_);
                                const Ray skyRay (ray.position, get<1>(*bsdfSample_));
                                const real_t pdf = get<2>(*bsdfSample_);

                                Sample s = sample;
                                s.primaryRay = skyRay;
                                const tuple<real_t,Color> L = scene.Li(s, rand);
                                const Color incomingLight = get<1>(L);

                                const real_t d = max(
                                    real_t(0),
                                    dot(skyRay.direction, vector_cast<Vector>(normalS))
                                );

                                ret += incomingLight*surfaceColor * d / pdf;
                        }

                        // Done.
                        return make_tuple(1.0f, ret, gd.getDistance());
                }

                return make_tuple(1.0f, Color(0), gd.getDistance());
        } else {
                return make_tuple (1.0,
                        Color(0),
                        constants::infinity
                );
        }
}

}
