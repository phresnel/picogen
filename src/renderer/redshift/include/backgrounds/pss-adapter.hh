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
#include "../sealed.hh"
#include "../smart_ptr.hh"

namespace redshift { namespace background {
        class PssSunSky;
} }

namespace redshift { namespace backgrounds {

        SEALED(PssAdapter);
        class PssAdapter
        : public Sky
        , MAKE_SEALED (PssAdapter)
        {
        public:
                PssAdapter (
                        shared_ptr<redshift::background::PssSunSky> preetham,
                        real_t sunSizeFactor,
                        real_t sunBrightnessFactor,
                        real_t atmosphereBrightnessFactor,
                        real_t atmosphericEffectsFactor
                ) ;
                ~PssAdapter();
        private:
                // So sunSky won't be deleted, even when inner classes
                // use dumb-pointers/references.
                shared_ptr<redshift::background::PssSunSky> sunSky;
        };
} }

#endif // PSSADAPTER_HH_INCLUDED_20100314
