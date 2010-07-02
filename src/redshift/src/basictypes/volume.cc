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

#include "../../include/basictypes/volume.hh"
#include "../../include/random.hh"

namespace redshift {

// VolumeRegion.
Color VolumeRegion::sigma_t(const Point &p, const Vector &w, Random &rand) const {
        const Color a = sigma_a(p, w, rand);
        const Color b = sigma_s(p, w, rand);
        return a + b;
}



// DensityRegion.
Color DensityRegion::tau (
        const Ray &rn, const Interval &i,
        real_t stepSize, real_t offset, Random &rand
) const {
        real_t t0=i.min(), t1=i.max();

	Color tau (real_t(0));

	t0 += offset * stepSize;
	while (t0 < t1) {
		tau = tau + this->sigma_t(rn(t0), -rn.direction, rand);
		t0 += stepSize;

		const real_t cutoffDistance = 1000; // TODO: expose
		if (t0 > cutoffDistance) {
		        const real_t continueProb = .5f;
			if (rand() > continueProb) break;
			tau = tau * (1/continueProb);
		}
	}
	return tau * stepSize;
}

}
