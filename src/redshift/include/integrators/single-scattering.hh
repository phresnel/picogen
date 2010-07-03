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

#ifndef SINGLE_SCATTERING_HH_INCLUDED_20100211
#define SINGLE_SCATTERING_HH_INCLUDED_20100211

#include "../basictypes/volume.hh"
#include "../sealed.hh"

namespace redshift {

        // VolumeIntegrator.
        SEALED (SingleScattering);
        class SingleScattering
                : public VolumeIntegrator
                , MAKE_SEALED(SingleScattering)
        {
        public:
                SingleScattering (real_t stepSize, real_t cutoffDistance);
                tuple<real_t,Color> Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const Interval &interval, Random& rand
                ) const ;
                tuple<real_t,Color> Transmittance(
                        const Scene &scene,
                        const Ray &ray, const Sample &sample,
                        const Interval &interval, Random& rand
                ) const ;
        private:
                real_t stepSize, cutoffDistance;
        };
}

#endif // EMISSION_HH_INCLUDED_20100211
