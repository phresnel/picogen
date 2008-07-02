/***************************************************************************
 *            ssdf.cc
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
#include <vector>

#include <SDL/SDL.h>

#include <picogen/picogen.h>
#include <picogen/picossdf/picossdf.h>
#include <picogen/graphics/objects/Instance.h>
#include <picogen/graphics/objects/templates/TriBIH.h>



using ::picogen::common::real;
using ::picogen::common::Vector3d;
using ::picogen::common::Ray;
using ::picogen::common::Color;
using ::picogen::common::intersection_t;

using ::picogen::common::Transformation;

using ::picogen::graphics::objects::templates::TriBIH;

using ::picogen::graphics::material::abstract::IBRDF;
using ::picogen::graphics::objects::abstract::IScene;
using ::picogen::graphics::objects::Sphere;


struct t_triangle {
    private:
        Vector3d t[3];
    public:
        t_triangle (Vector3d a, Vector3d b, Vector3d c) {
            t[0] = a;
            t[1] = b;
            t[2] = c;
        }
        t_triangle() {
        }

        const Vector3d & operator [] ( int i ) const {
            return t[i];
        }
};
typedef TriBIH<t_triangle> genBIH;
static genBIH myGenBIH;



class ConstantShader : public picogen::graphics::material::abstract::IShader {
        picogen::graphics::image::color::Color color;
    public:
        virtual ~ConstantShader() {};
        ConstantShader (picogen::graphics::image::color::Color color) : color (color) {}
        virtual void shade (
            picogen::graphics::image::color::Color &color,
            const picogen::misc::geometrics::Vector3d &normal,
            const picogen::misc::geometrics::Vector3d &position
        ) const {
            color = this->color;
        }
};
static const ConstantShader  red (picogen::graphics::image::color::Color (1.0, 0.3, 0.3));
static const ConstantShader  green (picogen::graphics::image::color::Color (0.3, 1.0, 0.3));
static const ConstantShader  blue (picogen::graphics::image::color::Color (0.3, 0.3, 1.0));
static const ConstantShader  white (picogen::graphics::image::color::Color (1.0, 1.0, 1.0));


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
            for (register int u=0; u<2; ++u) for (register int v=0; v<2; ++v) {
                    real r,g,b;
                    ( (Color) surface (x*2+u,y*2+v)).to_rgb (r, g, b);
                    //((xrt::base_types::color)surface((unsigned)x,(unsigned)y)).to_rgb( r, g, b );
                    // tone map

                    /*
                    const float brightness = 0.299*r + 0.584*g + 0.144*b;
                    const float e = 1-exp(-brightness * exp_tone );
                    r = (r/brightness) * e;
                    g = (g/brightness) * e;
                    b = (b/brightness) * e;
                    //*/

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





class Scene {
    public:
        virtual std::string getName() const = 0;
        virtual void initRenderer (int width, int height) = 0;
        virtual void flip (SDL_Surface *screen, float scale, float saturation) = 0;
        virtual void begin() = 0;
        virtual bool renderMore (int numPixels) = 0;  // i wanted to call it continue, but then continue is obviously a reserved word :P
        virtual void end() = 0;
};


class SSDFScene : public Scene, public SSDFBackend {
    protected:
        ::picogen::graphics::samplers::screen::XYIterator<
            ::picogen::misc::templates::surface<
                ::picogen::graphics::image::color::AverageColor
            >,
            ::picogen::graphics::cameras::FromPointToRect,
            ::picogen::graphics::samplers::ray::Simple
        > renderer;
        int width, height;

        typedef ::picogen::common::LinearList LinearList;

        LinearList list;
        ::picogen::common::Preetham   preetham;
        ::picogen::common::AABox      box;


        typedef ::picogen::graphics::objects::abstract::IScene IScene;

        enum SCENE_TYPE {
             LINEAR_LIST
        };

        struct Scene {
            union {
                LinearList *linearList;
            };
            SCENE_TYPE type;
        };
        typedef ::std::vector<Scene> SceneStack;

        Scene currentScene;
        SceneStack sceneStack;

        IScene *sceneRoot;
        IBRDF* currentBRDF;

