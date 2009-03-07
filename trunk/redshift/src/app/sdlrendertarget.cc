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

#include "SDL.h"
#include <stdexcept>
#include <sstream>

#include "../../include/setup.hh"
#include "../../include/basictypes/rendertargetlock.hh"
#include "../../include/basictypes/rendertarget.hh"
#include "../../include/app/sdlrendertarget.hh"
#include "../../include/smart_ptr.hh"


namespace redshift {

struct SDLRenderTargetLock : RenderTargetLock {
        SDLRenderTarget &display;
        SDLRenderTargetLock (SDLRenderTarget &display_) : display (display_) {
                if (SDL_MUSTLOCK (display.display) && 
                        SDL_LockSurface (display.display)<0) {
                        std::stringstream ss;
                        ss << "Failed to lock display in SDLRenderTarget::"
                           << "SDLRenderTargetLock()";
                        throw std::runtime_error (ss.str());
                }
        }

        ~SDLRenderTargetLock () {
                if (SDL_MUSTLOCK (display.display))
                        SDL_UnlockSurface (display.display);                        
        }

        void setPixel (int x, int y, redshift::Color &color) {
        }

        void getPixel (int x, int y, redshift::Color &color) const {
        }
};

SDLRenderTarget::SDLRenderTarget (int width_, int height_)
: width(width_), height(height_) {
        using namespace std;

        // Create a new window.
        display = SDL_SetVideoMode (width, height, 32, 
                                        SDL_HWSURFACE | SDL_DOUBLEBUF);
        if (0 == display) {                        
                stringstream ss;
                ss << "Unable to set video-mode to " 
                   << width << "x" << height << "x" << 32
                   << ": " << SDL_GetError();
                throw runtime_error (ss.str());
        }
}

SDLRenderTarget::~SDLRenderTarget() {
}

void SDLRenderTarget::lock (shared_ptr<RenderTargetLock>& lock) {
        lock = shared_ptr<RenderTargetLock> (new SDLRenderTargetLock (*this));
}

void SDLRenderTarget::flip() {
        SDL_Flip (display);
}

int SDLRenderTarget::getWidth() const {
        return width;
}

int SDLRenderTarget::getHeight() const {
        return height;
}

} // namespace redshift
