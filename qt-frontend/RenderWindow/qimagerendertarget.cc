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

#include "qimagerendertarget.hh"

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
                const color::RGB
                        rgb_ = color.toRGB(),
                        rgb(rgb_.R*255, rgb_.G*255, rgb_.B*255);

                const int
                        r = (int)rgb.R < 0 ? 0 : (int)rgb.R > 255 ? 255 : (int)rgb.R,
                        g = (int)rgb.G < 0 ? 0 : (int)rgb.G > 255 ? 255 : (int)rgb.G,
                        b = (int)rgb.B < 0 ? 0 : (int)rgb.B > 255 ? 255 : (int)rgb.B
                ;
                display.display.setPixel (x, y, QColor(r,g,b).rgb());
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
                return redshift::Color::FromRGB(
                                real_t(r), real_t(g), real_t(b));
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
                return redshift::Color::FromRGB(
                                real_t(r), real_t(g), real_t(b));
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
