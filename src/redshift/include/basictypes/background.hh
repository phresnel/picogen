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

#include "../setup.hh"

namespace redshift {
        // TODO: seperate file
        /*struct AtmosphericEffects {
                Spectrum attenuation;
                Spectrum inscatter;
        };*/

        /*class Background {
        public:
                virtual Color query (Ray const &ray) const = 0;

                // TODO: refactor the sun functions into a PreethamSunAdapter
                virtual bool hasSun () const { return false; }
                virtual Vector getSunDirection () const { return Vector(); }
                virtual Color getSunColor () const { return Color(); }
                virtual Color querySun (Ray const &) const { return Color(); }
                virtual bool isInSunSolidAngle (Vector const &) const { return false; }

                virtual bool hasFastDiffuseQuery () const {
                        return false;
                }
                virtual bool hasAtmosphereShade () const {
                        return false;
                }
                virtual Color diffuseQuery(Point const&, Normal const&, Random &) const {
                        return Color(0);
                }

                virtual Color atmosphereShade (
                        Color const &c, Ray const &, real_t
                ) const {
                        return c;
                }

                //virtual AtmosphericEffects atmosphericEffects (
        };*/

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
