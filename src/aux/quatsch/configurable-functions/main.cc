#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <string>
#include <iostream>
#include <cmath>


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
public:
        Heightmap (const std::string &filename, PixelToHeightMode mode)
        : h(0), width_(0), height_(0)
        {
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
                }
        }

        virtual ~Heightmap () {
                delete [] h;
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


enum HeightToNormalMode {
        SlopeToNext, // i.e. N = cross(right,bottom) throws if heightmap is smaller than 2x2
        Sobel,       // throws if heightmap is smaller than 3x3
        Scharr,      // throws if heightmap is smaller than 3x3
        Auto
};

template <typename T, typename V> class Normalmap {
        V *n;
        unsigned int width_, height_;
public:
        Normalmap (Heightmap<T> const &heightmap, T worldHeight, HeightToNormalMode mode)
        : n(0), width_(0), height_(0)
        {
                switch (mode) {
                doSlopeToNext:
                case SlopeToNext: {
                        const unsigned int w = heightmap.width()-1;
                        const unsigned int h = heightmap.height()-1;
                        n = new V[w * h];
                        width_ = w;
                        height_ = h;

                        const T stepU = 1.f / width_;
                        const T stepV = 1.f / height_;
                        for (unsigned int y=0; y<height_; ++y)
                        for (unsigned int x=0; x<width_; ++x) {
                                const T
                                        center = heightmap.at (x,y),
                                        right  = heightmap.at(x+1, y) - center,
                                        bottom = heightmap.at(x, y+1) - center;
                                const V
                                        R = normalize(V(stepU, right, 0)),
                                        B = normalize(V(0, bottom, stepV)),
                                        C = cross (B,R)
                                ;
                                n[y*width_+x] = V(-C.x, C.z, C.y);
                        }
                        } break;

                doSobel:
                case Sobel: {
                        const unsigned int w = heightmap.width()-2;
                        const unsigned int h = heightmap.height()-2;
                        n = new V[w * h];
                        width_ = w;
                        height_ = h;

                        for (unsigned int y=0; y<height_; ++y)
                        for (unsigned int x=0; x<width_; ++x) {
                                const unsigned int hx = 1+x;
                                const unsigned int hy = 1+y;

                                const T
                                        a0 = heightmap.at(hx - 1,hy - 1),
                                        a1 = heightmap.at(hx    ,hy - 1),
                                        a2 = heightmap.at(hx + 1,hy - 1),

                                        b0 = heightmap.at(hx - 1,hy + 1),
                                        b1 = heightmap.at(hx    ,hy + 1),
                                        b2 = heightmap.at(hx + 1,hy + 1),

                                        c0 = heightmap.at(hx - 1,hy    ),
                                        c2 = heightmap.at(hx + 1,hy    );
                                const T dy = a0 * +1.0f  +  a1 * +2.0f  +  a2 * +1.0f
                                           + b0 * -1.0f  +  b1 * -2.0f  +  b2 * -1.0f;
                                const T dx = a0 * -1.0f  +  c0 * -2.0f  +  b0 * -1.0f
                                           + a2 * +1.0f  +  c2 * +2.0f  +  b2 * +1.0f;

                                // Cross Product of components of gradient reduces to:
                                n[y*width_+x] = normalize (V(dx, dy, 1));
                        }
                        } break;

                //doScharr:
                case Scharr: {
                        // Same as sobel, but with other kernels. Gives sharper results,
                        // but that might also not be ideal.
                        const unsigned int w = heightmap.width()-2;
                        const unsigned int h = heightmap.height()-2;
                        n = new V[w * h];
                        width_ = w;
                        height_ = h;

                        for (unsigned int y=0; y<height_; ++y)
                        for (unsigned int x=0; x<width_; ++x) {
                                const unsigned int hx = 1+x;
                                const unsigned int hy = 1+y;

                                const T
                                        a0 = heightmap.at(hx - 1,hy - 1),
                                        a1 = heightmap.at(hx    ,hy - 1),
                                        a2 = heightmap.at(hx + 1,hy - 1),

                                        b0 = heightmap.at(hx - 1,hy + 1),
                                        b1 = heightmap.at(hx    ,hy + 1),
                                        b2 = heightmap.at(hx + 1,hy + 1),

                                        c0 = heightmap.at(hx - 1,hy    ),
                                        c2 = heightmap.at(hx + 1,hy    );
                                const T dy = a0 * +3.0f  +  a1 * +10.0f  +  a2 * +3.0f
                                           + b0 * -3.0f  +  b1 * -10.0f  +  b2 * -3.0f;
                                const T dx = a0 * -3.0f  +  c0 * -10.0f  +  b0 * -3.0f
                                           + a2 * +3.0f  +  c2 * +10.0f  +  b2 * +3.0f;

                                // Cross Product of components of gradient reduces to:
                                n[y*width_+x] = normalize (V(dx, dy, 1));
                        }
                        } break;

                case Auto:
                        if (heightmap.width()<=1 || heightmap.height()<=1)
                                throw;
                        else if (heightmap.width()<=2 || heightmap.height()<=2)
                                goto doSlopeToNext;
                        else
                                goto doSobel;
                }
        }

        virtual ~Normalmap () {
                delete [] n;
        }


        V at (int x, int y) const {
                if ((x<0) | (x>=(int)width_) | (y<0) | (y>=(int)height_)) return V(0,1,0);
                return n[y*width_ + x];
        }

        V lerp (T x, T y) const {
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
                return normalize((1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)));
        }

        V cosine (T x, T y) const {
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
                return normalize(
                        (1-v)*((1-u)*at(a,b) + u*at(c,b))
                        +v*((1-u)*at(a,d) + u*at(c,d)));
        }

        private:
                static V cubic (V v0, V v1, V v2, V v3, T x) {
                        const V
                                P = (v3 - v2) - (v0 - v1),
                                Q = (v0 - v1) - P,
                                R = v2 - v0,
                                S = v1
                                ;
                        return P*x*x*x + Q*x*x + R*x + S;
                }
        public:

        V cubic (T x, T y) const {
                // as per http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
                const T
                        wx = 1+x * (width_-3),
                        wy = 1+y * (height_-3);
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
                const V
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
                return normalize(h);
        }

        V nearest (T x, T y) const {
                return normalize(at((int)(0.5 + x * (width_-1)), (int)(0.5 + y * (height_-1))));
        }
};

