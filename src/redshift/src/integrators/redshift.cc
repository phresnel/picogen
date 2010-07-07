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

#include "../../include/integrators/redshift.hh"

namespace redshift {



RedshiftIntegrator::RedshiftIntegrator (unsigned int numAmbientSamples)
: numAmbientSamples(numAmbientSamples)
{
}




tuple<real_t,Color,real_t> RedshiftIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        Random &rand,
        const bool doMirror // TODO: I think that one can die
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


                if (doMirror && bsdf->is (Bsdf::reflection, Bsdf::specular)) {
                        Color spec = Color(0);

                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector,real_t> > v_ = bsdf->sample_f (
                                -ray.direction,
                                Bsdf::reflection, Bsdf::specular,
                                rand);
                        if (v_) {
                                const tuple<Color,Vector,real_t> v = *v_;
                                ray.direction = get<1>(v);
                                spec = spec + get<1>(scene.Li (ray, sample, rand)) * get<0>(v) * (1/get<2>(v));
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

                                        const real_t d = max(
                                            real_t(0),
                                            dot(sunDir,vector_cast<Vector>(normalS))
                                        );
                                        const Color sunColor_ = sun.color(sunRay);
                                        const Color sunColor = scene.attenuate(
                                                sunColor_,
                                                sunRay,
                                                sample,
                                                constants::infinity,
                                                rand
                                        );
                                        ret += surfaceColor * sunColor * d;
                                }
                        }

                        // Skylight.
                        Color skylightSum = Color(0);
                        for (int numSamples = 0; numSamples < numAmbientSamples; ++numSamples) {
                                Ray skyRay (ray.position, ray.direction);

                                const optional<tuple<Color,Vector,real_t> > v_ =
                                        bsdf->sample_f (
                                                -skyRay.direction,
                                                Bsdf::reflection, Bsdf::diffuse,
                                                rand);
                                if (v_) {
                                        skyRay.direction = get<1>(*v_);
                                        const real_t pdf = get<2>(*v_);

                                        Scene::LiMode m;
                                        m.SkipSun = true;
                                        const tuple<real_t,Color> L =
                                                scene.Li(skyRay,
                                                         sample,
                                                         rand,
                                                         m);

                                        const real_t d = max(real_t(0),
                                            dot(skyRay.direction, vector_cast<Vector>(normalS))
                                        );

                                        skylightSum += get<1>(L) * get<0>(*v_) * d*(1/pdf);
                                }
                        }

                        if (numAmbientSamples > 0)
                                ret += skylightSum * (1. / numAmbientSamples);

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



tuple<real_t,Color,real_t> RedshiftIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample, Random &rand
) const {
        return Li(scene, raydiff, sample, rand, true);
}

}
