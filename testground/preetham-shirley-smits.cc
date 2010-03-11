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


#include "../poco/redshift/include/setup.hh"
#include "preetham-shirley-smits/sunsky.hh"



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





#include <stdio.h>
#include <SDL/SDL.h>

#define WIDTH 256
#define HEIGHT 256
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
        using namespace redshift;

        if (SDL_MUSTLOCK(screen)) {
                if (SDL_LockSurface(screen) < 0) return;
        }

        for (unsigned int y=0; y<HEIGHT; ++y) {
                for (unsigned int x=0; x<WIDTH; ++x) {
                        const real_t u = (x / (real_t)WIDTH);
                        const real_t v = (y / (real_t)HEIGHT);
                        const Vector d = screenToHemisphereSat(u,v);

                        setpixel (
                                screen, x, y,
                                d.x*128+128,d.y*128+128,d.z*128+128
                        );
                }
        }

        if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
        SDL_Flip(screen);
}


int main(int argc, char* argv[])
{
        SDL_Surface *screen;
        SDL_Event event;

        redshift::Spectrum::static_init();

        int keypress = 0;

        if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

        if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
                SDL_Quit();
                return 1;
        }

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
