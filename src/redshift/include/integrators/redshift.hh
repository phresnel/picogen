//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
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

#ifndef REDSHIFTINTEGRATOR_INCLUDED_20100316
#define REDSHIFTINTEGRATOR_INCLUDED_20100316

#include "../geometry.hh"
#include "../sealed.hh"
#include "../basictypes/scene.hh"
#include "../basictypes/sample.hh"
#include "../basictypes/transport.hh"
#include "../basictypes/bsdf.hh"

namespace redshift {
        SEALED(RedshiftIntegrator);
        class RedshiftIntegrator
        : public Integrator
        , MAKE_SEALED(RedshiftIntegrator) {
        public:
                DistantRadiance Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const LiRecursion &,
                        Random& rand
                ) const ;

                RedshiftIntegrator (unsigned int numAmbientSamples=0);
        private:
                DistantRadiance Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const LiRecursion &,
                        Random& rand,
                        const bool doMirror
                ) const ;

                const int numAmbientSamples;
        };
}

#endif // REDSHIFTINTEGRATOR_INCLUDED_20100316
