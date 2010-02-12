//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef DIRECTLIGHTING_HH_INCLUDED_20090110
#define DIRECTLIGHTING_HH_INCLUDED_20090110

#include "../setup.hh"
#include "../basictypes/scene.hh"
#include "../basictypes/sample.hh"
#include "../basictypes/transport.hh"
#include "../basictypes/bsdf.hh"

namespace redshift {
        class DirectLighting : public Integrator {
        public:
                virtual tuple<real_t,Color,real_t> Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample
                ) const ;
                virtual tuple<real_t,Color> Li_VolumeOnly (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample
                ) const ;
        private:
                tuple<real_t,Color,real_t> Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const bool doMirror
                ) const ;

                mutable MersenneTwister<real_t,0,1> diffuseRng;
        };
}

#endif // DIRECTLIGHTING_HH_INCLUDED_20090110
