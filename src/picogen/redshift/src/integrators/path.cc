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
#include "../../include/random.hh"
#include "ignore_strict_aliasing" // because of boost::optional

namespace redshift {



PathIntegrator::PathIntegrator ()
{
}




DistantRadiance PathIntegrator::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        const LiRecursion &lirec,
        Random &rand
) const {
        real_t throughput = 1;
        if (lirec.depth()>50)
                return DistantRadiance(Color(0), Distance(constants::infinity));
        if (lirec.depth()>10) { // let us gamble then
                const real_t continueProb = 0.5;
                if (rand() >= continueProb)
                        return DistantRadiance(Color(0), Distance(constants::infinity));
                throughput *= 1 / continueProb;
        }

        const optional<Intersection> I (scene.intersect (raydiff));

        if (!I) {
                return DistantRadiance (
                        Color(0),
                        Distance(constants::infinity));
        } else {
                Color col (0);

                const DifferentialGeometry dg = I->getDifferentialGeometry();
                const Normal normalG = dg.getGeometricNormal();
                const Normal normalS = dg.getShadingNormal();
                const shared_ptr<Bsdf> bsdf = I->getPrimitive().getBsdf (dg);
                const BsdfSample bsdfSample = bsdf->sample_f (-raydiff.direction,
                                                             BsdfFilter::all(),
                                                             rand);
                const Point poi = dg.getCenter()
                                + vector_cast<PointCompatibleVector>(
                                        bsdfSample.type().isReflective()
                                        ? normalG
                                        : -normalG
                                  ) * real_t(0.001);
                const Distance distance (length(raydiff.position-poi));

                const bool applyDirectSunlight = bsdfSample.type().isDiffuse()
                                               && scene.getBackground()->sun();
                // Sunlight.
                if (applyDirectSunlight) {
                        const Sun& sun = *scene.getBackground()->sun();
                        const Vector sunDir = sun.direction();
                        const Ray sunRay (poi,sunDir);

                        const Color surfaceColor = bsdf->f(
                                -raydiff.direction,
                                sunDir,
                                BsdfFilter::allDiffuse(),
                                rand
                        );

                        if (!scene.doesIntersect (sunRay)) { // <-- won't give plausible results
                                                             //     if sun-ray would go through transparent
                                                             //     material
                                const real_t d = fabs(
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
                                col += surfaceColor * sunColor * d;
                        }
                }

                // Recurse.
                Scene::LiMode m;
                m.SkipSun = applyDirectSunlight;
                const Ray nextRay (poi, bsdfSample.incident());
                const Color incoming = scene.radiance(
                                nextRay,
                                sample,
                                lirec, rand, m
                );

                const real_t d =
                        bsdfSample.type().isSpecular()
                        ? 1
                        : fabs(dot(nextRay.direction,vector_cast<Vector>(normalS)));

                col += incoming * bsdfSample.color() * d * (1/bsdfSample.pdf());

                return DistantRadiance(col*throughput, distance);
        }
}

}
