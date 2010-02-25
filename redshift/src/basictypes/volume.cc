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

namespace redshift {

// VolumeRegion.
Color VolumeRegion::sigma_t(const Point &p, const Vector &w) const {
	return sigma_a(p, w) + sigma_s(p, w);
}



// DensityRegion.
Color DensityRegion::tau (
        const Ray &rn, const Interval &i,
        real_t stepSize, real_t offset
) const {
        real_t t0=i.min(), t1=i.max();

	Color tau = Color::fromRgb(0,0,0);

	t0 += offset * stepSize;
	while (t0 < t1) {
		tau = tau + this->sigma_t(rn(t0), -rn.direction);
		t0 += stepSize;
	}
	return tau * stepSize;
}

}