class Normal {
public:
        double x, y, z;
        Normal () {}
        Normal (Normal const &n) : x(n.x), y(n.y), z(n.z) {}
        Normal &operator = (Normal n) { x = n.x; y = n.y; z = n.z; return *this; }
        Normal(double x, double y, double z)
        : x(x), y(y), z(z) {}
};
Normal operator * (double f, Normal n) {
        return Normal(f*n.x, f*n.y, f*n.z);
}
Normal operator * (Normal n, double f) {
        return f * n;
}
Normal operator + (Normal l, Normal r) {
        return Normal(l.x+r.x, l.y+r.y, l.z+r.z);
}
Normal operator - (Normal l, Normal r) {
        return Normal(l.x-r.x, l.y-r.y, l.z-r.z);
}
Normal normalize(Normal n) {
        double is = 1/sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        return Normal(n.x*is, n.y*is, n.z*is);
}
Normal cross (Normal l, Normal r) {
        return Normal(
                l.y*r.z - l.z*r.y,
                l.z*r.x - l.x*r.z,
                l.x*r.y - l.y*r.x
        );
}

template <typename T> void render (SDL_Surface *screen,
                                   Heightmap<T> const &heightmap,
                                   Normalmap<T,Normal> const &normalmap
) {

        if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen)<0)
                return;

        //#pragma omp parallel for
        for (int y=0; y<screen->h; y++) {
                Uint32 *bufp32 = (Uint32*)screen->pixels + y*(screen->pitch/screen->format->BytesPerPixel);
                Uint16 *bufp16 = (Uint16*)screen->pixels + y*(screen->pitch/screen->format->BytesPerPixel);
                Uint8 *bufp8 = (Uint8*)screen->pixels + y*(screen->pitch/screen->format->BytesPerPixel);

                for (int x=0; x<screen->w; x++) {
                        const T u = (double)(x%(screen->w/2)) / (screen->w/2),
                                v = (double)(y%(screen->h/2)) / (screen->h/2);
                        const T
                                r_ = (y<screen->h/2)
                                        ? ((x<screen->w/2)?heightmap.nearest(u,v):heightmap.lerp(u,v))
                                        : ((x<screen->w/2)?heightmap.cosine(u,v):heightmap.cubic(u,v))
                                ;
                        const Normal
                                n = (y<screen->h/2)
                                        ? ((x<screen->w/2)?normalmap.nearest(u,v):normalmap.lerp(u,v))
                                        : ((x<screen->w/2)?normalmap.cosine(u,v):normalmap.cubic(u,v))
                                ;
                        const T
                                r = .5+.5*n.x    +(r_*0),
                                g = .5+.5*n.y,
                                b = .5+.5*n.z;
                        /*const T
                                r = .5+.5*n.z,
                                g = r,
                                b = r;*/
                        /*const T
                                r = r_<0.0?0.0:r_>1.0?1.0:r_,
                                g = r,
                                b = r;//*/

                        if (screen->format->BytesPerPixel == 4) {
                                *(bufp32++) =
                                        SDL_MapRGB(screen->format,
                                           (unsigned char)(255.0*r),
                                           (unsigned char)(255.0*g),
                                           (unsigned char)(255.0*b)
                                          );
                        } else if (screen->format->BytesPerPixel == 2) {
                                *(bufp16++) =
                                        SDL_MapRGB(screen->format,
                                           (unsigned char)(255.0*r),
                                           (unsigned char)(255.0*g),
                                           (unsigned char)(255.0*b)
                                          );
                        } else if (screen->format->BytesPerPixel == 1) {
                                *(bufp8++) =
                                        SDL_MapRGB(screen->format,
                                           (unsigned char)(255.0*r),
                                           (unsigned char)(255.0*g),
                                           (unsigned char)(255.0*b)
                                          );
                        } else {
                                std::cerr << "bpp is: " << (int)screen->format->BytesPerPixel << std::endl;
                        }
                }
        }

        if (SDL_MUSTLOCK(screen))
                SDL_UnlockSurface(screen);
        SDL_Flip (screen);
}

