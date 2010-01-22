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
        class Background {
        public:
                virtual Color query (Ray const &ray) const = 0;                

                // TODO: refactor the sun functions into a PreethamSunAdapter
                virtual bool hasSun () const { return false; }
                virtual Vector getSunDirection () const { return Vector(); }
                virtual Color querySun (Ray const &ray) { return Color(); }

                virtual bool hasFastDiffuseQuery () const {
                        return false;
                }
                virtual bool hasAtmosphereShade () const {
                        return false;
                }
                virtual Color diffuseQuery(Point const&, Normal const&) const {
                        return Color::fromRgb(0,0,0);
                }
                virtual Color atmosphereShade (
                        Color const &, Ray const &, real_t
                ) const {
                        return Color::fromRgb(0,0,0);
                }
        };
}

#endif // BACKGROUND_H_INCLUDED_20090301
