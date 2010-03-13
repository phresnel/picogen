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


#include "../../include/integrators/single-scattering.hh"
#include "../../include/basictypes/scene.hh"

namespace redshift {



SingleScattering::SingleScattering (real_t stepSize_, real_t cutoffDistance_)
: stepSize(stepSize_), cutoffDistance(cutoffDistance_)
{
}



tuple<real_t,Color> SingleScattering::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        const Interval &interval,
        Random& rand
) const {
        const Ray ray = raydiff;
        const shared_ptr<VolumeRegion> vr = scene.getVolumeRegion();
        if (!vr) return 0.f;

        const shared_ptr<Background> bg = scene.getBackground();
        const Vector sunDir = bg->getSunDirection();
        const Color sunCol = bg->getSunColor();


	real_t t0, t1;
	//if (!vr || !vr->IntersectP(ray, &t0, &t1)) return 0.f;

        // quirk: should cull against AABB here
        t0 = interval.min();
        t1 = interval.max()>cutoffDistance?cutoffDistance:interval.max();

	// Do emission-only volume integration in _vr_
	Color Lv = Color::fromRgb(0.f,0.f,0.f);

	// Prepare for volume integration stepping
	const int N = static_cast<int>(ceil((t1-t0) / stepSize)); // Ceil2Int(), PBRT p. 856 (A.3.4)
        const real_t step = (t1 - t0) / N;

	Color Tr = Color::fromRgb(1.f,1.f,1.f);
	Point curr = ray(t0), prev;
	const Vector w = -ray.direction;

        t0 += rand()*step;
	/*if (sample)
		t0 += sample->oneD[scatterSampleOffset][0] * step;
	else
		t0 += RandomFloat() * step;
        */

	for (int i = 0; i < N; ++i, t0 += step) {
		prev = curr;
		curr = ray(t0);

                const real_t offset = rand();
                const Color stepTau =
                        vr->tau(
                                Ray(prev, ray.direction),//normalize(vector_cast<Vector>(curr - prev))),
                                Interval(0,step),
                                .5f * stepSize,
                                offset,
                                rand
                        );

		Tr = Tr * exp(-stepTau);

		// Terminate if transmittance is small
		if (Tr.Y() < 0.05) {
			const real_t continueProb = .5f;
			if (rand() > continueProb) break;
			Tr = Tr * (1/continueProb);
		}

                // Compute emission-only source term at _p_
		Lv = Lv + Tr * vr->Lve(curr, w, rand);
                const Color ss = vr->sigma_s (curr, w, rand);

                // Background
                const Ray sunRay (curr,sunDir);
                if (!scene.doesIntersect (sunRay)) {
                        const tuple<real_t,Color> T_ =
                                Transmittance(
                                        scene,
                                        sunRay,
                                        sample,
                                        Interval(0,cutoffDistance),
                                        rand
                                );//TODO: quirk interval max
                        const Color T = get<1>(T_);
                        const Color Ld = sunCol * T;
                        const real_t pdf = 1;
                        Lv = Lv + Tr * ss * vr->p(curr,w, -sunDir, rand) * Ld * (1.f/pdf);
                }
	}
	return make_tuple(1.f,Lv * step);
}



tuple<real_t,Color> SingleScattering::Transmittance(
        const Scene &scene,
        const Ray &ray, const Sample &sample,
        const Interval &interval, Random& rand
) const {
	if (!scene.getVolumeRegion())
                return make_tuple(1.f,Color::fromRgb(1,1,1));

	const real_t step = stepSize;//sample ? stepSize : 4.f * stepSize;
	const real_t offset = rand ();
		//sample ? sample->oneD[tauSampleOffset][0] :
		//RandomFloat();
	const Color tau =
		scene.getVolumeRegion()->tau(ray, interval, step, offset, rand);
	return make_tuple(1.f,exp(-tau));
}



}
