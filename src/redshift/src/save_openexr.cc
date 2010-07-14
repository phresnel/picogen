//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#include <OpenEXR/ImfRgbaFile.h>
#include "redshift/include/basictypes/film.hh"

namespace redshift {

void saveOpenEXR (Film const &film, const char *filename) {
        typedef redshift::color::RGB PicogenRGB;

        const unsigned int width = film.width(), height = film.height();
        std::vector<Imf::Rgba> imfpixels(width*height);

        for (unsigned int y=0; y<height; ++y)
        for (unsigned int x=0; x<width; ++x) {
                const PicogenRGB prgb = film.average(x, y).toRGB();
                Imf::Rgba &irgb = imfpixels[y*width+x];
                irgb.a = 1;
                irgb.r = prgb.R;
                irgb.g = prgb.G;
                irgb.b = prgb.B;
        }

        Imf::RgbaOutputFile file ("/home/smach/Desktop/pico.exr",
                                  width, height, Imf::WRITE_RGBA);
        file.setFrameBuffer (&imfpixels[0], 1, width);
        file.writePixels (height);
}

}