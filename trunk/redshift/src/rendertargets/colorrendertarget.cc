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
#include "../../include/rendertargets/colorrendertarget.hh"
#include "../../include/smart_ptr.hh"

namespace redshift {


struct ColorRenderTarget::ColorRenderTargetLock : redshift::RenderTargetLock {
        
        redshift::ColorRenderTarget &display;
        
        ColorRenderTargetLock (redshift::ColorRenderTarget &display_)
        : display (display_)
        {
        }

        ~ColorRenderTargetLock () {                                       
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
                display.display [y*display.getWidth() + x] = color;
        }

        Color getPixel (int x, int y) const {
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
                return display.display [y*display.getWidth() + x];
        }
};



ColorRenderTarget::ColorRenderTarget (int width_, int height_)
: width(width_), height(height_), display(new Color [width*height])
{        
}



ColorRenderTarget::~ColorRenderTarget() {
}



shared_ptr<RenderTargetLock> ColorRenderTarget::lock () {
        return shared_ptr<RenderTargetLock> (new ColorRenderTargetLock (*this));
}



void ColorRenderTarget::flip() {
}



int ColorRenderTarget::getWidth() const {
        return width;
}



int ColorRenderTarget::getHeight() const {
        return height;
}

} // namespace redshift
