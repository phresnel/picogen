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

#ifndef FILM_HH_INCLUDED_20100709
#define FILM_HH_INCLUDED_20100709

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

                void operator += (Color const &color) {
                        sum += color;
                        ++sampleCount;
                }
        };

        SEALED(Film);
        class Film : MAKE_SEALED(Film) {
        public:
                Film (unsigned int w, unsigned int h)
                : width_(w), height_(h), surface(new Pixel[w*h])
                {}

                Pixel pixel (unsigned int x, unsigned int y) const {
                        return surface[y*width_+x];
                }
                Color average (unsigned int x, unsigned int y) const {
                        return surface[y*width_+x].average();
                }

                Pixel &pixel (unsigned int x, unsigned int y) {
                        return surface[y*width_+x];
                }

                unsigned int width()  const { return width_;  }
                unsigned int height() const { return height_; }
        private:
                Film &operator = (Film const &);
                Film (Film const &);
                Film();

                const unsigned int width_, height_;
                shared_array<Pixel> surface;
        };
}

#endif // FILM_HH_INCLUDED_20100709
