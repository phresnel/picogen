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

namespace redshift { namespace background {
        class PssSunSky;
} }

namespace redshift { namespace backgrounds {

        DefineFinalizer(PssAdapter);
        class PssAdapter
        : public Sky
        , DoFinalize (PssAdapter)
        {
        public:
                PssAdapter (
                        shared_ptr<redshift::background::PssSunSky> preetham,
                        real_t sunSizeFactor,
                        real_t sunBrightnessFactor,
                        real_t atmosphereBrightnessFactor,
                        real_t atmosphericFxDistanceFactor
                ) ;
                ~PssAdapter();
        };
} }

#endif // PSSADAPTER_HH_INCLUDED_20100314
