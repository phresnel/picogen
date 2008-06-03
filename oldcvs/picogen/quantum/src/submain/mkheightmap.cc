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


#include <iostream>

#include <SDL/SDL.h>

#include <picogen/picogen.h>


using picogen::common::real;
using picogen::common::Vector3d;



template <typename T> struct Heightmap {

    typedef T Hexel; // yes, hexel, as in pixel :P

    const real scaling;
    const unsigned int width;
    const unsigned int height;

    Hexel *map; // this is going to be a little bit low-level (as compared to operator [] overloading stuff)

    Heightmap( real scaling, unsigned int width, unsigned int height )
    : scaling (scaling), width (width), height (height), map (new Hexel[width*height])
    { }
    ~Heightmap() {
        delete [] map;
    }

    // okay, we provide an operator (), but just for screen dumping (hence i const it)
    const Hexel operator () ( unsigned int x, unsigned int y ) const {
        const Hexel tmp = map[ y*width + x ];
        return tmp < 0.0 ? 0.0 : tmp > 1.0 ? 1.0 : tmp;
    }

    void fill (const picogen::misc::functional::Function_R2_R1 &f) {
        for (unsigned int y=0; y<height; y++) {

            const unsigned int ofs_y = y*width;
            const real v = scaling * (static_cast<real>(y) / static_cast<real>(width));

            for (unsigned int x=0; x<width; x++) {

                const real u = scaling * (static_cast<real>(x) / static_cast<real>(width));
                map[ x + ofs_y ] = f(u,v); // yay

            }
        }
    }

};





template<class t_surface>
void
draw (
    SDL_Surface *p_target,
    const t_surface &surface,
    float scale,
    float exp_tone,
    float saturation
) {
    if (SDL_MUSTLOCK (p_target) && SDL_LockSurface (p_target) <0)
        return;
    int x,y;
    for (y=0; y<p_target->h; y++) {
        /// \todo FIX we are currently assuming a 32bit SDL buffer
        /// \todo get rid of pointer arithmetic
        Uint32 *bufp   = (Uint32*) p_target->pixels + y* (p_target->pitch>>2);
        for (x=0; x<p_target->w; x++) {
            /*float d = 1.0 / source->density;
            float r = source->color[0]*scale*d;
            float g = source->color[1]*scale*d;
            float b = source->color[2]*scale*d;*/
            real accu_r=real (0), accu_g=real (0), accu_b=real (0);
            const real h = surface (x,y);
            accu_r = accu_g = accu_b = h;
            /*
            for (register int u=0; u<2; ++u) for (register int v=0; v<2; ++v) {
                    real r,g,b;
                    ( (Color) surface (x*2+u,y*2+v)).to_rgb (r, g, b);
                    //((xrt::base_types::color)surface((unsigned)x,(unsigned)y)).to_rgb( r, g, b );
                    // tone map

                    // scale linearly
                    r *= scale;
                    g *= scale;
                    b *= scale;
                    // saturate color
                    const float sbrightness = 0.299*r + 0.584*g + 0.144*b;
                    r = saturation*r + (1-saturation) *sbrightness;
                    g = saturation*g + (1-saturation) *sbrightness;
                    b = saturation*b + (1-saturation) *sbrightness;
                    // final saturation
                    accu_r += r<0 ? 0 : r>1 ? 1 : r;
                    accu_g += g<0 ? 0 : g>1 ? 1 : g;
                    accu_b += b<0 ? 0 : b>1 ? 1 : b;
                }
            accu_r *= 0.25;
            accu_g *= 0.25;
            accu_b *= 0.25;
            */

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







std::string inlispEditor() {
    using namespace std;
    char s[1024];
    fflush (stdin);
    fgets (s, sizeof (s), stdin);
    return std::string (s);
    /*std::string code;
    std::cin >> code;
    return code;
    */
}

int main_mkheightmap (int argc, char *argv[]) {

    using namespace std;
    using namespace picogen::misc::functional;


    bool doContinue = true;

    while (doContinue) {
        using namespace std;
        cout << "Type in some formula in inlisp-syntax: " << endl;

        const string code = inlispEditor();

        if (doContinue) {

            cout << "you have written:\n   " << code << "\n";

            SDL_Surface *screen = 0;
            using namespace picogen::misc::functional;
            try {
                Heightmap<double> h( 100.0, 512, 512 );
                h.fill (Function_R2_R1 (code));


                if (SDL_Init (SDL_INIT_VIDEO) < 0) {
                    cerr << "Unable to init SDL: " << SDL_GetError() << endl;
                    return 1;
                }
                atexit (SDL_Quit);
                SDL_Surface *screen = SDL_SetVideoMode (h.width, h.height, 32, SDL_HWSURFACE);
                if (0 == screen) {
                    cerr << "Unable to set video-mode: " << SDL_GetError() << endl;
                    return 1;
                }


                // dump the heightmap onto the screen
                /// \todo include more visualisation modes than only greyscale
                /// \todo include an option to only display a subset of the map
                /// \todo include an option to zoom in/out
                draw (screen,h,1.0,1.0,1.0);

                // so, what do we next?
                bool isOptionValid = false;
                while (!isOptionValid) {
                    string whatNext;
                    cout << "What next? (q! to quit, c to continue heightmap grinding): ";
                    cin >> whatNext;
                    if ( whatNext == string ("q!")) {
                        isOptionValid = true;
                        doContinue = false;
                    } else if ( whatNext == string ("c")) {
                        isOptionValid = true;
                    } else {
                    }
                }

            } catch (const functional_general_exeption &e) {
                cerr << "there was some error in your function:\n   " << e.getMessage() << endl;
            }
            if (0 != screen) {
                SDL_FreeSurface (screen);
                screen = 0;
            }
            SDL_Quit();
        }
    }
    return 0;
}
