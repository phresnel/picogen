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

#include "ignore_strict_aliasing" // because of boost::optional
#include "../../include/integrators/whitted.hh"

namespace picogen { namespace redshift {



WhittedIntegrator::WhittedIntegrator ()
{}




DistantRadiance WhittedIntegrator::Li (
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
                const shared_ptr<Bsdf> bsdf   = I->getPrimitive().getBsdf (gd);
                const shared_ptr<Sky> bg      (scene.getBackground());
                const Normal normalG          = gd.getGeometricNormal(),
                             normalS          = gd.getShadingNormal();
                const Point poi               = gd.getCenter()+
                        vector_cast<PointCompatibleVector>(normalG*real_t(0.001));

                if (doMirror && bsdf->hasComponent(BsdfFilter::allSpecular())) {
                        Color spec = Spectrum::noinit;

                        const BsdfSample v = bsdf->sample_f (
                                -raydiff.direction,
                                BsdfFilter::allSpecular(),
                                rand);
                        const Ray ray (poi, v.incident());
                        spec = scene.radiance (ray, sample, lirec, rand);
                        return DistantRadiance(spec, Distance(length(raydiff.position-poi)));
                } else if (bg->sun()) {
                        const Sun& sun = *bg->sun();
                        const Vector sunDir = sun.direction();
                        const Ray sunRay (poi,sunDir);
                        const Color surfaceColor = bsdf->f(
                                -raydiff.direction,
                                sunDir,
                                BsdfFilter::allDiffuse(),
                                rand
                        );

                        if (!scene.doesIntersect (sunRay)) {

                                const real_t d = max(
                                    real_t(0),
                                    dot(sunDir,vector_cast<Vector>(normalS))
                                );
                                const Color sunColor = scene.attenuate(
                                        sun.color(sunRay),
                                        sunRay,
                                        sample,
                                        constants::infinity,
                                        rand
                                );
                                Color ret = surfaceColor * sunColor * d;
                                return DistantRadiance(ret, Distance(length(raydiff.position-poi)));
                        }
                }

                return DistantRadiance(Color(0), Distance(length(raydiff.position-poi)));
        } else {
                return DistantRadiance (
                        Color(0),
                        Distance(constants::infinity)
                );
        }
}



DistantRadiance WhittedIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        const LiRecursion &lirec,
        Random &rand
) const {
        return Li(scene, raydiff, sample, lirec, rand, true);
}

} }