        ::std::vector<IBRDF*> brdfStack;

        PicoSSDF ssdf;

    public:


        // --- SSDFBackend implementation -------------------------------
        virtual int beginGlobalBlock () {
            std::cout << "begin global" << std::endl;
            return 0;
        }
        virtual int endGlobalBlock () {
            std::cout << "end global" << std::endl;
            return 0;
        }


        virtual int beginListBlock () {
            std::cout << "begin list" << std::endl;
            currentScene.type = LINEAR_LIST;
            currentScene.linearList = new LinearList;
            if (0 == sceneRoot) {
                sceneRoot = currentScene.linearList;
            }
            sceneStack.push_back (currentScene);
            return 0;
        }
        virtual int endListBlock () {
            std::cout << "end list" << std::endl;
            currentScene.linearList->invalidate();
            sceneStack.pop_back();
            currentScene = sceneStack.back();
            return 0;
        }
        virtual int beginTriBIHBlock () {
            std::cout << "begin tri bih" << std::endl;
            return 0;
        }
        virtual int endTriBIHBlock () {
            std::cout << "end tri bih" << std::endl;
            return 0;
        }


        virtual int setBRDFToLambertian (::picogen::misc::prim::real reflectance) {
            currentBRDF = new ::picogen::graphics::material::brdf::Lambertian(reflectance);
            brdfStack.push_back (currentBRDF);
            return 0;
        }
        virtual int setBRDFToSpecular (::picogen::misc::prim::real reflectance) {
            currentBRDF = new ::picogen::graphics::material::brdf::Specular(reflectance);
            brdfStack.push_back (currentBRDF);
            return 0;
        }

        virtual int addSphereTerminal (
            ::picogen::misc::prim::real radius,
            const ::picogen::misc::geometrics::Vector3d &center,
            const ::picogen::graphics::image::color::Color &color
        ) {
            using namespace ::std;
            using namespace ::picogen::graphics::material::brdf;



            switch (currentScene.type) {
                case LINEAR_LIST:{
                    Sphere *sphere = new Sphere;
                    sphere->setColor (color);
                    sphere->setRadius (radius);
                    sphere->setPosition (center);
                    sphere->setBRDF (currentBRDF);
                    currentScene.linearList->insert (sphere);
                    // TODO URGENT !!!!one1 --> see LinearList.cc
                }break;
            };
            return 0;
        }
        // --------------------------------------------------------------


        explicit SSDFScene(const std::string &filename)
        : sceneRoot(0), currentBRDF(0), ssdf(filename, this) {
            box.enableOutside (false);
        }



        virtual ~SSDFScene() {
            if (0 != sceneRoot)
                delete sceneRoot;

            ::std::vector<IBRDF*>::iterator it;
            for (it=brdfStack.begin(); it!=brdfStack.end(); ++it) {
                delete *it;
            }
            brdfStack.clear();
        }



        virtual std::string getName() const {
            return std::string ("pure-cornell");
        }



        virtual void initRenderer (int width, int height) {
            using namespace picogen;

            this->width  = width;
            this->height = height;
            // setup screen and camera
            renderer.camera().defineCamera ( (real) width/ (real) height, 1.0, 0.5);
            renderer.surface().reset (width*2, height*2);

            // setup camera transform
            renderer.transformation() =
                Transformation().setToRotationX (3.14159*0.0) *
                Transformation().setToRotationY (3.14159*0.0) *
                Transformation().setToTranslation (Vector3d (0.0,0.0,-1.5) *1.0)
                ;

            // setup and recognize sky
            preetham.setTurbidity (1.9);
            preetham.setSunSolidAngleFactor (1.0);
            const real L = 0.25;
            preetham.setColorFilter (Color (1.0,1.0,1.0) *1.0*L);
            preetham.setSunColor (Color (1.0,0.9,0.8) *1600.0*L);
            preetham.setSunDirection (Vector3d (-1.7,2.5,3.3).normal());
            preetham.enableFogHack (1, 0.00082*0.05, 500000);
            preetham.invalidate();
            renderer.path_integrator().setSky (&preetham);

            // setup boxen
            box.enableFace (box.y_positive, false);

            // recognize scene
            /*list.insert (&box);
            list.insert (sceneRoot);
            // invalidate and recognize object-list
            list.invalidate();*/
            if (0 == sceneRoot) { // Well, we allow to just render atmosphere. So we put in a dummy list.
                sceneRoot = new LinearList;
                sceneRoot->invalidate ();
            }
            renderer.path_integrator().setIntersectable (sceneRoot);
        }

