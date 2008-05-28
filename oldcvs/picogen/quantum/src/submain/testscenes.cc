/***************************************************************************
 *            testscenes.cc
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
#include <picogen/picogen.h>
#include <SDL/SDL.h>


using picogen::misc::prim::real;
using picogen::misc::geometrics::Vector3d;
using picogen::misc::geometrics::Ray;
using picogen::graphics::image::color::Color;
using picogen::graphics::structs::intersection_t;



class ConstantShader : public picogen::graphics::material::abstract::IShader {
    picogen::graphics::image::color::Color color;
public:
    virtual ~ConstantShader() {};
    ConstantShader( picogen::graphics::image::color::Color color ) : color(color) {}
    virtual void Shade(
        picogen::graphics::image::color::Color &color,
        const picogen::misc::geometrics::Vector3d &normal,
        const picogen::misc::geometrics::Vector3d &position
    ) const {
        color = this->color;
    }
};
static const ConstantShader  red  ( picogen::graphics::image::color::Color( 1.0, 0.3, 0.3 ) );
static const ConstantShader  green( picogen::graphics::image::color::Color( 0.3, 1.0, 0.3 ) );
static const ConstantShader  blue ( picogen::graphics::image::color::Color( 0.3, 0.3, 1.0 ) );
static const ConstantShader  white( picogen::graphics::image::color::Color( 1.0, 1.0, 1.0 ) );


struct Lambertian : public picogen::graphics::material::abstract::IBRDF {
    real mirror;

    Lambertian() : mirror(0) {}
    Lambertian( real m ) : mirror(m) {}

    virtual bool RandomSample(
        param_out(real,brdf),
        param_out(real,p),
        param_out(bool,specular),
        param_out(Ray,r_out),
        param_in (Ray,r_in),
        param_in (Vector3d,N)
    ) const {
        using picogen::misc::constants::pi;
        /*if( (static_cast<real>( rand() % 10000 ) / 10000.0)>0.9 )
        	return false;*/
        r_out.x() = r_in.x();
        p = 1.0;
        brdf = 1;
        if ( ((rand()%10000)/10000.0) < mirror ) {
            r_out.w() = r_in.w() - N.normal()*2*(r_in.w()*N.normal());

            p = fabs(N.normal()*r_out.w().normal());
            brdf = 1.0;
            specular = true;
        } else {
            do {
                r_out.w() = Vector3d(
                                static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
                                static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
                                static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0
                            );
            } while ( r_out.w().lengthSq()>1 || N*r_out.w()<0.0 );

            r_out.w() = r_out.w().normal();
            p = (1.0-mirror)/(2.0*pi);// / xrt::constants::pi;
            brdf = 1.0/pi;// / xrt::constants::pi;//r_out.w().normal() * N;// / xrt::constants::pi;
            specular = false;
        }
        return true;
    }
};


template<class t_surface>
void
draw(
    SDL_Surface *p_target,
    const t_surface &surface,
    float scale,
    float exp_tone,
    float saturation
) {
    if ( SDL_MUSTLOCK(p_target) && SDL_LockSurface(p_target)<0 )
        return;
    int x,y;
    for ( y=0; y<p_target->h; y++ ) {
        // TODO FIX we are currently assuming a 32bit SDL buffer
        // TODO get rid of pointer arithmetic
        Uint32 *bufp   = (Uint32*)p_target->pixels + y*(p_target->pitch>>2);
        for ( x=0; x<p_target->w; x++ ) {
            /*float d = 1.0 / source->density;
            float r = source->color[0]*scale*d;
            float g = source->color[1]*scale*d;
            float b = source->color[2]*scale*d;*/
            real accu_r=real(0), accu_g=real(0), accu_b=real(0);
            for ( register int u=0; u<2; ++u ) for ( register int v=0; v<2; ++v ) {
                    real r,g,b;
                    ((Color)surface(x*2+u,y*2+v)).to_rgb( r, g, b );
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
                    r = saturation*r + (1-saturation)*sbrightness;
                    g = saturation*g + (1-saturation)*sbrightness;
                    b = saturation*b + (1-saturation)*sbrightness;
                    // final saturation
                    accu_r += r<0 ? 0 : r>1 ? 1 : r;
                    accu_g += g<0 ? 0 : g>1 ? 1 : g;
                    accu_b += b<0 ? 0 : b>1 ? 1 : b;
                }
            accu_r *= 0.25;
            accu_g *= 0.25;
            accu_b *= 0.25;

            *(bufp++) =
                SDL_MapRGB(p_target->format,
                           (unsigned char)(255.0*accu_r),
                           (unsigned char)(255.0*accu_g),
                           (unsigned char)(255.0*accu_b)
                          );
        }
    }


    if ( SDL_MUSTLOCK(p_target) )
        SDL_UnlockSurface(p_target);
    SDL_Flip(p_target);
}





