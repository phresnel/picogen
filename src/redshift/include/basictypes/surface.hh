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

#ifndef SURFACE_HH_INCLUDED_20100709
#define SURFACE_HH_INCLUDED_20100709

#include "../sealed.hh"

namespace redshift {
        struct Pixel {
                Color sum;
                unsigned int sampleCount;

                Color average () const {
                        return sum * (real_t(1)/sampleCount);
                }

                void operator += (Pixel const &pixel) {
                        sum += pixel.sum;
                        sampleCount += pixel.sampleCount;
                }
        };

        SEALED(Surface);
        class Surface : MAKE_SEALED(Surface) {
        public:
                Surface (unsigned int w, unsigned int h)
                : width(w), height(h), surface(new Pixel[w*h])
                {}

                Pixel pixel (unsigned int x, unsigned int y) const {
                        return surface[y*width+x];
                }
                Color color (unsigned int x, unsigned int y) const {
                        return surface[y*width+x].average();
                }

                Pixel &pixel (unsigned int x, unsigned int y) {
                        return surface[y*width+x];
                }
        private:
                Surface &operator = (Surface const &);
                Surface (Surface const &);
                Surface();

                const unsigned int width, height;
                shared_array<Pixel> surface;
        };
}

#endif // SURFACE_HH_INCLUDED_20100709