int main(int argc, char **argv)
{
	SDL_Surface *screen;
	SDL_Event event;
	int done=0;

	/* check arguments */
	if(argc!=2)
	{
		printf("%s filename\n",*argv);
		return 1;
	}

	/* startup SDL */
	if(SDL_Init(SDL_INIT_VIDEO)==-1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	/* load the image */
        Heightmap<double> heightmap (argv[1], Luminance);
        Normalmap<double, Normal> normalmap (heightmap, 1., Auto);
	/*image=IMG_Load(argv[1]);
	if(!image)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		SDL_Quit();
		return 1;
	}

	printf("loaded %s: %dx%d %dbpp\n", argv[1],
			image->w, image->h, image->format->BitsPerPixel);*/

	/* open the screen for displaying the image */
	/* try to match the image size and depth    */
	screen=SDL_SetVideoMode(512, 512, 32, SDL_ANYFORMAT);
	if(!screen)
	{
		printf("SDL_SetVideoMode: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	/* set the window title to the filename */
	SDL_WM_SetCaption(argv[1],argv[1]);

	/* print some info about the obtained screen */
	printf("screen is %dx%d %dbpp\n", screen->w, screen->h, screen->format->BitsPerPixel);

	/* do the initial image display */
	SDL_Flip(screen);

	/* the event loop, redraws if needed, quits on keypress or quit event */
	while(!done && SDL_WaitEvent(&event)!=-1)
	{
		render (screen, heightmap, normalmap);
		switch(event.type)
		{
			case SDL_QUIT: case SDL_KEYDOWN:
				done=1;
				break;
			case SDL_VIDEOEXPOSE:
				break;
			default:
				break;
		}
	}

	/* free the loaded image */

	/* shutdown SDL */
	SDL_Quit();

	return 0;
}