class TestScene {
public:
    virtual std::string getName() const = 0;
    virtual void initRenderer( int width, int height ) = 0;
    virtual void flip( SDL_Surface *screen, float scale, float saturation ) = 0;
    virtual void begin() = 0;
    virtual bool renderMore( int numPixels ) = 0; // i wanted to call it continue, but then continue is obviously a reserved word :P
    virtual void end() = 0;
};


#include <picogen/graphics/objects/Instance.h>

class PureCornell : public TestScene {
protected:
    picogen::graphics::samplers::screen::XYIterator<
    picogen::misc::templates::surface<
    picogen::graphics::image::color::AverageColor
    >,
    picogen::graphics::cameras::FromPointToRect,
    picogen::graphics::samplers::ray::Simple
    > renderer;
    int width, height;
    picogen::gob::LinearList list;
    picogen::gob::Preetham   preetham;
    picogen::gob::AABox      box, innerbox;
    picogen::gob::Instance   box1;

public:

    explicit PureCornell() : box1(&innerbox) {
        box.enableOutside( false );
    }
    virtual std::string getName() const {
        return std::string( "pure-cornell" );
    }

    virtual void initRenderer( int width, int height ) {
        using namespace picogen;

        this->width  = width;
        this->height = height;
        // setup screen and camera
        renderer.camera().DefineCamera( (real)width/(real)height, 1.0, 0.5 );
        renderer.surface().reset( width*2, height*2 );

        // setup camera transform
        renderer.transformation() =
            geom::Transformation().setToRotationX( 3.14159*0.0 ) *
            geom::Transformation().setToRotationY( 3.14159*0.0 ) *
            geom::Transformation().setToTranslation( Vector3d(0.0,0.0,-1.5)*1.0 )
            ;

        // setup and recognize sky
        preetham.SetTurbidity( 2.3 );
        preetham.SetSunSolidAngleFactor(1.0);
        const prim::real L = 0.25;
        preetham.SetColorFilter( Color(1.0,1.0,1.0)*1.0*L );
        preetham.SetSunColor( Color(0.95,0.95,1.0)*3000.0*L);
        preetham.SetSunDirection( Vector3d(-1.0,1.8,-2.3).normal() );
        preetham.EnableFogHack( 0, 0.00082*0.05, 50000 );
        preetham.Invalidate();
        renderer.path_integrator().SetSky( &preetham );

        // setup boxen
        using picogen::misc::geometrics::Transformation;
        innerbox.setShader( &white );
        box1.setTransform(
            Transformation().setToScaling(1.0/0.5)
            //* Transformation().setToRotationY(0.3)
            * Transformation().setToTranslation( Vector3d(0.0,1.0,0.0) )
        );

        // recognize scene
        list.Insert( &box );
        //list.Insert( &box1 );

        // invalidate and recognize object-list
        list.Invalidate();
        renderer.path_integrator().SetIntersectable( &list );
    }

    virtual void flip( SDL_Surface *screen, float scale, float saturation ) {
        ::SDL_Flip(screen);
        ::draw(screen,renderer.surface(), scale, 1.0, saturation );//scale, exp_tone, saturation);
    }

    virtual void begin() {
        renderer.BeginRender();
    }

    virtual bool renderMore( int numPixels ) {
        renderer.SetNumPixelsPerContinue( numPixels );
        bool c = renderer.Continue();
        if ( c ) {
            renderer.OneMoreRun();
        }
        return c;
    }

    virtual void end() {
    }
};

class CornellOpenSky : public PureCornell {
public:
    virtual std::string getName() const {
        return std::string( "cornell-open-sky" );
    }
    virtual void initRenderer( int width, int height ) {
        PureCornell::initRenderer( width, height );
        box.enableFace( box.y_positive, false );
    }
};


#include <picogen/graphics/objects/CloudsBouthors.h>

