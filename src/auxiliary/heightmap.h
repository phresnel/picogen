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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <cmath>

// TODO: put this into kallisto
namespace picogen { namespace redshift { namespace aux {

enum PixelToHeightMode {
        Average,
        Luminance
};

template <typename T> class Heightmap {
public:
        enum WrapMode { Zero, Wrap, Clamp, Mirror };

        Heightmap (const std::string &filename, PixelToHeightMode mode)
        : h(0), width_(0), height_(0)
        {
                if (!load (filename, mode))
                        throw std::runtime_error("error while loading " + filename);
        }

        Heightmap ()
        : h(0), width_(0), height_(0)
        {}

        virtual ~Heightmap () {
                delete [] h;
        }

        void setWrapMode (WrapMode mode) {
                wrapMode_ = mode;
        }

        WrapMode wrapMode() const {
                return wrapMode_;
        }

        bool load (const std::string &filename, PixelToHeightMode mode) {
                SDL_Surface *image = IMG_Load(filename.c_str());
                if (image) {
                        h = new T[image->w*image->h];
                        width_ = image->w;
                        height_ = image->h;

                        for (unsigned int y=0, my=height_-1; y<height_; ++y, --my) {
                        for (unsigned int x=0; x<width_; ++x) {
                                const Uint32 col = getPixel (image, x, y);
                                Uint8 r,g,b;
                                SDL_GetRGB (col, image->format, &r, &g, &b);

                                switch (mode) {
                                case Luminance:
                                        h[my*width_+x] = (0.299*r + 0.567*g + 0.114*b)/255; // this maybe for colorful images
                                        break;
                                case Average:
                                        h[my*width_+x] = (r+g+b)/T(3*255);
                                        break;
                                };
                        }
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
                switch (wrapMode_) {
                case Zero:
                        if ((x<0) | (x>=(int)width_) | (y<0) | (y>=(int)height_))
                                return 0;
                        break;
                case Wrap:
                        //TODO: wrapping should not be done with loops
                        while (x<0)             x+=width_;
                        while (x>=(int)width_)  x-=width_;
                        while (y<0)             y+=height_;
                        while (y>=(int)height_) y-=height_;
                        break;
                case Clamp:
                        if (x<0)             x=0;
                        if (x>=(int)width_)  x=width_-1;
                        if (y<0)             y=0;
                        if (y>=(int)height_) y=height_-1;
                        break;
                case Mirror:
                        {
                        // even or odd?
                        bool x_even = true;
                        bool y_even = true;
                        // now wrap
                        //TODO: wrapping should not be done with loops
                        while (x<0)             { x+=width_; x_even=!x_even; }
                        while (x>=(int)width_)  { x-=width_; x_even=!x_even; }
                        while (y<0)             {y+=height_; y_even=!y_even; }
                        while (y>=(int)height_) {y-=height_; y_even=!y_even; }
                        // possibly mirror
                        x = x_even ? x : (width_-1)-x;
                        y = y_even ? y : (height_-1)-y;
                        }
                        break;
                }
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


        /*
        //TODO: should be removed: at the integers, the curve becomes
        //      parallel to the horizontal axis, making the integer
        //      boundaries visible
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
                return  (1-v) * ((1-u)*at(a,b) + u*at(c,b))
                      +  v    * ((1-u)*at(a,d) + u*at(c,d)) ;
        }
        */

private:
        // from gimp / catmull
        static T cubic (
                          const T  pt0,
                          const T  pt1,
                          const T  pt2,
                          const T  pt3, const T dx)
        {
          return (T) ((( ( -pt0 + 3 * pt1 - 3 * pt2 + pt3 ) *   dx +
                               ( 2 * pt0 - 5 * pt1 + 4 * pt2 - pt3 ) ) * dx +
                             ( -pt0 + pt2 ) ) * dx + (pt1 + pt1) ) / 2.0;
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

private:
        T *h;
        unsigned int width_, height_;
        WrapMode wrapMode_;

        static Uint32 getPixel (SDL_Surface *s, unsigned int x, unsigned int y) {
                switch (s->format->BytesPerPixel) {
                case 1: return ((Uint8*)s->pixels)[x + y*s->pitch];
                case 2: return ((Uint16*)s->pixels)[x + y*(s->pitch/2)];
                case 3: {
                        unsigned int a = ((Uint8*)s->pixels)[3*x + y*s->pitch];
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

        Heightmap (Heightmap const &);
        Heightmap & operator = (Heightmap const &) ;
};

} } }

#endif // AUX_HEIGHTMAP_HH_INCLUDED_20100424
