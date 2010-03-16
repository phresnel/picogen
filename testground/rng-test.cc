#include "../redshift/include/setup.hh"

#include <stdio.h>
#include <SDL/SDL.h>

//#include "../kallisto/rng/kiss.hh"
#include <iostream>

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


redshift::Random createRandom(
        uint32_t a, uint32_t b, uint32_t c, uint32_t d
) {
        using namespace kallisto::random::marsaglia;
        MWC A(a,b);          // a, b
        MWC B(c,d);          // c, d
        for (uint32_t warm=0; warm<4; ++warm) {  // warm up
                A(); B();
        }
        MWC mwc = MWC(A(), B()); // A+B
        mwc.skip(16);      // warm up

        // IMPORTANT: using variables instead of passing
        // as between the ( and the ) of a call, there is no sequence point
        // (and thus it'd be not portable)
        const uint32_t e=mwc(), f=mwc(), g=mwc(), h=mwc();
        redshift::Random ret = redshift::Random (e, f, g, h);
        ret.skip(16);
        return ret;
}


void DrawScreen(SDL_Surface* screen) {

        if (SDL_MUSTLOCK(screen)) {
                if (SDL_LockSurface(screen) < 0) return;
        }

        static int userSalt = 0;
        static int sample = 0;

        static long long int funCount = 0;
        funCount++;

        if (funCount % 3 == 1) sample++;
        if (funCount % 3 == 2) userSalt++;

        double avg = 0;

        static double avgB[WIDTH*HEIGHT];

        for (unsigned int y=0; y<HEIGHT; ++y) {
                for (unsigned int x=0; x<WIDTH; ++x) {
                        redshift::Random rand = createRandom(x,y,userSalt,sample);
                        /*
                        using namespace kallisto::random::marsaglia;
                        MWC coordR(x,y);
                        MWC salt(userSalt,sample);
                        for (int i=0; i<4; ++i) {
                                coordR(); salt();
                        }
                        MWC mwc(coordR(), salt());
                        mwc.skip(16);
                        const uint32_t a=mwc(), b=mwc(), c=mwc(), d=mwc();
                        redshift::Random rand(a,b,c,d);
                        rand.skip(16);
                        */

                        double r_ = rand();

                        avgB[y*WIDTH + x] += r_;
                        int r = 255 * (avgB[y*WIDTH + x] / funCount);
                        setpixel (
                                screen, x, y,
                                r,r,r
                        );
                }
        }

        static double static_avg = 0.0;
        static_avg += (avg / (WIDTH*HEIGHT));
        //std::cout << std::fixed << (avg / (WIDTH*HEIGHT)) << std::endl;
        std::cout << std::fixed << (static_avg / funCount) << "    \r" << std::flush;

        if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
        SDL_Flip(screen);
}

int main(int argc, char* argv[])
{
        freopen( "CON", "w", stdout );

        /*for (int i=0; i<8; ++i) {
                using namespace kallisto::random::marsaglia;
                UNI rand = UNI(0,i,0,0);
                for (int i=0; i<4; ++i) {
                        std::cout << rand() << ", ";
                }
                std::cout << std::endl;
        }

        return 0;//*/
        SDL_Surface *screen;
        SDL_Event event;

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