class Clouds : public TestScene {
private:
    picogen::graphics::samplers::screen::XYIterator<
    picogen::misc::templates::surface<
    picogen::graphics::image::color::AverageColor
    >,
    picogen::graphics::cameras::FromPointToRect_Cylinder,
    picogen::graphics::samplers::ray::Simple
    > renderer;
    int width, height;
    picogen::gob::LinearList list;
    picogen::gob::Preetham   preetham;
    picogen::gob::ZBNCubemap cubemap, cloudcubemap;
    picogen::gob::CloudsBouthors clouds;

public:
    explicit Clouds() {
    }

    virtual std::string getName() const {
        return std::string( "clouds" );
    }

    virtual void initRenderer( int width, int height ) {
        using namespace picogen;

        this->width  = width;
        this->height = height;
        // setup screen and camera
        renderer.camera().DefineCamera( (1.0*(real)width)/(real)height, 2.0, 1.0 );
        renderer.surface().reset( width*2, height*2 );

        // setup camera transform
        renderer.transformation() =
            geom::Transformation().setToRotationX( 3.14159*-0.0 ) *
            geom::Transformation().setToRotationY( 3.14159*0.0 ) *
            geom::Transformation().setToTranslation( Vector3d(0.0,0.0,0.0) )
            ;

        // setup and recognize sky
        preetham.SetTurbidity( 2.7 );
        preetham.SetSunSolidAngleFactor(1.0);
        const prim::real L = 0.25;
        preetham.SetColorFilter( Color(1.0,1.0,1.0)*1.0*L );
        preetham.SetSunColor( Color(0.95,0.95,1.0)*3000.0*L);
        preetham.SetSunDirection( Vector3d(0.0,1.0,-1.0).normal() );
        preetham.EnableFogHack( 0, 0.00082*0.05, 50000000 );
        preetham.Invalidate();
        renderer.path_integrator().SetSky( &preetham );


        {
            int cbsize = 128;
            cubemap.getSurface(cubemap.x_negative).resize(cbsize,cbsize);
            cubemap.getSurface(cubemap.x_positive).resize(cbsize,cbsize);
            cubemap.getSurface(cubemap.y_negative).resize(cbsize,cbsize);
            cubemap.getSurface(cubemap.y_positive).resize(cbsize,cbsize);
            cubemap.getSurface(cubemap.z_negative).resize(cbsize,cbsize);
            cubemap.getSurface(cubemap.z_positive).resize(cbsize,cbsize);
        }
        {
            int cbsize = 256;
            cloudcubemap.getSurface(cubemap.x_negative).resize(cbsize,cbsize);
            cloudcubemap.getSurface(cubemap.x_positive).resize(cbsize,cbsize);
            cloudcubemap.getSurface(cubemap.y_negative).resize(cbsize,cbsize);
            cloudcubemap.getSurface(cubemap.y_positive).resize(cbsize,cbsize);
            cloudcubemap.getSurface(cubemap.z_negative).resize(cbsize,cbsize);
            cloudcubemap.getSurface(cubemap.z_positive).resize(cbsize,cbsize);
        }
        /* clouds */ {
            graphics::objects::SimpleHeightField heightField;

            using namespace picogen::misc::functions;
            uv_to_scalar::PerlinNoise function4;
            function4.setPersistence(0.85);
            function4.setBaseFrequency( 1 );
            function4.setOctaves(5);
            function4.setDomainScale(3.0);
            function4.setRangeScale(0.0125);
            function4.setMode( function4.normal );
            function4.setMod( function4.mod_normal );
            function4.setEvalScale( 1.9 );
            function4.seed( 5 );
            uv_to_scalar::Power moon_multifractal( &function4, 1.0 );

            Lambertian lambertian;
            heightField.SetBRDF( &lambertian );
            heightField.SetShader( &white );
            heightField.SetBox( Vector3d(-10000,0,-10000), Vector3d(10000,400.0,10000) );
            heightField.Init( 1024, &moon_multifractal, 0.1, false );

            cloudcubemap.render( Vector3d(0,400+0*-1300,0), &heightField );
            clouds.setIntersectable( &cloudcubemap );
            clouds.setSky( &preetham );
            list.Insert( &clouds );
        }

        /* terrain */
        if (0) {
            graphics::objects::SimpleHeightField heightField;

            using namespace picogen::misc::functions;
            uv_to_scalar::PerlinNoise function4;
            function4.setPersistence(0.95);
            function4.setBaseFrequency( 2 );
            function4.setOctaves(8);
            function4.setDomainScale(3.0);
            function4.setRangeScale(0.0125);
            function4.setMode( function4.ridged );
            function4.setMod( function4.mod_heterofractal );
            function4.setEvalScale( 1.9 );
            function4.seed( 5 );
            uv_to_scalar::Power moon_multifractal( &function4, 0.4 );

            Lambertian lambertian;
            heightField.SetBRDF( &lambertian );
            heightField.SetShader( &white );
            heightField.SetBox( Vector3d(-10000,-3000,-10000), Vector3d(10000,0.0,10000) );
            heightField.Init( 256, &moon_multifractal, 0.1, false );

            cubemap.render( Vector3d(0,0,0), &heightField );
            list.Insert( &cubemap );
        }

        // invalidate and recognize object-list
        list.Invalidate();
        renderer.path_integrator().SetIntersectable( &list );
    }

