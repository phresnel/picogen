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

#ifndef AUX_HEIGHTMAP_HH_INCLUDED_20100424
#define AUX_HEIGHTMAP_HH_INCLUDED_20100424

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <cmath>

// TODO: put this into kallisto
namespace redshift { namespace aux {

enum PixelToHeightMode {
        Average,
        Luminance
};

template <typename T> class Heightmap {
        T *h;
        unsigned int width_, height_;

        static Uint32 getPixel (SDL_Surface *s, unsigned int x, unsigned int y) {
                switch (s->format->BytesPerPixel) {
                case 1: return ((Uint8*)s->pixels)[x + y*s->pitch];
                case 2: return ((Uint16*)s->pixels)[x + y*(s->pitch/2)];
                case 4: return ((Uint32*)s->pixels)[x + y*(s->pitch/4)];
                };
                return 0;
        }

        Heightmap (Heightmap const &);
        Heightmap & operator = (Heightmap const &) ;
public:
        Heightmap (const std::string &filename, PixelToHeightMode mode)
        : h(0), width_(0), height_(0)
        {
                if (!load (filename, mode))
                        throw std::runtime_error("error while loading " + filename);
        }

        Heightmap () {}

        virtual ~Heightmap () {
                delete [] h;
        }

        bool load (const std::string &filename, PixelToHeightMode mode) {
                SDL_Surface *image = IMG_Load(filename.c_str());
                if (image) {
                        h = new T[image->w*image->h];
                        width_ = image->w;
                        height_ = image->h;

                        for (unsigned int y=0; y<height_; ++y)
                        for (unsigned int x=0; x<width_; ++x) {
                                const Uint32 col = getPixel (image, x, y);
                                Uint8 r,g,b;
                                SDL_GetRGB (col, image->format, &r, &g, &b);

                                switch (mode) {
                                case Luminance:
                                        h[y*width_+x] = (0.299*r + 0.567*g + 0.114*b)/255; // this maybe for colorful images
                                        break;
                                case Average:
                                        h[y*width_+x] = (r+g+b)/T(3*255);
                                        break;
                                };
                        }
                        SDL_FreeSurface(image);
                        return true;
                }
                return false;
        }


        unsigned int width() const {
                return width_;
        }

        unsigned int height() const {
                return height_;
        }


        T at (int x, int y) const {
                if ((x<0) | (x>=(int)width_) | (y<0) | (y>=(int)height_)) return 0;
                return h[y*width_ + x];
        }

        T lerp (T x, T y) const {
                const T
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        a = (int)(wx),
                        b = (int)(wy),
                        c = 1 + (int)(wx),
                        d = 1 + (int)(wy);
                const T
                        u = wx - a,
                        v = wy - b;
                return (1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)) ;
        }

        T cosine (T x, T y) const {
                // as per http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
                const T
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        a = (int)(wx),
                        b = (int)(wy),
                        c = 1 + (int)(wx),
                        d = 1 + (int)(wy);
                const T
                        u_ = wx - a,
                        v_ = wy - b,
                        u = .5 * (1 - std::cos (3.1415927 * u_)),
                        v = .5 * (1 - std::cos (3.1415927 * v_))
                        ;
                return (1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)) ;
        }

        private:
                static T cubic (T v0, T v1, T v2, T v3, T x) {
                        const T
                                P = (v3 - v2) - (v0 - v1),
                                Q = (v0 - v1) - P,
                                R = v2 - v0,
                                S = v1
                                ;
                        return P*x*x*x + Q*x*x + R*x + S;
                }
        public:

        T cubic (T x, T y) const {
                // as per http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
                const T
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        x0 = -1 + (int)(wx),
                        y0 = -1 + (int)(wy),
                        x1 = (int)(wx),
                        y1 = (int)(wy),
                        x2 = 1 + (int)(wx),
                        y2 = 1 + (int)(wy),
                        x3 = 2 + (int)(wx),
                        y3 = 2 + (int)(wy)
                        ;
                const T
                        u = wx - x1,
                        v = wy - y1
                        ;
                const T
                        a0 = at(x0, y0),
                        a1 = at(x1, y0),
                        a2 = at(x2, y0),
                        a3 = at(x3, y0),

                        b0 = at(x0, y1),
                        b1 = at(x1, y1),
                        b2 = at(x2, y1),
                        b3 = at(x3, y1),

                        c0 = at(x0, y2),
                        c1 = at(x1, y2),
                        c2 = at(x2, y2),
                        c3 = at(x3, y2),

                        d0 = at(x0, y3),
                        d1 = at(x1, y3),
                        d2 = at(x2, y3),
                        d3 = at(x3, y3),

                        a = cubic(a0, a1, a2, a3, u),
                        b = cubic(b0, b1, b2, b3, u),
                        c = cubic(c0, c1, c2, c3, u),
                        d = cubic(d0, d1, d2, d3, u),
                        h = cubic(a, b, c, d, v)
                        ;
                return h;
        }

        T nearest (T x, T y) const {
                return at((int)(0.5 + x * (width_-1)), (int)(0.5 + y * (height_-1)));
        }
};

} }

#endif // AUX_HEIGHTMAP_HH_INCLUDED_20100424
