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




DistantRadiance RedshiftIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        const LiRecursion &lirec,
        Random &rand,
        const bool doMirror // TODO: I think that one can die
) const {
        if (lirec.depth()>10)  // plain and stupid
                return DistantRadiance (Color(0),Distance(constants::infinity));
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


                if (doMirror && bsdf->hasComponent (Bsdf::reflection, Bsdf::specular)) {
                        Color spec = Color(0);

                        Ray ray (poi, raydiff.direction);
                        const BsdfSample v = bsdf->sample_f (
                                -ray.direction,
                                Bsdf::reflection, Bsdf::specular,
                                rand);

                        ray.direction = v.incident();
                        const Color rad = scene.radiance (ray, sample, lirec, rand);
                        spec = spec + rad * v.color() * (1/v.pdf());

                        return DistantRadiance(spec, Distance(gd.getDistance()));
                } else if (bsdf->hasComponent(Bsdf::reflection, Bsdf::diffuse)) {

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

                                const BsdfSample v = bsdf->sample_f (
                                        -skyRay.direction,
                                        Bsdf::reflection, Bsdf::diffuse,
                                        rand);
                                skyRay.direction = v.incident();

                                Scene::LiMode m;
                                m.SkipSun = true;
                                m.SkipSurface = true;
                                const Color L = scene.radiance(
                                        skyRay, sample, lirec, rand, m);

                                const real_t d = max(real_t(0),
                                    dot(skyRay.direction, vector_cast<Vector>(normalS))
                                );

                                skylightSum += L * v.color() * d*(1/v.pdf());
                        }

                        if (numAmbientSamples > 0)
                                ret += skylightSum * (1. / numAmbientSamples);

                        // Done.
                        return DistantRadiance(ret, Distance(gd.getDistance()));
                }

                return DistantRadiance(Color(0), Distance(gd.getDistance()));
        } else {
                return DistantRadiance(
                        Color(0),
                        Distance(constants::infinity)
                );
        }
}



DistantRadiance RedshiftIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        const LiRecursion &lirec,
        Random &rand
) const {
        return Li(scene, raydiff, sample, lirec, rand, true);
}

}
