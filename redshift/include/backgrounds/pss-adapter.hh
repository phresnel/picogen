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

#ifndef PSSADAPTER_HH_INCLUDED_20100314
#define PSSADAPTER_HH_INCLUDED_20100314

#include "../basictypes/background.hh"

namespace redshift { namespace backgrounds {
        DefineFinalizer(PssAdapter);
        class PssAdapter
        : public Background
        , DoFinalize (PssAdapter)
        {
        public:
                PssAdapter (
                        shared_ptr<redshift::background::PssSunSky> preetham,
                        real_t sunSizeFactor,
                        real_t sunBrightnessFactor
                ) ;
                Color query (Ray const &ray) const;
                bool hasFastDiffuseQuery () const { return false; }
                //Color diffuseQuery (Point const&, Normal const&, Random &) const ;
                Color atmosphereShade (
                        Color const &, Ray const &, real_t
                ) const;
                bool hasAtmosphereShade () const { return true; }

                // TODO: refactor the sun functions into a PreethamSunAdapter
                bool hasSun () const { return true; }
                Vector getSunDirection () const { return normalize(preetham->GetSunPosition()); }
                Color getSunColor () const;
                Color querySun (Ray const &ray) const;
        private:

                redshift::background::PssSunSky::Spectrum query_ (Ray const &ray) const;
                shared_ptr<redshift::background::PssSunSky> preetham;
                real_t sunSizeFactor, sunBrightnessFactor;

                bool isInSunSolidAngle (Vector const &vec) const ;
        };
} }

#endif // PSSADAPTER_HH_INCLUDED_20100314