        virtual void flip (SDL_Surface *screen, float scale, float saturation) {
            ::SDL_Flip (screen);
            ::draw (screen,renderer.surface(), scale, 1.0, saturation);//scale, exp_tone, saturation);
        }

        virtual void begin() {
            renderer.beginRender();
        }

        virtual bool renderMore (int numPixels) {
            renderer.setNumPixelsPerContinue (numPixels);
            bool c = renderer.renderMore();
            if (c) {
                renderer.oneMoreRun();
            }
            return c;
        }

        virtual void end() {
        }
};



static int loop (SDL_Surface *screen, Scene *scene, int width, int height) {
    //scene->flip( screen );
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    // prepare and run loop
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    unsigned int pixelsPerContinue = (width*height*4) /32;
    unsigned int runCount = 1;
    //clock_t startTime = clock();
    bool done = false;
    bool queryingForReallyQuit = false;
    unsigned int numManDumbs = 0;
    float scale = 1;
    float exp_tone = 1.0;
    float saturation = 1.0;

    srand (time (NULL));


    //renderer.SetNumPixelsPerContinue( pixelsPerContinue );//*(HEIGHT/1) );
    //renderer.BeginRender();
    scene->begin();
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent (&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (queryingForReallyQuit) {
                    if (event.key.keysym.sym == SDLK_y) {
                        done = true;
                        queryingForReallyQuit = false;
                    } else if (event.key.keysym.sym == SDLK_n) {
                        done = false;
                        queryingForReallyQuit = false;
                    }
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    queryingForReallyQuit = 1;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    char s[256];
                    sprintf (s, "screendump.%.4d.bmp", numManDumbs++);
                    SDL_SaveBMP (screen, s);
                }
                if (event.key.keysym.sym == SDLK_y) {
                    pixelsPerContinue-=100*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if (event.key.keysym.sym == SDLK_x) {
                    pixelsPerContinue-=10*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if (event.key.keysym.sym == SDLK_c) {
                    pixelsPerContinue-=width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if (event.key.keysym.sym == SDLK_v) {
                    pixelsPerContinue+=width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if (event.key.keysym.sym == SDLK_b) {
                    pixelsPerContinue+=10*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if (event.key.keysym.sym == SDLK_n) {
                    pixelsPerContinue+=100*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }

                if (event.key.keysym.sym == SDLK_5) {
                    scale = 1.0;
                }
                if (event.key.keysym.sym == SDLK_6) {
                    scale += 0.01;
                }
                if (event.key.keysym.sym == SDLK_7) {
                    scale += 0.1;
                }
                if (event.key.keysym.sym == SDLK_8) {
                    scale += 1.0;
                }
                if (event.key.keysym.sym == SDLK_9) {
                    scale += 10.0;
                }
                if (event.key.keysym.sym == SDLK_4) {
                    scale -= 0.01;
                }
                if (event.key.keysym.sym == SDLK_3) {
                    scale -= 0.1;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    scale -= 1.0;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    scale -= 10.0;
                }

                if (event.key.keysym.sym == SDLK_t) {
                    exp_tone = 1.0;
                }
                if (event.key.keysym.sym == SDLK_z) {
                    exp_tone += 0.01;
                }
                if (event.key.keysym.sym == SDLK_u) {
                    exp_tone += 0.1;
                }
                if (event.key.keysym.sym == SDLK_i) {
                    exp_tone += 1.0;
                }
                if (event.key.keysym.sym == SDLK_o) {
                    exp_tone += 10.0;
                }
                if (event.key.keysym.sym == SDLK_r) {
                    exp_tone -= 0.01;
                }
                if (event.key.keysym.sym == SDLK_e) {
                    exp_tone -= 0.1;
                }
                if (event.key.keysym.sym == SDLK_w) {
                    exp_tone -= 1.0;
                }
                if (event.key.keysym.sym == SDLK_q) {
                    exp_tone -= 10.0;
                }

                if (event.key.keysym.sym == SDLK_g) {
                    saturation -= 0.1;
                }
                if (event.key.keysym.sym == SDLK_h) {
                    saturation -= 0.01;
                }
                if (event.key.keysym.sym == SDLK_j) {
                    saturation = 1.0;
                }
                if (event.key.keysym.sym == SDLK_k) {
                    saturation += 0.01;
                }
                if (event.key.keysym.sym == SDLK_l) {
                    saturation += 0.1;
                }

            }
        }

        bool cont = scene->renderMore (pixelsPerContinue);
        if (cont) {
            runCount++;
            //
            //renderer.OneMoreRun();
            //if( 0 == (runCount%3) )
            //    renderer.surface().saveBinaryToFile( checkpointFile );
        } else if (queryingForReallyQuit) {
            SDL_WM_SetCaption ("Really Quit? (Y/N) ", "Really Quit? (Y/N) ");
        } else {
            char s[256];
            //float t = (static_cast<float>(clock()-startTime))/static_cast<float>(CLOCKS_PER_SEC);
            //sprintf( s, "%uspp:%ux%u/%.2fsec/color{scale%.2f", runCount, WIDTH, HEIGHT , t, scale );
            sprintf (s, "%uspp;color.{*%.2f,t%.2f,s%.2f};refresh{%.2fl}", runCount, scale, exp_tone, saturation, (float) pixelsPerContinue/ (float) width);
            fprintf (stderr, "\rrendering pass %u (%s)", 1+runCount, s);
            SDL_WM_SetCaption (s, "...");
        }

        //scene->flip( screen );
        scene->flip (screen, scale, saturation);
        //draw(screen,renderer.surface(),scale, exp_tone, saturation);
        /*unsigned int numSamples;
        for( numSamples=0; numSamples<10000; numSamples ++ ){
            unsigned int u = rand() % surf.width();
            unsigned int v = rand() % surf.height();
            if( (rand()%500000) > u*u+v*v )
                surf(u,v) += base_types::color(1,1,1);
        }*/
    }
    scene->end();
    return 0;
}

static int grind (int width, int height, Scene *scene) {
    using namespace std;
    scene->initRenderer (width, height);

    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        cerr << "Unable to init SDL: " << SDL_GetError() << endl;
        return 1;
    }
    atexit (SDL_Quit);
    SDL_Surface *screen = SDL_SetVideoMode (width,height,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (NULL == screen) {
        cerr << "Unable to set video-mode: " << SDL_GetError() << endl;
        return 1;
    }

    //scene->flip();
    return loop (screen, scene, width, height);  /// \todo check if SQL cleans up surface resource
}



static void printUsage() {
}



int main_ssdf (int argc, char *argv[]) {
    using namespace std;

    if (argc == 0) {
        cerr << "hmpf no filename" << endl;
        printUsage();
        return -1;
    }


    //++ Options.
    string filename ("");
    //-- Options.

    const string primary = argv[0];
    argc--;
    argv++;

    if (primary == string ("-f") || primary == string ("--filename")) {
        if (argc == 0) {   // check if any argument is given, we need at least one remaining
            printUsage();
            return -1;
        }
        const std::string secondary = argv [0];
        filename = argv [0];
    } else {
        cerr << "Parameter '" << primary << "' unknown." << endl;
        return -1;
    }

    Scene *grindScene;
    try {
        grindScene = new SSDFScene (filename);
        return grind (320, 320, grindScene);
    } catch (PicoSSDF::exception_file_not_found e) {
        cerr << "doh, exception_file_not_found." << endl;
    } catch (PicoSSDF::exception_unknown e) {
        cerr << "doh, exception_unknown." << endl;
    } catch (PicoSSDF::exception_syntax_error e) {
        cerr << "" << e.file << ""
             << ":" << e.line
             << ":error:" << e.reason << ""
             //<< " (near \"" << e.curr << "\")"
             << endl;
    } catch (...) {
        cerr << "doh, ..." << endl;
    }
    return 0;
}
