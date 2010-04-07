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


#include "../redshift/include/setup.hh"
#include "../redshift/include/backgrounds/preetham-shirley-smits/sunsky.hh"



redshift::Vector screenToHemisphereSat (float u, float v) {
        using namespace redshift;

        const float x = (u*2-1)*1.1;
        const float z = (v*2-1)*1.1;

        if (x*x+z*z>1.0f) {
                return normalize (redshift::Vector(x,0,z));
        }

        const float h = 1-x*x-z*z;
        const float y = sqrt (h);
        return normalize (redshift::Vector(x,y,z));
}





//#include <stdio>
#include <SDL/SDL.h>

#define WIDTH 512
#define HEIGHT WIDTH
#define BPP 4
#define DEPTH 32

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
        Uint32 *pixmem32;
        Uint32 colour;

        colour = SDL_MapRGB( screen->format, r, g, b );

        pixmem32 = (Uint32*) screen->pixels + y*WIDTH + x;
        *pixmem32 = colour;
}


void DrawScreen(SDL_Surface* screen) {
        //const double ft = std::fmod (6 + clock() / (double)CLOCKS_PER_SEC, 24);
        using namespace redshift;
        using namespace background;
        /*PssSunSky pss(
                30, // [in] lat Latitude (0-360)
                30,			// [in] long Longitude (-90,90) south to north
                0,			// [in] sm  Standard Meridian
                90,			// [in] jd  Julian Day (1-365)
                12.0,			// [in] tod Time Of Day (0.0,23.99) 14.25 = 2:15PM
                3,			// [in] turb  Turbidity (1.0,30+) 2-6 are most useful for clear days.
                true			// [in] initAtmEffects  if atm effects are not initialized, bad things will
                                        // happen if you try to use them....
        );*/
        PssSunSky pss (Vector(1,0.25,0), 15, 1., true);

        using namespace redshift;
        using namespace color;

        if (SDL_MUSTLOCK(screen)) {
                if (SDL_LockSurface(screen) < 0) return;
        }

        for (unsigned int y=0; y<HEIGHT; ++y) {
                for (unsigned int x=0; x<WIDTH; ++x) {
                        const int numSamples = 1;
                        RGB sum;
                        for (int i=0; i<numSamples; ++i) {

                                const real_t mu = rand() / (RAND_MAX+1.0f);
                                const real_t mv = rand() / (RAND_MAX+1.0f);
                                const real_t u = ((x+mu) / (real_t)WIDTH);
                                const real_t v = ((y+mv) / (real_t)HEIGHT);
                                const Vector d = screenToHemisphereSat(u,v);

                                #if 1
                                PssSunSky::Spectrum ssp = pss.GetSkySpectralRadiance(d);
                                const Vector viewer (0,0,0);
                                const Vector at = viewer + d * real_t(10000000.f);
                                PssSunSky::Spectrum att(Spectrum::noinit), inscatter(Spectrum::noinit);
                                pss.GetAtmosphericEffects (viewer, at, att, inscatter);
                                ssp = ssp*att + inscatter;

                                //ssp = ssp*att + inscatter*(1.-att);
                                /*const color::RGB rgbS = inscatter.toRGB();
                                const color::RGB rgbX = ssp.toRGB();
                                const color::RGB rgbR (200000,0,0);*/


                                //const RGB rgb = Spectrum::FromRGB(RGB(u,v,1)).toRGB();

                                /*std::cout << "\n\n";
                                std::cout << "spec: ";
                                for (int i=0; i<Spectrum::num_components; ++i)
                                        std::cout << s[i] << ' ';
                                std::cout << "\nrgb_:" << rgb.R << ' ' << rgb.G << ' ' << rgb.B << "\n";
                                std::cout << "\n\n";
                                exit(0);*/
                                const PssSunSky::Spectrum tot = ssp;
                                const SRGB& rgb = tot.toRGB().toSRGB();
                                        /*(x%10)==0 ? rgbR :
                                        (x/10)%2==0 ? rgbS : rgbX*/;
                                #else
                                real_t thetav = acos(d.up());
                                real_t phiv = atan2(d.ahead(),d.right());
                                RGB rgb (phiv, 0, 0);
                                #endif

                                sum.R += rgb.R;
                                sum.G += rgb.G;
                                sum.B += rgb.B;
                        }
                        const double fac = 0.02;//0.00005;
                        const int r_ = (sum.R*fac)*255 / numSamples;
                        const int g_ = (sum.G*fac)*255 / numSamples;
                        const int b_ = (sum.B*fac)*255 / numSamples;
                        const int r = r_<0 ? 0 : r_>255 ? 255 : r_;
                        const int g = g_<0 ? 0 : g_>255 ? 255 : g_;
                        const int b = b_<0 ? 0 : b_>255 ? 255 : b_;
                        setpixel (
                                screen, x, y,
                                r,
                                g,
                                b
                        );
                }
        }

        if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
        SDL_Flip(screen);
}


int main(int /*argc*/, char* /*argv*/[])
{
        //freopen( "CON", "w", stdout );
        redshift::static_init();


        using namespace redshift;
        using namespace color;
        using namespace background;
/*        PssSunSky pss (Vector(1,4,1), 1.9, 0, true);

        PssSunSky::Spectrum white_(1);
        for (double f=0; f<30000; f+=1000) {
                PssSunSky::Spectrum att_, inscatter_;
                pss.GetAtmosphericEffects(Vector(0,10,0), Vector(0,10+f*0.5,f),
                        att_, inscatter_);

                const RGB att = att_.toRGB();
                const RGB inscatter = inscatter_.toRGB();

                const PssSunSky::Spectrum white__ = white_ * att_;
                const RGB white = white__.toRGB();
                std::cout << std::fixed
                        << f << ": (" << att.R << " | " << att.G << " | " << att.B << ")   "
                        << "   (" << inscatter.R << " | " << inscatter.G << " | " << inscatter.B << ")   "
                        << "   (" << white.R << " | " << white.G << " | " << white.B << ")"
                        << "\n"
                ;
        }

std::cout << "exiting from DrawScreen()." << std::endl;
return 0;*/


        SDL_Surface *screen;
        SDL_Event event;

        int keypress = 0;

        if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

        if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
                SDL_Quit();
                return 1;
        }

        //freopen( "CON", "w", stdout );

        while (!keypress) {
                DrawScreen(screen);

                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                        case SDL_QUIT:
                                keypress = 1;
                                break;
                        case SDL_KEYDOWN:
                                keypress = 1;
                                break;
                        }
                }
        }

        SDL_Quit();

        return 0;
}
