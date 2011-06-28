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

#ifndef AUX_IMAGE_HH_INCLUDED_20100817
#define AUX_IMAGE_HH_INCLUDED_20100817

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <cmath>

// TODO: put this into kallisto
namespace picogen { namespace redshift { namespace aux {

class ColorImage {
        Color *h;
        real_t *alpha_;
        unsigned int width_, height_;

        static Uint32 getPixel (SDL_Surface *s, unsigned int x, unsigned int y) {
                switch (s->format->BytesPerPixel) {
                case 1: return ((Uint8*)s->pixels)[x + y*s->pitch];
                case 2: return ((Uint16*)s->pixels)[x + y*(s->pitch/2)];
                case 3: {
                        unsigned int a = ((Uint8*)s->pixels)[    3*x + y*s->pitch];
                        unsigned int b = ((Uint8*)s->pixels)[1 + 3*x + y*s->pitch];
                        unsigned int c = ((Uint8*)s->pixels)[2 + 3*x + y*s->pitch];
                        return (a<<16) | (b<<8) | c;
                        }
                case 4: {
                        unsigned int a = ((Uint8*)s->pixels)[    4*x + y*s->pitch];
                        unsigned int b = ((Uint8*)s->pixels)[1 + 4*x + y*s->pitch];
                        unsigned int c = ((Uint8*)s->pixels)[2 + 4*x + y*s->pitch];
                        unsigned int d = ((Uint8*)s->pixels)[3 + 4*x + y*s->pitch];
                        return (d<<24) | (a<<16) | (b<<8) | c;
                        }
                };
                return 0;
        }

        ColorImage (ColorImage const &);
        ColorImage & operator = (ColorImage const &) ;
public:
        ColorImage (const std::string &filename)
        : h(0), alpha_(0), width_(0), height_(0)
        {
                if (!load (filename))
                        throw std::runtime_error("error while loading " + filename);
        }

        ColorImage () {}

        virtual ~ColorImage () {
                delete [] h;
                delete [] alpha_;
        }

        bool load (const std::string &filename) {
                SDL_Surface *image = IMG_Load(filename.c_str());
                if (image) {
                        h = new Color[image->w*image->h];
                        alpha_ = new real_t[image->w*image->h];
                        width_ = image->w;
                        height_ = image->h;

                        for (unsigned int y=0; y<height_; ++y)
                        for (unsigned int x=0; x<width_; ++x) {
                                const Uint32 col = getPixel (image, x, y);
                                Uint8 r,g,b,a;
                                SDL_GetRGBA (col, image->format, &r,&g,&b,&a);

                                h[y*width_+x] = Color::FromRGB(
                                                        r/real_t(255),
                                                        g/real_t(255),
                                                        b/real_t(255),
                                                        ReflectanceSpectrum);
                                alpha_[y*width_+x] = a/real_t(255);

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


        Color at (int x, int y) const {
                if ((x<0) | (x>=(int)width_) | (y<0) | (y>=(int)height_))
                        return Color(0);
                return h[y*width_ + x];
        }

        real_t alpha_at (int x, int y) const {
                if ((x<0) | (x>=(int)width_) | (y<0) | (y>=(int)height_))
                        return real_t(0);
                return alpha_[y*width_ + x];
        }

        Color lerp (real_t x, real_t y) const {
                const real_t
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        a = (int)(wx),
                        b = (int)(wy),
                        c = 1 + (int)(wx),
                        d = 1 + (int)(wy);
                const real_t
                        u = wx - a,
                        v = wy - b;
                return (1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)) ;
        }

        real_t alpha_lerp (real_t x, real_t y) const {
                const real_t
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        a = (int)(wx),
                        b = (int)(wy),
                        c = 1 + (int)(wx),
                        d = 1 + (int)(wy);
                const real_t
                        u = wx - a,
                        v = wy - b;
                return (1-v)*((1-u)*alpha_at(a,b) + u*alpha_at(c,b))
                        +v*((1-u)*alpha_at(a,d) + u*alpha_at(c,d)) ;
        }

        Color cosine (real_t x, real_t y) const {
                // as per http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
                const real_t
                        wx = x * (width_-1),
                        wy = y * (height_-1);
                const int
                        a = (int)(wx),
                        b = (int)(wy),
                        c = 1 + (int)(wx),
                        d = 1 + (int)(wy);
                const real_t
                        u_ = wx - a,
                        v_ = wy - b,
                        u = .5 * (1 - std::cos (3.1415927 * u_)),
                        v = .5 * (1 - std::cos (3.1415927 * v_))
                        ;
                return (1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)) ;
        }

        private:
                static Color cubic (Color v0,
                                    Color v1,
                                    Color v2,
                                    Color v3,
                                    real_t x) {
                        const Color
                                P = (v3 - v2) - (v0 - v1),
                                Q = (v0 - v1) - P,
                                R = v2 - v0,
                                S = v1
                                ;
                        return P*x*x*x + Q*x*x + R*x + S;
                }
        public:

        Color cubic (real_t x, real_t y) const {
                // as per http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
                const real_t
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
                const real_t
                        u = wx - x1,
                        v = wy - y1
                        ;
                const Color
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

        Color nearest (real_t x, real_t y) const {
                return at((int)(0.5 + x * (width_-1)), (int)(0.5 + y * (height_-1)));
        }
};

} } }

#endif // AUX_IMAGE_HH_INCLUDED_20100817
