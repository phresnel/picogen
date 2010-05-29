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

#ifndef SDLCOMMANDPROCESSOR_HH_INCLUDED_20090318
#define SDLCOMMANDPROCESSOR_HH_INCLUDED_20090318

#include "../setup.hh"
#include "../interaction/usercommandprocessor.hh"
#include "../auxiliary/stopwatch.hh"

namespace redshift { namespace interaction {

        DefineFinalizer(SdlCommandProcessor);
        class SdlCommandProcessor
        : public UserCommandProcessor, DoFinalize (SdlCommandProcessor) {
        public:
                SdlCommandProcessor () ;

                void tick () ;
                bool userWantsToQuit () const ;

        private:
                bool quit;
                StopWatch lastTime;

                SdlCommandProcessor (SdlCommandProcessor const &);
                SdlCommandProcessor & operator = (SdlCommandProcessor const &);
        };
} }

#endif // SDLCOMMANDPROCESSOR_HH_INCLUDED_20090318
