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

// Phase functions. (Direct transcription from PBRT.)
real_t PhaseIsotropic(const Vector &, const Vector &) {
	return 1.f / (4.f * constants::pi);
}
real_t PhaseRayleigh(const Vector &w, const Vector &wp) {
	real_t costheta = dot(w, wp);
	return  3.f/(16.f*constants::pi) * (1 + costheta * costheta);
}
real_t PhaseMieHazy(const Vector &w, const Vector &wp) {
	real_t costheta = dot(w, wp);
	return 9.f/(4.f*constants::pi) * powf(1.f + costheta*costheta, 8.f);
}
real_t PhaseMieMurky(const Vector &w, const Vector &wp) {
	real_t costheta = dot(w, wp);
	return 50.f/(4.f*constants::pi) * powf(1.f + costheta*costheta, 32.f);
}

real_t PhaseHG(const Vector &w, const Vector &wp, real_t g) {
	real_t costheta = dot(w, wp);
	return 1.f / (4.f * constants::pi) * (1.f - g*g) /
		powf(1.f + g*g - 2.f * g * costheta, 1.5f);
}

real_t PhaseSchlick(const Vector &w,
                   const Vector &wp, real_t g) {
	real_t k = 1.55f * g - .55f * g * g * g;
	real_t kcostheta = k * dot(w, wp);
	return 1.f / (4.f * constants::pi) * (1.f - k*k) /
		((1.f - kcostheta) * (1.f - kcostheta));
}


// VolumeRegion.
Color VolumeRegion::sigma_t(const Point &p, const Vector &w) const {
	return sigma_a(p, w) + sigma_s(p, w);
}


}
