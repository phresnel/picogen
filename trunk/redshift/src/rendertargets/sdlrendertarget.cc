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
#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"
#include "../../include/rendertargets/sdlrendertarget.hh"
#include "../../include/smart_ptr.hh"

namespace redshift {


struct SdlRenderTarget::SdlRenderTargetLock : redshift::RenderTargetLock {
        
        redshift::SdlRenderTarget &display;
        
        SdlRenderTargetLock (redshift::SdlRenderTarget &display_)
        : display (display_)
        {
                if (SDL_MUSTLOCK (display.display) && 
                        SDL_LockSurface (display.display)<0) {
                        std::stringstream ss;
                        ss << "Failed to lock display in SdlRenderTarget::"
                           << "SdlRenderTargetLock()";
                        throw std::runtime_error (ss.str());
                }
        }

        ~SdlRenderTargetLock () {
                if (SDL_MUSTLOCK (display.display))
                        SDL_UnlockSurface (display.display);                        
        }

        void setPixel (int x, int y, redshift::Color const &color) {
                if (static_config::debug) {
                        if (x<0 || x>=display.getWidth()
                             || y<0 || y>=display.getHeight()
                        ) {
                                std::stringstream ss;
                                ss << "Invalid coordinates passed to "
                                   << "ColorRenderTargetLock: "
                                   << '(' << x << ',' << y << ')'
                                   << ", but resolution is " 
                                   << display.getWidth() << "x"
                                   << display.getHeight();
                                throw std::out_of_range (ss.str());
                        }
                }
        
                Uint32 &bufp = static_cast<Uint32*>(display.display->pixels)
                                          [y * (display.display->pitch/4) + x];
                
                Rgb const  rgb = saturate (color.toRgb(), 0.0, 1.0);
                bufp = SDL_MapRGB(display.display->format,
                                (unsigned char)(255.0*rgb.r),
                                (unsigned char)(255.0*rgb.g),
                                (unsigned char)(255.0*rgb.b));
        }

        Color getPixel (int x, int y) const {
                // TODO STUB
                if (static_config::debug) {
                        if (x<0 || x>=display.getWidth()
                             || y<0 || y>=display.getHeight()
                        ) {
                                std::stringstream ss;
                                ss << "Invalid coordinates passed to "
                                   << "ColorRenderTargetLock: "
                                   << '(' << x << ',' << y << ')'
                                   << ", but resolution is " 
                                   << display.getWidth() << "x"
                                   << display.getHeight();
                                throw std::out_of_range (ss.str());
                        }
                }
                return Color();
        }
};



SdlRenderTarget::SdlRenderTarget (int width_, int height_)
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



SdlRenderTarget::~SdlRenderTarget() {
}



shared_ptr<RenderTargetLock> SdlRenderTarget::lock () {
        return shared_ptr<RenderTargetLock> (new SdlRenderTargetLock (*this));
}



void SdlRenderTarget::flip() {
        SDL_Flip (display);
}



int SdlRenderTarget::getWidth() const {
        return width;
}



int SdlRenderTarget::getHeight() const {
        return height;
}

} // namespace redshift
