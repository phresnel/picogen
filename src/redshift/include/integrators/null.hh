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

#ifndef NULLINTEGRATOR_INCLUDED_20100415
#define NULLINTEGRATOR_INCLUDED_20100415

#include "../setup.hh"
#include "../basictypes/scene.hh"
#include "../basictypes/sample.hh"
#include "../basictypes/transport.hh"
#include "../basictypes/bsdf.hh"

namespace redshift {
        DefineFinalizer(NullIntegrator);
        class NullIntegrator : public Integrator, public VolumeIntegrator
        , DoFinalize(NullIntegrator) {
        public:
                // Surface null
                tuple<real_t,Color,real_t> Li (
                        const Scene &,
                        const RayDifferential &,
                        const Sample &, Random&
                ) const {
                        return make_tuple (
                                1.0,
                                Color(0),
                                constants::infinity
                        );
                }

                // Volume null
                tuple<real_t,Color> Li (
                        const Scene &,
                        const RayDifferential &,
                        const Sample &,
                        const Interval &,
                        Random&
                ) const {
                        return make_tuple(1.f, Color(0));
                }
                tuple<real_t,Color> Transmittance(
                        const Scene &,
                        const Ray &, const Sample &,
                        const Interval &,
                        Random&
                ) const {
                        return make_tuple(1.f,Color(1));
                }

                NullIntegrator () {}
        private:
        };
}

#endif // NULLINTEGRATOR_INCLUDED_20100415
