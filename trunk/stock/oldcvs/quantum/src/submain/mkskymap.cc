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


#include <stdio.h>
#include <ctype.h>

#include <iostream>

#include <SDL/SDL.h>

#include <picogen/picogen.h>
#include <picogen/graphics/objects/CloudAdapter.h>

namespace {

    using picogen::real;
    using picogen::geometrics::Vector3d;
    using picogen::geometrics::Transformation;
    using picogen::geometrics::Ray;
    using picogen::graphics::objects::Preetham;
    using picogen::graphics::objects::CloudAdapter;
    using picogen::graphics::objects::abstract::ISky;
    using picogen::graphics::color::Color;
    using picogen::misc::functional::Function_R2_R1;
    using picogen::misc::functional::Function_R2_R1_Refcounted;
    using boost::intrusive_ptr;
    
    struct PreethamParams {
        Vector3d sunDirection;
        real turbidity;
        real solidAngleFactor;
        
        Color atmosphereColorFilter, sunColor;
        
        bool fogHackEnable;
        real fogHackDensity;
        real fogHackMaxDist;
        
        bool falloffHackEnable;
        real falloffHackParameters [3];
        //real falloffHackMaxAngleFactor;
        //real falloffHackExponent;
        
        PreethamParams () 
        : sunDirection (1.0, 1.0, 0.0)
        , turbidity (2.1)        
        , solidAngleFactor (1.0)
        , atmosphereColorFilter (0.05,0.05,0.05)
        , sunColor (1500.0,1500.0,1500.0)
        , fogHackEnable (false)
        , fogHackDensity (0.001)
        , fogHackMaxDist (1000000.0)
        , falloffHackEnable (false)
        //, falloffHackMaxAngleFactor (1.5)
        //, falloffHackExponent (1.5)
        {
        }
    };


    real f2_to_hemisphere (real fx, real fy) {
        const real fz_sq = 1 - fx*fx - fy*fy;
        if (fz_sq < 0.0) {
            return -1;
        }
        return sqrt (fz_sq);
    }


    void
    draw (
        SDL_Surface *p_target,
        ISky &sky,
        float scale,
        float exp_tone,
        float saturation,
        bool disableSunColor
    ) {
        if (SDL_MUSTLOCK (p_target) && SDL_LockSurface (p_target) <0)
            return;
        int x,y;
        for (y=0; y<p_target->h; y++) {
            /// \todo FIX we are currently assuming a 32bit SDL buffer
            /// \todo get rid of pointer arithmetic
            Uint32 *bufp   = (Uint32*) p_target->pixels + y* (p_target->pitch>>2);
            const real fy = 1.0 - 2.0 * static_cast <real> (y) / static_cast <real> (p_target->h);


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
                //CloudAdapter (preetham, fun) (ray).saturate (Color(0,0,0), Color(1,1,1)).to_rgb (r,g,b);
                Color skyCol (0,0,0), sunCol (0,0,0);
                sky.shade (skyCol, ray);
                if (!disableSunColor) {
                    sky.sunShade (sunCol, ray);
                }
                (skyCol+sunCol).saturate (Color(0,0,0), Color(1,1,1)).to_rgb (r,g,b);

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



    int showPreviewWindow (intrusive_ptr<Function_R2_R1_Refcounted> function, const PreethamParams &preethamParams, int width, int height) {
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
        preetham.setTurbidity (preethamParams.turbidity);
        preetham.setSunSolidAngleFactor (preethamParams.solidAngleFactor);
        preetham.setColorFilter (preethamParams.atmosphereColorFilter);
        preetham.setSunColor (preethamParams.sunColor);
        preetham.setSunDirection (preethamParams.sunDirection.computeNormal());
        
        preetham.enableFogHack (preethamParams.fogHackEnable, preethamParams.fogHackDensity, preethamParams.fogHackMaxDist);
        
        preetham.enableSunFalloffHack (preethamParams.falloffHackEnable);
        //preetham.setSunFalloffMaxSolidAngleFactor (preethamParams.solidAngleFactor * preethamParams.falloffHackMaxAngleFactor);
        //preetham.setSunFalloffExponent (preethamParams.falloffHackExponent);
        preetham.setSunFalloffHackParameters (preethamParams.falloffHackParameters [0], preethamParams.falloffHackParameters [1], preethamParams.falloffHackParameters [2]);
        
        preetham.invalidate ();

        if (0 != function.get()) {
            CloudAdapter ca (preetham, function);
            draw (screen, ca, 1.0, 1.0, 1.0, true);
        } else {
            draw (screen, preetham, 1.0, 1.0, 1.0, false);
        }

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


#ifdef mkskymap_err_missing_arg
#error mkskymap_err_missing_arg already defined
#endif
#define mkskymap_err_missing_arg() \
    if (argc<=0) { \
        cerr << "error: no argument given to option: " << option << '\n'; \
        cerr << "type 'picogen mkskymap --help'" << endl; \
        return -1; \
    }

#ifdef mkskymap_err_not_enough_args
#error mkskymap_err_not_enough_args already defined
#endif
#define mkskymap_err_not_enough_args() \
    if (argc<=0) { \
        cerr << "error: not enough arguments given to option: " << option << '\n'; \
        cerr << "type 'picogen mkskymap --help'" << endl; \
        return -1; \
    }

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
    
    PreethamParams preethamParams;
    
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
                mkskymap_err_missing_arg();
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

                mkskymap_err_missing_arg();
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
                mkskymap_err_missing_arg();
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
                mkskymap_err_missing_arg();
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
                mkskymap_err_missing_arg();
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

            } else if (option == "-d" || option == "--direction") {
                
                real x, y, z;
                // Read x-direction.
                {
                    mkskymap_err_missing_arg();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> x;
                    argc--; argv++;
                }
                
                // Read y-direction.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> y;
                    argc--; argv++;
                }
                
                // Read z-direction.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> z;
                    argc--; argv++;
                }        
                
                preethamParams.sunDirection = Vector3d (x, y, z);

            } else if (option == "-C" || option == "--sun-color") {
                
                real r, g, b;
                // Read Red.
                {
                    mkskymap_err_missing_arg();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> r;
                    argc--; argv++;
                }
                
                // Read Green.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> g;
                    argc--; argv++;
                }
                
