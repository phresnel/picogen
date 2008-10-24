/***************************************************************************
 *            mkskymap.cc
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
/// \todo include RAW-export option
/// \todo include PNG-export option
/// \todo include more visualisation modes than only greyscale
/// \todo include an option to only display a subset of the map
/// \todo include an option to zoom in/out
/// \todo include ram-less heightmap creation (will be slower when using normalization)
/// \todo include an option to sequentially save the heightmap, one chunk by another
/// \todo include options to set the x/y-center on the height-function
/// \todo This actually a project global todo: get rid of stuff like FLT_MAX, we want our project be portable.


#include <stdio.h>
#include <ctype.h>

#include <iostream>

#include <SDL/SDL.h>

#include <picogen/picogen.h>

namespace {

    using picogen::real;
    using picogen::geometrics::Vector3d;
    using picogen::geometrics::Transformation;
    using picogen::geometrics::Ray;
    using picogen::graphics::objects::Preetham;
    using picogen::graphics::color::Color;
    using picogen::misc::functional::Function_R2_R1;
    using picogen::misc::functional::Function_R2_R1;


    real f2_to_hemisphere (real fx, real fy) {
        const real fz_sq = 1 - fx*fx - fy*fy;
        if (fz_sq < 0.0) {
            return -1;
        }
        return sqrt (fz_sq);
    }

    class CloudAdapter {
        private:
            const Preetham &preetham;
            Function_R2_R1 &fun;
            static real sat01 (real f);

        public:
            CloudAdapter (const Preetham &preetham, Function_R2_R1 &fun);
            Color operator () (const Ray &ray) const;
    };

    CloudAdapter::CloudAdapter (const Preetham &preetham, Function_R2_R1 &fun)
    : preetham (preetham), fun (fun) {
    }


    real CloudAdapter::sat01 (real f) {
        return f < 0 ? 0 : f > 1 ? 1 : f;
    }

    Color CloudAdapter::operator () (const Ray &ray) const {
        #if 0
            const real v = (real)y / (real)p_target->h;
            const real u = (real)x / (real)p_target->w;
            const real rs = 1;
        #else
            const real cloudPlaneHeight = 0.25;
            const real rs = (cloudPlaneHeight - ray.getPosition() [1]) / ray.getDirection() [1];
            const real u = ray (rs) [0];
            const real v = ray (rs) [2];
        #endif

        const real fx = u;
        const real fy = v;


        //++
        const real si = 0.05 * rs * rs;
        Vector3d U1 (fx-si, 0 , fy);
        U1 = U1 + Vector3d (0, f2_to_hemisphere (U1[0], U1[2]), 0);
        Vector3d U2 (fx+si, 0 , fy);
        U2 = U2 + Vector3d (0, f2_to_hemisphere (U2[0], U2[2]), 0);

        Vector3d V1 (fx, 0 , fy-si);   V1 = V1 + Vector3d (0, f2_to_hemisphere (V1[0], V1[2]), 0);

        Vector3d V2 (fx, 0 , fy+si);   V2 = V2 + Vector3d (0, f2_to_hemisphere (V2[0], V2[2]), 0);

        const Vector3d diff [2] = {
            (U2 - U1).computeNormal (),
            (V2 - V1).computeNormal ()
        };

        const real h = sat01 (fun (u, v));
        const real hu1 = sat01 (fun (u-si, v));
        const real hu2 = sat01 (fun (u+si, v));
        const real hv1 = sat01 (fun (u, v-si));
        const real hv2 = sat01 (fun (u, v+si));

        // compute tangent vectors
        const real H = 0.025;
        const Vector3d U = /*Vector3d (si,0,0).computeNormal(); //*/ Vector3d (si, H*(hu2-hu1), 0).computeNormal();
        const Vector3d V = /*Vector3d (0,0,si).computeNormal(); //*/ Vector3d (0, H*(hv2-hv1), si).computeNormal();
        //--


        // cloud's surface normal
        const Vector3d normal_ = U.computeCross (V).computeNormal();

        // transform normal
        const Vector3d normal =
            diff[0]*normal_[0]
            +ray.getDirection()*-normal_[1]
            +diff[1]*normal_[2]
        ;//*/


        // compute sunsky color
        Color skyColor, sunColorInRay;
        preetham.shade (skyColor, ray);
        preetham.sunShade (sunColorInRay, ray);
        const Color sunSky = sunColorInRay + skyColor;

        Color sunColor; preetham.getSunColor (sunColor);

        Vector3d sunDirection; preetham.getSunDirection (sunDirection);
        const real dot = (normal * sunDirection);
        const real dot_ray_sun = (ray.w() * sunDirection);

        // compute diffuse light
        real diffuse_r=0, diffuse_g=0, diffuse_b=0;
        real diffuseDot;
        {
            const real diffuseFac = 0.05;
            diffuseDot = -dot;
            diffuseDot = (diffuseDot*0.5 + 0.5); // [0..1]
            //diffuseDot = diffuseDot*0.75 + 0.25;   // [0.25..1]
            diffuseDot = diffuseDot<0?0:diffuseDot>1?1:diffuseDot;
            diffuseDot = pow (diffuseDot, 1.0);
            (sunColor * diffuseDot * diffuseFac).to_rgb (diffuse_r, diffuse_g, diffuse_b);
        }

        // compute 1-scatter light
        real s1_light_r=0, s1_light_g=0, s1_light_b=0;
        if (true) {
            real s = dot_ray_sun;
            s = s<0?0:s>1?1:s;
            s = pow (s, 64); // reduce sun hemispherical area, hmm.
            s = s / (1+pow(h*30,4));
            s *= 4.0;
            Color s1 = sunColor * s;
            s1.to_rgb (s1_light_r, s1_light_g, s1_light_b);
        }

        // ambient
        real ambient_r = 0.5;
        real ambient_g = 0.5;
        real ambient_b = 0.5;

        real r, g, b;
        (sunSky).to_rgb (r,g,b);

        switch (0) {
            case 0: {
                const real h_ = h;

                r = r * (1-h_)/**/ + (diffuse_r + s1_light_r + ambient_r)*h_;
                g = g * (1-h_)/**/ + (diffuse_g + s1_light_g + ambient_g)*h_;
                b = b * (1-h_)/**/ + (diffuse_b + s1_light_b + ambient_b)*h_;
            } break;
            case 21:
                r = normal[0]*0.5+0.5;
                g = normal[1]*0.5+0.5;
                b = normal[2]*0.5+0.5;
                break;
            case 22:
                r = normal_[0]*0.5+0.5;
                g = normal_[1]*0.5+0.5;
                b = normal_[2]*0.5+0.5;
                break;
            case 231:
                r = diff[0][0]*0.5+0.5;
                g = diff[0][1]*0.5+0.5;
                b = diff[0][2]*0.5+0.5;
                break;
            case 232:
                r = diff[1][0]*0.5+0.5;
                g = diff[1][1]*0.5+0.5;
                b = diff[1][2]*0.5+0.5;
                break;
            case 3:
                r = g = b = dot;
                break;
            case 4:
                r = g = b = h;
                break;
            case 5:
                r = g = b = rs;
                break;
            case 6:
                r = g = b = diffuseDot;
                break;
        };
        return Color (r, g, b);
    }


    void
    draw (
        SDL_Surface *p_target,
        Function_R2_R1 &fun,
        float scale,
        float exp_tone,
        float saturation,
        const Preetham &preetham
    ) {
        if (SDL_MUSTLOCK (p_target) && SDL_LockSurface (p_target) <0)
            return;
        int x,y;
        for (y=0; y<p_target->h; y++) {
            /// \todo FIX we are currently assuming a 32bit SDL buffer
            /// \todo get rid of pointer arithmetic
            Uint32 *bufp   = (Uint32*) p_target->pixels + y* (p_target->pitch>>2);
            const real fy = -1.0 + 2.0 * static_cast <real> (y) / static_cast <real> (p_target->h);


            for (x=0; x<p_target->w; x++) {

                const real fx = -1.0 + 2.0 * static_cast <real> (x) / static_cast <real> (p_target->w);

                Ray ray;
                ray.setPosition (Vector3d (0.0, 0.0, 0.0));
                const real fz = f2_to_hemisphere (fx, fy);
                if (fz < 0.0) {
                    * (bufp++) = SDL_MapRGB (p_target->format, 0, 0, 0);
                    continue;
                }

                ray.setDirection (Vector3d (fx, fz, fy));

                real r, g, b;
                CloudAdapter (preetham, fun) (ray).saturate (Color(0,0,0), Color(1,1,1)).to_rgb (r,g,b);

                * (bufp++) =
                    SDL_MapRGB (p_target->format,
                                (unsigned char) (255.0*r),
                                (unsigned char) (255.0*g),
                                (unsigned char) (255.0*b)
                               );
            }
        }


        if (SDL_MUSTLOCK (p_target))
            SDL_UnlockSurface (p_target);
        SDL_Flip (p_target);
    }



    int showPreviewWindow (Function_R2_R1 &function, int width, int height) {
        using namespace std;

        if (SDL_Init (SDL_INIT_VIDEO) < 0) {
            cerr << "Unable to init SDL for preview: " << SDL_GetError() << endl;
            return -1;
        }
        atexit (SDL_Quit);
        SDL_Surface *screen = SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE);
        if (0 == screen) {
            cerr << "Unable to set video-mode for preview: " << SDL_GetError() << endl;
            return -1;
        }

        Preetham preetham;
        preetham.setTurbidity (2.4);
        preetham.setSunSolidAngleFactor (1.0);
        const real L = 0.035;
        Color filter = Color (1.0,1.0,1.0);
        preetham.setColorFilter (filter *1.0*L);
        preetham.setSunColor (filter *1000.0*L);
        preetham.setSunDirection (Vector3d (1.0,1.0,0.0).normal());
        preetham.enableFogHack (false, 0.0, 99999999.0);
        preetham.invalidate ();

        // dump the heightmap onto the screen
        draw (screen, function, 1.0, 1.0, 1.0, preetham);

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



    template <typename T> std::string exportText (T &heightmap, const std::string &outputFilename, bool forceOverwrite) {
        using namespace std;

        // Check if file already exists.
        if (!forceOverwrite) {
            FILE *f = fopen (outputFilename.c_str(), "r");
            if (0 != f) {
                fclose (f);
                return "File '" + outputFilename + "' already exists. Remove it or invoke the -f option to force overwrite.";
            }
        }

        // Open file for writing.
        FILE *f = fopen (outputFilename.c_str(), "w");
        if (0 == f) {
            return "Could not open '" + outputFilename + "' for writing.";
        }

        fprintf (f, "width:%u\nheight:%u\nvalues:",
            static_cast<unsigned int> (heightmap.width),
            static_cast<unsigned int> (heightmap.height)
        );

        for (unsigned int y=0; y<heightmap.height; ++y) {
            for (unsigned int x=0; x<heightmap.width; ++x) {
                const float h = static_cast<float> (heightmap(x,y));
                fprintf (f, "%f ", h);
            }
        }

        fclose (f);
        return "";
    }



    template <typename T> std::string exportWinBMP (T &heightmap, const std::string &outputFilename, bool forceOverwrite) {
        using namespace std;

        // Check if file already exists.
        if (!forceOverwrite) {
            FILE *f = fopen (outputFilename.c_str(), "r");
            if (0 != f) {
                fclose (f);
                return "File '" + outputFilename + "' already exists. Remove it or invoke the -f option to force overwrite.";
            }
        }

        if (SDL_Init (SDL_INIT_VIDEO) < 0) {
            return "Unable to init SDL for bitmap export: " + string (SDL_GetError());
        }

        atexit (SDL_Quit);
        SDL_Surface *surface = SDL_CreateRGBSurface (SDL_SWSURFACE, heightmap.width, heightmap.height, 32,
            0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
        if (0 == surface) {
            return "Unable to create surface for bitmap export: " + string (SDL_GetError());
        }

// TODO IMPORTANT
        //draw (surface, heightmap, 1.0, 1.0, 1.0, -10000000);
        SDL_SaveBMP (surface, outputFilename.c_str());
        SDL_FreeSurface (surface);
        SDL_Quit();
        return "";
    }



    static void printUsage() {
        std::cout
            << "Invocation of mkskymap: picogen mkskymap [options]\n"
            << "In the version of picogen you have installed, the following options are possible:\n"
            << "\n"
            << "-Lhs / --Lheight-slang <program : string>:  -define an input program in \n"
            << "                                             height-slang syntax.\n"
            << "                                             (see http://picogen.org)\n"
            << " -En / --ExportName <name : string>:        -define a basis-name for exported \n"
            << "                                             files. \n"
            << "                                             default is 'mkskymap-out'\n"
            << "-Et / --Etext:                              -export as plaintext. \n"
            << "                                             the format is self-explanatory.\n"
            << "-Ebmp / --Ebitmap:                          -export as standard bmp file.\n"
            << "-p / --preview:                             -show a window with the heightmap.\n"
            //<< "-l / --preview-water-level <L : floating point>:-set a water level for preview\n"
            //<< "                                             (only useful together with \n"
            //<< "                                              --preview)\n"
            << "-f / --force-overwrite:                     -force overwrite of existing files.\n"
            << "-w / --width <positive integer>:            -set target width, in pixels. \n"
            << "                                             standard is 512.\n"
            << "-h / --height <positive integer>:           -set target height, in pixels. \n"
            << "                                             standard is 512.\n"
            //<< "-a / --anti-aliasing <X : positive integer>:-enable X*X antialiasing.\n"
            << "                                             standard is 1.\n"
            << "-W / --domain-width <S : floating point>:   -scales the input coordinates from \n"
            << "                                             [0..1)x[0..1) to [0..S)x[0..S)\n"
            //<< "-n / --normalize :                          -enable normalization, that is, \n"
            //<< "                                             scale the height-values linearly \n"
            << "                                             so that all values are in [0..1)\n"
            << "--help : show this help and exit"
        << std::endl;
    }
};

