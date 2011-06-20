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

#include "../../include/interaction/sdlcommandprocessor.hh"

#ifdef __MINGW32__
 #include <SDL/SDL.h>
#else
 #include <SDL.h>
#endif

namespace redshift { namespace interaction {



SdlCommandProcessor::SdlCommandProcessor ()
: quit(false)
{}



void SdlCommandProcessor::tick () {
        if (lastTime()<0.1)
                return;
        lastTime.restart();
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
