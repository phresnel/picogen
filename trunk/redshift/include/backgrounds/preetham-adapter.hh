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

#ifndef PREETHAM_HH_INCLUDED_20100112
#define PREETHAM_HH_INCLUDED_20100112

#include "../basictypes/background.hh"

namespace redshift { namespace backgrounds {
        DefineFinalizer(PreethamAdapter);
        class PreethamAdapter
        : public Background
        , DoFinalize (PreethamAdapter)
        {
        public:
                PreethamAdapter (shared_ptr<redshift::background::Preetham> preetham) ;
                Color query (Ray const &ray) const;
                bool hasFastDiffuseQuery () const { return true; }
                Color diffuseQuery (Point const&, Normal const&) const ;
                Color atmosphereShade (
                        Color const &, Ray const &, real_t
                ) const;
                bool hasAtmosphereShade () const { return true; }
        private:
                shared_ptr<redshift::background::Preetham> preetham;
                mutable MersenneTwister<float,0,1> diffuseRng;
        };
} }

#endif // PREETHAM_HH_INCLUDED_20100112
