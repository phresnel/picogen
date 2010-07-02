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

#ifndef BACKGROUND_H_INCLUDED_20090309
#define BACKGROUND_H_INCLUDED_20090309

#include "../redshift-kallisto.hh"
#include "../basictypes/spectrum.hh"

namespace redshift {
        class Sun {
        public:
                virtual Vector direction() const = 0;
                virtual Color  color(Ray const &) const = 0;
                virtual bool   isInSunSolidAngle(Vector const &) const = 0;

                virtual ~Sun() {}
        };

        class Atmosphere {
        public:
                virtual Color  color(Ray const &) const = 0;
                virtual ~Atmosphere() {}
        };

        class AtmosphericEffects {
        public:
                virtual Color shade(Color const &, Ray const &, real_t distance) const = 0;
                virtual ~AtmosphericEffects() {}
        };

        class Sky {
                Sun *sunPtr;
                Atmosphere *atmospherePtr;
                AtmosphericEffects *atmosphericEffectsPtr;
        public:
                Sky()
                : sunPtr(0)
                , atmospherePtr(0)
                , atmosphericEffectsPtr(0)
                {}

                Sky(Sun *sunPtr,
                    Atmosphere *atmospherePtr,
                    AtmosphericEffects *atmosphericEffectsPtr
                )
                : sunPtr(sunPtr)
                , atmospherePtr(atmospherePtr)
                , atmosphericEffectsPtr(atmosphericEffectsPtr)
                {}

                const Sun*        sun()        const { return sunPtr; }
                const Atmosphere* atmosphere() const { return atmospherePtr; }
                const AtmosphericEffects* atmosphericEffects() const {
                        return atmosphericEffectsPtr;
                }
        };
}

#endif // BACKGROUND_H_INCLUDED_20090301