                // Read Blue.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> b;
                    argc--; argv++;
                }        
                
                preethamParams.sunColor.from_rgb (r, g, b);

            } else if (option == "-F" || option == "--color-filter") {
                
                real r, g, b;
                // Read Red.
                {
                    mkskymap_err_missing_arg();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> r;
                    argc--; argv++;
                }
                
                // Read Green.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> g;
                    argc--; argv++;
                }
                
                // Read Blue.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> b;
                    argc--; argv++;
                }
                
                preethamParams.atmosphereColorFilter.from_rgb (r, g, b);

            } else if (option == "-O" || option == "--fog-hack") {
                
                real density, maxRange;
                // Read Density.
                {
                    mkskymap_err_missing_arg();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> density;
                    argc--; argv++;
                }
                
                // Read Max-Range.
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> maxRange;
                    argc--; argv++;
                }
                
                preethamParams.fogHackEnable = true;
                preethamParams.fogHackDensity = density;
                preethamParams.fogHackMaxDist = maxRange;

            } else if (option == "-t" || option == "--turbidity") {                
                mkskymap_err_missing_arg();
                stringstream ss; 
                ss << string (argv[0]);
                ss >> preethamParams.turbidity;
                argc--; argv++;                
            } else if (option == "-A" || option == "--solid-angle") {                
                mkskymap_err_missing_arg();
                stringstream ss; 
                ss << string (argv[0]);
                ss >> preethamParams.solidAngleFactor;
                argc--; argv++;                
                
                /*
                preetham.enableSunFalloffHack (preethamParams.falloffHackEnable);
                preetham.setSunFalloffMaxSolidAngleFactor (preethamParams.solidAngleFactor * preethamParams.falloffHackMaxAngleFactor);
                */
            } else if (option == "-o" || option == "--falloff") {
                // Read Parameters.
                {
                    mkskymap_err_missing_arg();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> preethamParams.falloffHackParameters [0];
                    argc--; argv++;
                }
                
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> preethamParams.falloffHackParameters [1];
                    argc--; argv++;
                }
                
                {
                    mkskymap_err_not_enough_args();
                    stringstream ss; 
                    ss << string (argv[0]);
                    ss >> preethamParams.falloffHackParameters [2];
                    argc--; argv++;
                }
                
                preethamParams.falloffHackEnable = true;
            } else if (option == "--help") {
                printUsage();
                return 0;
            } else {
                cerr << "unknown option in argument list: " << option << endl;
                printUsage();
                return -1;
            }
        }

        if (0 != code.size() && lingua == Lingua_unknown) {
            cerr << "error: unkown language or wrong option used" << endl;
            printUsage();
            return -1;
        }


        intrusive_ptr<Function_R2_R1_Refcounted> fun (0 != code.size() ? new Function_R2_R1_Refcounted (code) : 0);

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
            return showPreviewWindow (fun, preethamParams, width, height);
        }

    } catch (const functional_general_exeption &e) {
        cerr << "error: " << e.getMessage() << endl;
        return -1;
    } catch (...) {
        cerr << "unknown exception." << endl;
    }
    return 0;
}

#undef mkskymap_err_missing_arg
#undef mkskymap_err_not_enough_args
