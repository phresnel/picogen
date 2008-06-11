/***************************************************************************
 *            mkheightmap.cc
 *
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/// \todo add an [optional] alpha channell to the heightmap, so it becomes usefull for creating 2d clouds
/// \todo include IEEE-floating-point binary-export option
/// \todo include ASCII-export option
/// \todo include RAW-export option
/// \todo include PNG-export option
/// \todo include BMP-export option
/// \todo include more visualisation modes than only greyscale
/// \todo include an option to only display a subset of the map
/// \todo include an option to zoom in/out
/// \todo include ram-less heightmap creation (will be slower when using normalization)
/// \todo include an option to sequentially save the heightmap, one chunk by another
/// \todo include options to set the x/y-center on the height-function
/// \todo This actually a project global todo: get rid of stuff like FLT_MAX, we want our project be portable.


#include <stdio.h>
#include <curses.h>
#include <ctype.h>

#include <iostream>

#include <SDL/SDL.h>

#include <picogen/picogen.h>


using picogen::common::real;
using picogen::common::Vector3d;



template <typename T> struct Heightmap {

    public:

        typedef T Hexel; // yes, hexel, as in pixel :P

    private:

    public:

        /// \todo hide member variables away into private section
        const real scaling;
        const unsigned int width;
        const unsigned int height;

        Hexel *map; // this is going to be a little bit low-level (as compared to operator [] overloading stuff)

        Heightmap( real scaling, unsigned int width, unsigned int height )
        : scaling (scaling),
          width (width), height (height), map (new Hexel[width*height])
        { }



        ~Heightmap() {
            delete [] map;
        }



        // okay, we provide an operator (), but just for screen dumping (hence i const it)
        const Hexel operator () ( unsigned int x, unsigned int y ) const {
            const Hexel tmp = map[ y*width + x ];
            return tmp < 0.0 ? 0.0 : tmp > 1.0 ? 1.0 : tmp;
        }



        void fillZero() {
            for (unsigned int x=0; x<width*height; x++) {
                map[ x ] = static_cast<Hexel>(0.0);
            }
        }



        void fill (const picogen::misc::functional::Function_R2_R1 &f, unsigned int antiAliasFactor) {

            using namespace std;

            fillZero();

            for (unsigned int y=0; y<height*antiAliasFactor; y++) {

                const unsigned int ofs_y = (y/antiAliasFactor)*width;
                const real v = scaling * (static_cast<real>(y) / static_cast<real>(width*antiAliasFactor));

                for (unsigned int x=0; x<width*antiAliasFactor; x++) {

                    const real u = scaling * (static_cast<real>(x) / static_cast<real>(width*antiAliasFactor));
                    map[ (x/antiAliasFactor) + ofs_y ] += static_cast<Hexel>(f(u,v)) / static_cast<Hexel>(antiAliasFactor*antiAliasFactor);

                }
            }
        }



        void normalize () {
            /// \todo Rewrite to 1d as in fillZero()
            using namespace std;
            Hexel minHeight_(map[0]);
            Hexel maxHeight_(map[0]);

            for (unsigned int y=0; y<height; y++) {
                const unsigned int ofs_y = y*width;
                for (unsigned int x=0; x<width; x++) {
                    const Hexel & m = map[ x + ofs_y ];
                    if( m<minHeight_ )
                        minHeight_ = m;
                    if( m>maxHeight_ )
                        maxHeight_ = m;
                }
            }

            const Hexel minHeight(minHeight_);
            const Hexel totalHeight   (1.0 / (maxHeight_ - minHeight_));
            for (unsigned int y=0; y<height; y++) {
                const unsigned int ofs_y = y*width;
                for (unsigned int x=0; x<width; x++) {
                    Hexel &m = map[ x + ofs_y ];
                    m = ( m - minHeight ) * totalHeight;
                }
            }
        }

};




/// \todo strip down function draw
template<class t_surface>
void
draw (
    SDL_Surface *p_target,
    const t_surface &surface,
    float scale,
    float exp_tone,
    float saturation,
    float waterLevel
) {
    if (SDL_MUSTLOCK (p_target) && SDL_LockSurface (p_target) <0)
        return;
    int x,y;
    for (y=0; y<p_target->h; y++) {
        /// \todo FIX we are currently assuming a 32bit SDL buffer
        /// \todo get rid of pointer arithmetic
        Uint32 *bufp   = (Uint32*) p_target->pixels + y* (p_target->pitch>>2);
        for (x=0; x<p_target->w; x++) {

            real accu_r=real (0), accu_g=real (0), accu_b=real (0);
            const real h_ = surface (x,y);
            const real h  = h_<0.0 ? 0.0 : h_>1.0 ? 1.0 : h_;

            if (h<waterLevel) {
                accu_r = 0.3;
                accu_g = 0.3;
                accu_b = 0.8;
            } else {
                accu_r = accu_g = accu_b = h;
            }


            * (bufp++) =
                SDL_MapRGB (p_target->format,
                            (unsigned char) (255.0*accu_r),
                            (unsigned char) (255.0*accu_g),
                            (unsigned char) (255.0*accu_b)
                           );
        }
    }


    if (SDL_MUSTLOCK (p_target))
        SDL_UnlockSurface (p_target);
    SDL_Flip (p_target);
}



template <typename T> int showPreviewWindow (T &heightmap, real waterLevel) {
    using namespace std;

    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        cerr << "Unable to init SDL for preview: " << SDL_GetError() << endl;
        return -1;
    }
    atexit (SDL_Quit);
    SDL_Surface *screen = SDL_SetVideoMode (heightmap.width, heightmap.height, 32, SDL_HWSURFACE);
    if (0 == screen) {
        cerr << "Unable to set video-mode for preview: " << SDL_GetError() << endl;
        return -1;
    }

    // dump the heightmap onto the screen
    draw (screen, heightmap, 1.0, 1.0, 1.0, waterLevel);

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent (&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    done = true;
                }
            }
        }
    }

    SDL_FreeSurface (screen);
    SDL_Quit();
    return 0;
}


void printUsage() {
}


int main_mkheightmap (int argc, char *argv[]) {

    using namespace std;
    using namespace picogen::misc::functional;

    if (argc<=0) {
        cerr << "error: no arguments given to `picogen mkheightmap`" << endl;
        printUsage();
        return -1;
    }

    typedef enum Lingua_t {
        Lingua_inlisp,
        Lingua_unknown
    } Lingua;
    std::string code("");
    Lingua lingua = Lingua_unknown;
    bool showPreview = false;
    unsigned int width=512, height=512;
    float scaling = 1.0;
    float waterLevel = -10000000.0;
    unsigned int antiAliasFactor = 1;
    //float heightmapNormalizationAccuracy = -1.0;
    bool doNormalize = false;

    while (argc>0) {
        const std::string option( argv[0] );
        argc--;
        argv++;

        // -Lx
        if (option[1] == 'L' && Lingua_unknown != lingua) {
            // we already have read some code, more is not allowed
            cerr << "error: only one formula/program allowed" << endl;
            printUsage();
            return -1;
        }
        if (option == "-Lin" || option == "--inlisp") {
            lingua = Lingua_inlisp;
            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            code = string (argv[0]);
            argc--;
            argv++;
        } else if (option == "-p" || option == "--preview") {
            showPreview = true;
        } else if (option == "-w"  || option == "--width") {

            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;
            // check for unsigned-integer'ness
            for (string::const_iterator it = intStr.begin(); it!=intStr.end(); ++it) {
                if (!isdigit (*it)) {
                    cerr << "error: only positive integer numbers are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%u", &width);

        } else if (option == "-h"  || option == "--height") {
            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;
            // check for unsigned-integer'ness
            for (string::const_iterator it = intStr.begin(); it!=intStr.end(); ++it) {
                if (!isdigit (*it)) {
                    cerr << "error: only positive integer numbers are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%u", &height);
        } else if (option == "-a"  || option == "--anti-aliasing") {
            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;
            // check for unsigned-integer'ness
            for (string::const_iterator it = intStr.begin(); it!=intStr.end(); ++it) {
                if (!isdigit (*it)) {
                    cerr << "error: only positive integer numbers >= 1 are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%u", &antiAliasFactor);
            if (antiAliasFactor<1) {
                cerr << "error: only positive integer numbers >= 1 are allowed for option " << option << endl;
                printUsage();
                return -1;
            }
        } else if (option == "-W"  || option == "--domain-width") {
            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;

            // Check for correct format.
            bool hasDot = false;
            string::const_iterator it = intStr.begin();
            if (*it == '-' ) {
                ++it;
            }
            for ( ; it!=intStr.end(); ++it) {
                if (*it == '.') {
                    if (hasDot) {
                        cerr << "error: wrong format for option " << option << " (too many dots)" << endl;
                        printUsage();
                        return -1;
                    }
                    hasDot = true;
                } else if (!isdigit (*it)) {
                    cerr << "error: only floating point numbers are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%f", &scaling);

        } else if (option == "-l"  || option == "--preview-water-level") {
            if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;

            // Check for correct format.
            bool hasDot = false;
            string::const_iterator it = intStr.begin();
            if (*it == '-' ) {
                ++it;
            }
            for ( ; it!=intStr.end(); ++it) {
                if (*it == '.') {
                    if (hasDot) {
                        cerr << "error: wrong format for option " << option << " (too many dots)" << endl;
                        printUsage();
                        return -1;
                    }
                    hasDot = true;
                } else if (!isdigit (*it)) {
                    cerr << "error: only floating point numbers are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%f", &waterLevel);
        } else if (option == "-n"  || option == "--normalize") {
            doNormalize = true;
            /*if (argc<=0) {
                cerr << "error: no argument given to option: " << option << endl;
                printUsage();
                return -1;
            }
            const string intStr = string (argv[0]);
            argc--;
            argv++;
            // check for unsigned-integer'ness
            bool hasDot = false;
            for (string::const_iterator it = intStr.begin(); it!=intStr.end(); ++it) {
                if (*it == '.') {
                    if (hasDot) {
                        cerr << "error: wrong format for option " << option << " (too many dots)" << endl;
                        printUsage();
                        return -1;
                    }
                    hasDot = true;
                }else if (!isdigit (*it)) {
                    cerr << "error: only floating point numbers are allowed for option " << option << endl;
                    printUsage();
                    return -1;
                }
            }
            /// \todo get rid of below sscanf
            sscanf (intStr.c_str(), "%f", &heightmapNormalizationAccuracy);
            if (heightmapNormalizationAccuracy < 0.0 || heightmapNormalizationAccuracy > 1.0) {
                cerr << "error: the argument for option " << option << " must be >= 0.0 and <= 1.0 (where 0.0 being 0%, 1.0 being 100%)" << endl;
                printUsage();
                return -1;
            }*/
        } else {
            cerr << "unknown option in argument list: " << option << endl;
            printUsage();
            return -1;
        }
    }

    if (lingua == Lingua_unknown) {
        cerr << "error: unkown language or wrong option used" << endl;
        printUsage();
        return -1;
    }



    try {

        Heightmap<double> heightmap (scaling, width, height);
        heightmap.fill (Function_R2_R1 (code), antiAliasFactor);
        if( doNormalize )
            heightmap.normalize();

        if (showPreview) {
            return showPreviewWindow (heightmap, waterLevel);
        }
    } catch (const functional_general_exeption &e) {
        cerr << "error: " << e.getMessage() << endl;
        return -1;
    }
    return 0;
}