    virtual void flip( SDL_Surface *screen, float scale, float saturation ) {
        ::SDL_Flip(screen);
        ::draw(screen,renderer.surface(), scale, 1.0, saturation );//scale, exp_tone, saturation);
    }

    virtual void begin() {
        renderer.BeginRender();
    }
    virtual bool renderMore( int numPixels ) {
        renderer.SetNumPixelsPerContinue( numPixels );
        bool c = renderer.Continue();
        if ( c ) {
            renderer.OneMoreRun();
        }
        return c;
    }
    virtual void end() {
    }
};




static int loop( SDL_Surface *screen, TestScene *scene, int width, int height ) {
    //scene->flip( screen );
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    // prepare and run loop
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    unsigned int pixelsPerContinue = (width*height*4)/16;
    unsigned int runCount = 1;
    //clock_t startTime = clock();
    bool done = false;
    bool queryingForReallyQuit = false;
    unsigned int numManDumbs = 0;
    float scale = 1;
    float exp_tone = 1.0;
    float saturation = 1.0;

    srand( time(NULL) );


    //renderer.SetNumPixelsPerContinue( pixelsPerContinue );//*(HEIGHT/1) );
    //renderer.BeginRender();
    scene->begin();
    while (!done ) {
        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            if ( event.type == SDL_QUIT ) {
                done = true;
            }
            if ( event.type == SDL_KEYDOWN ) {
                if ( queryingForReallyQuit ) {
                    if ( event.key.keysym.sym == SDLK_y ) {
                        done = true;
                        queryingForReallyQuit = false;
                    } else if ( event.key.keysym.sym == SDLK_n ) {
                        done = false;
                        queryingForReallyQuit = false;
                    }
                } else if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                    queryingForReallyQuit = 1;
                }
                if ( event.key.keysym.sym == SDLK_d ) {
                    char s[256];
                    sprintf( s, "screendump.%.4d.bmp", numManDumbs++ );
                    SDL_SaveBMP( screen, s );
                }
                if ( event.key.keysym.sym == SDLK_y ) {
                    pixelsPerContinue-=100*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if ( event.key.keysym.sym == SDLK_x ) {
                    pixelsPerContinue-=10*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if ( event.key.keysym.sym == SDLK_c ) {
                    pixelsPerContinue-=width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if ( event.key.keysym.sym == SDLK_v ) {
                    pixelsPerContinue+=width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if ( event.key.keysym.sym == SDLK_b ) {
                    pixelsPerContinue+=10*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }
                if ( event.key.keysym.sym == SDLK_n ) {
                    pixelsPerContinue+=100*width; /*renderer.SetNumPixelsPerContinue( pixelsPerContinue );*/
                }

                if ( event.key.keysym.sym == SDLK_5 ) {
                    scale = 1.0;
                }
                if ( event.key.keysym.sym == SDLK_6 ) {
                    scale += 0.01;
                }
                if ( event.key.keysym.sym == SDLK_7 ) {
                    scale += 0.1;
                }
                if ( event.key.keysym.sym == SDLK_8 ) {
                    scale += 1.0;
                }
                if ( event.key.keysym.sym == SDLK_9 ) {
                    scale += 10.0;
                }
                if ( event.key.keysym.sym == SDLK_4 ) {
                    scale -= 0.01;
                }
                if ( event.key.keysym.sym == SDLK_3 ) {
                    scale -= 0.1;
                }
                if ( event.key.keysym.sym == SDLK_2 ) {
                    scale -= 1.0;
                }
                if ( event.key.keysym.sym == SDLK_1 ) {
                    scale -= 10.0;
                }

                if ( event.key.keysym.sym == SDLK_t ) {
                    exp_tone = 1.0;
                }
                if ( event.key.keysym.sym == SDLK_z ) {
                    exp_tone += 0.01;
                }
                if ( event.key.keysym.sym == SDLK_u ) {
                    exp_tone += 0.1;
                }
                if ( event.key.keysym.sym == SDLK_i ) {
                    exp_tone += 1.0;
                }
                if ( event.key.keysym.sym == SDLK_o ) {
                    exp_tone += 10.0;
                }
                if ( event.key.keysym.sym == SDLK_r ) {
                    exp_tone -= 0.01;
                }
                if ( event.key.keysym.sym == SDLK_e ) {
                    exp_tone -= 0.1;
                }
                if ( event.key.keysym.sym == SDLK_w ) {
                    exp_tone -= 1.0;
                }
                if ( event.key.keysym.sym == SDLK_q ) {
                    exp_tone -= 10.0;
                }

                if ( event.key.keysym.sym == SDLK_g ) {
                    saturation -= 0.1;
                }
                if ( event.key.keysym.sym == SDLK_h ) {
                    saturation -= 0.01;
                }
                if ( event.key.keysym.sym == SDLK_j ) {
                    saturation = 1.0;
                }
                if ( event.key.keysym.sym == SDLK_k ) {
                    saturation += 0.01;
                }
                if ( event.key.keysym.sym == SDLK_l ) {
                    saturation += 0.1;
                }

            }
        }

        bool cont = scene->renderMore( pixelsPerContinue );
        if ( cont ) {
            runCount++;
            //
            //renderer.OneMoreRun();
            //if( 0 == (runCount%3) )
            //    renderer.surface().saveBinaryToFile( checkpointFile );
        } else if ( queryingForReallyQuit ) {
            SDL_WM_SetCaption( "Really Quit? (Y/N) ", "Really Quit? (Y/N) " );
        } else {
            char s[256];
            //float t = (static_cast<float>(clock()-startTime))/static_cast<float>(CLOCKS_PER_SEC);
            //sprintf( s, "%uspp:%ux%u/%.2fsec/color{scale%.2f", runCount, WIDTH, HEIGHT , t, scale );
            sprintf( s, "%uspp;color.{*%.2f,t%.2f,s%.2f};refresh{%.2fl}", runCount, scale, exp_tone, saturation, (float)pixelsPerContinue/(float)width );
            fprintf( stderr, "\rrendering pass %u (%s)", 1+runCount, s );
            SDL_WM_SetCaption( s, "..." );
        }

        //scene->flip( screen );
        scene->flip( screen, scale, saturation );
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

static int grind( int width, int height, TestScene *scene ) {
    using namespace std;
    scene->initRenderer( width, height );

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        cerr << "Unable to init SDL: " << SDL_GetError() << endl;
        return 1;
    }
    atexit(SDL_Quit);
    SDL_Surface *screen = SDL_SetVideoMode(width,height,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( NULL == screen ) {
        cerr << "Unable to set video-mode: " << SDL_GetError() << endl;
        return 1;
    }

    //scene->flip();
    return loop( screen, scene, width, height ); // TODO check if SQL cleans up surface resource
}



int main_testscenes( int argc, char *argv[] ) {
    using namespace std;
    static PureCornell pureCornell;
    static CornellOpenSky cornellOpenSky;
    static Clouds clouds;
    vector<TestScene*> scenes;
    scenes.push_back( &clouds );
    scenes.push_back( &pureCornell );
    scenes.push_back( &cornellOpenSky );

    TestScene* grindScene = NULL;

    while ( NULL == grindScene ) {
        cout << "the following test-scenes are available:\n\n  ";
        int i=0;
        for ( vector<TestScene*>::const_iterator it = scenes.begin(); it != scenes.end(); ++it ) {
            if ( it != scenes.begin() ) cout << ", ";
            cout << (*it)->getName() << " [" << i << "]";
            ++i;
        }
        cout << "\n" << endl;
        cout << "which one do you want to grind (enter number)? ";
        unsigned int num;
        cin >> num;
        if ( num < scenes.size() ) {
            grindScene = scenes[num];
            break;
        }
    }
    cout << "you have made your choice: " << grindScene->getName() << endl;

    int width, height;
    cout << "enter width (0 for default 320): ";
    cin >> width;
    cout << "enter height (0 for default 320): ";
    cin >> height;
    if ( width == 0 ) width = 320*2;
    if ( height == 0 ) height = 80*2;//320;

    cout << "your resolution of choice is: " << width << "x" << height << endl;
    cout << "commencing mission" << endl;
    return grind( width, height, grindScene );
}
