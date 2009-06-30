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

#include <stdexcept>
#include <sstream>

#include <QColor>

#include "../include/qimagerendertarget.hh"

namespace redshift {


struct QImageRenderTarget::QImageRenderTargetLock : redshift::RenderTargetLock {
        
        redshift::QImageRenderTarget & display;
        
        QImageRenderTargetLock (redshift::QImageRenderTarget & display_)
        : display (display_)
        {
        }

        ~QImageRenderTargetLock () {                                       
        }

        void setPixel (int x, int y, redshift::Color const &color) {
                if (static_config::debug) {
                        if (x<0 || x>=display.getWidth()
                             || y<0 || y>=display.getHeight()
                        ) {
                                std::stringstream ss;
                                ss << "Invalid coordinates passed to "
                                   << "QImageRenderTargetLock: "
                                   << '(' << x << ',' << y << ')'
                                   << ", but resolution is " 
                                   << display.getWidth() << "x"
                                   << display.getHeight();
                                throw std::out_of_range (ss.str());
                        }
                }
                //display.display [y*display.getWidth() + x] = color;
                Rgb const  rgb = saturate (color.toRgb(), 0.0, 1.0);
                display.display.setPixel (x, y, ((uint)(255.0*rgb.r) << 16)
                                              | ((uint)(255.0*rgb.g) << 8)
                                              | (uint)(255.0*rgb.b) );
        }

        Color getPixel (int x, int y) const {
                if (static_config::debug) {
                        if (x<0 || x>=display.getWidth()
                             || y<0 || y>=display.getHeight()
                        ) {
                                std::stringstream ss;
                                ss << "Invalid coordinates passed to "
                                   << "QImageRenderTargetLock: "
                                   << '(' << x << ',' << y << ')'
                                   << ", but resolution is " 
                                   << display.getWidth() << "x"
                                   << display.getHeight();
                                throw std::out_of_range (ss.str());
                        }
                }
                qreal r, g, b, a;
                QColor (display.display.pixel (x, y)).getRgbF (&r, &g, &b, &a);
                redshift::Color ret;
                ret.fromRgb (Rgb(real_t(r), real_t(g), real_t(b)));
                return ret;
        }
};



struct QImageRenderTarget::QImageRenderTargetConstLock
: redshift::RenderTargetLock {
        
        redshift::QImageRenderTarget const & display;
        
        QImageRenderTargetConstLock (redshift::QImageRenderTarget const & display_)
        : display (display_)
        {
        }

        ~QImageRenderTargetConstLock () {                                       
        }

        void setPixel (int, int, redshift::Color const &) {}

        Color getPixel (int x, int y) const {
                if (static_config::debug) {
                        if (x<0 || x>=display.getWidth()
                             || y<0 || y>=display.getHeight()
                        ) {
                                std::stringstream ss;
                                ss << "Invalid coordinates passed to "
                                   << "QImageRenderTargetLock: "
                                   << '(' << x << ',' << y << ')'
                                   << ", but resolution is " 
                                   << display.getWidth() << "x"
                                   << display.getHeight();
                                throw std::out_of_range (ss.str());
                        }
                }
                qreal r, g, b, a;
                QColor (display.display.pixel (x, y)).getRgbF (&r, &g, &b, &a);
                redshift::Color ret;
                ret.fromRgb (Rgb(real_t(r), real_t(g), real_t(b)));
                return ret;
        }
};



QImageRenderTarget::QImageRenderTarget (int width_, int height_)
: width(width_), height(height_), display(width, height, QImage::Format_RGB32)
{        
}



QImageRenderTarget::~QImageRenderTarget() {
}



QImageRenderTarget::operator QImage () {
        return display;
}



shared_ptr<RenderTargetLock> QImageRenderTarget::lock () {
        return shared_ptr<RenderTargetLock> (new QImageRenderTargetLock (*this));
}



shared_ptr<RenderTargetLock const> QImageRenderTarget::lock () const {
        return shared_ptr<RenderTargetLock const>
                        (new QImageRenderTargetConstLock (*this));
}



void QImageRenderTarget::flip() {
}



int QImageRenderTarget::getWidth() const {
        return width;
}



int QImageRenderTarget::getHeight() const {
        return height;
}

} // namespace redshift
