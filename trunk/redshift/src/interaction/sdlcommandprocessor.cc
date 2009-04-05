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

#include <SDL.h>
#include <ctime>

#include "../../include/setup.hh"
#include "../../include/interaction/usercommandprocessor.hh"
#include "../../include/interaction/sdlcommandprocessor.hh"

namespace redshift { namespace interaction {



SdlCommandProcessor::SdlCommandProcessor () 
: quit(false)
, lastTime (clock())
{}



void SdlCommandProcessor::tick () {
        clock_t const curr = clock();
        if (curr - lastTime < (CLOCKS_PER_SEC/10))
                return;
        lastTime = clock();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                // check for messages
                switch (event.type) {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                        if (SDLK_ESCAPE != event.key.keysym.sym)
                                break;
                        // Fall through
                case SDL_QUIT:
                        quit = true;
                        break;
                                        
                }
        }
}



bool SdlCommandProcessor::userWantsToQuit () const {
        return quit;
}



} }
