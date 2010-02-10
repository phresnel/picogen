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

#ifndef VOLUME_HH_INCLUDED_20100210
#define VOLUME_HH_INCLUDED_20100210

#include "../setup.hh"
#include "scene.hh"
#include "sample.hh"
#include "transport.hh"


namespace redshift {

        // Phase functions.
        real_t PhaseIsotropic(const Vector &w, const Vector &wp);
        real_t PhaseRayleigh(const Vector &w, const Vector &wp);
        real_t PhaseMieHazy(const Vector &w, const Vector &wp);
        real_t PhaseMieMurky(const Vector &w, const Vector &wp);
        real_t PhaseHG(const Vector &w, const Vector &wp, real_t g);
        real_t PhaseSchlick(const Vector &w, const Vector &wp, real_t g);

        // VolumeRegion definition.
        class VolumeRegion {
        public:
                virtual Color sigma_a (const Point &p, const Vector &w)const=0;
                virtual Color sigma_s (const Point &p, const Vector &w)const=0;

                virtual Color Lve (const Point &p,const Vector &w) const = 0;

                virtual real_t p (const Point &p,
                                  const Vector &w_in,const Vector &w_out) = 0;

                virtual Color sigma_t (const Point &p, const Vector &w)const=0;

                virtual Color Tau (const Ray &r,
                                   real_t step=1.f, real_t offset=.5f) const=0;
        };
}

#endif // VOLUME_HH_INCLUDED_20100210