#ifdef MKSKYMAP_STANDALONE
int main (int argc, char *argv[]) {
#else
int main_mkskymap (int argc, char *argv[]) {
#endif
    --argc;
    ++argv;

    using namespace std;
    using namespace picogen::misc::functional;

    if (argc<=0) {
        cerr << "error: no arguments given to `picogen mkskymap`" << endl;
        printUsage();
        return -1;
    }

    /// \todo MINOR Someone please cleanup the following line.
    typedef enum Lingua_t {
        Lingua_inlisp,
        Lingua_unknown
    } Lingua;
    std::string code("");
    Lingua lingua = Lingua_unknown;
    bool showPreview = false;
    unsigned int width=512, height=512;
    float scaling = 1.0;
    //float waterLevel = -10000000.0;
    unsigned int antiAliasFactor = 1;
    //float heightmapNormalizationAccuracy = -1.0;
    //bool doNormalize = false;
    //bool doPrintInfo = false;
    string exportFileNameBase ("mkskymap-out");
    bool forceOverwriteFiles = false;

    struct ExportFlags {
        bool text : 1;
        bool winBMP : 1;
        ExportFlags ()
        : text(false), winBMP(false)
        {}
    };

    ExportFlags exportFlags;
    try {
        while (argc>0) {
            const std::string option (argv[0]);
            argc--;
            argv++;

            /// \todo add an option to specifiy the output-filename

            // -Lx
            if (option[1] == 'L' && Lingua_unknown != lingua) {
                // We already have read some code, more is not allowed.
                cerr << "error: only one formula or program allowed" << endl;
                printUsage();
                return -1;
            }
            if (option == "-Lhs" || option == "--Lheight-slang") {
                lingua = Lingua_inlisp;
                if (argc<=0) {
                    cerr << "error: no argument given to option: " << option << endl;
                    printUsage();
                    return -1;
                }
                code = string (argv[0]);
                argc--;
                argv++;
            } else if (option == "-En" || option == "--ExportName") {
                if (argc<=0) {
                    cerr << "error: no argument given to option: " << option << endl;
                    printUsage();
                    return -1;
                }
                exportFileNameBase = string (argv[0]);
                argc--;
                argv++;
            } else if (option == "-Et" || option == "--Etext" ) {
                    exportFlags.text = true;
            } else if (option == "-Ebmp" || option == "--Ebitmap" ) {
                    exportFlags.winBMP = true;
            } else if (option == "-p" || option == "--preview") {
                showPreview = true;
            } else if (option == "-f" || option == "--force-overwrite") {
                forceOverwriteFiles = true;
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

            } else if (option == "--help") {
                printUsage();
                return 0;
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





        if (code.size() == 0) {
            functional_general_exeption ("you gave me no code");
        }

        //Heightmap<double> heightmap (scaling, width, height);
        Function_R2_R1 *fun = new Function_R2_R1 (code);
        /*heightmap.fill (fun, antiAliasFactor);
        if (doNormalize)
            heightmap.normalize();

        if (doPrintInfo)
            heightmap.printInfo();*/
/*
        #define MKSKYMAP_EXPORT(FLAG,EXT,FUN)                                    \
        if (exportFlags.FLAG) {                                                     \
            const string outputFilename = exportFileNameBase + string(EXT);         \
            cerr << "Exporting as text to file '" + outputFilename + "' ...";       \
            std::string s = FUN (heightmap, outputFilename, forceOverwriteFiles);   \
            if (s != string("")) {                                                  \
                cerr << "\nerror: " << s << endl;                                   \
            } else {                                                                \
                cerr << "okay." << endl;                                            \
            }                                                                       \
        }

        MKSKYMAP_EXPORT (text, ".txt", exportText );
        MKSKYMAP_EXPORT (winBMP, ".bmp", exportWinBMP );
*/

        if (showPreview) {
            return showPreviewWindow (*fun, width, height);
        }
        delete fun;

    } catch (const functional_general_exeption &e) {
        cerr << "error: " << e.getMessage() << endl;
        return -1;
    } catch (...) {
        cerr << "unknown exception." << endl;
    }
    return 0;
}
