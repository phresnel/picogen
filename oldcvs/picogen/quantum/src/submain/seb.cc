/***************************************************************************
 *            seb.cc
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
#include <picogen/picogen.h>
#include <picogen/picossdf/picossdf.h>
#include <picogen/experimental/PicoPico.h>
#include <picogen/experimental/Rain.h>
#include <picogen/experimental/Snow.h>
#include <picogen/experimental/ShaderCC_GCC.h>
#include "compile_info.h"

using picogen::misc::prim::real;
using picogen::misc::geometrics::Vector3d;
using picogen::misc::geometrics::Ray;
using picogen::graphics::image::color::Color;
using picogen::graphics::structs::intersection_t;

/*
int main()
{
	picogen::graphics::samplers::ray::Simple simpleSampler;
	picogen::graphics::objects::TriBIH bih;
	picogen::graphics::objects::LinearList linearList;
	picogen::graphics::objects::Preetham preetham;
	picogen::misc::templates::vector<int> t;
	cloud_system::PerlinNoise perlinNoise;
	bih.Invalidate();
	std::cout << "Hello world" << std::endl;
	return 0;
}
*/
/* Created by Anjuta version 1.2.4a */
/*	This file will not be overwritten */

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <SDL/SDL.h>
#include <stdio.h>




void dump_call_stack() {
    std::cout << "call stack: " << std::endl;
    std::string indent(".");
    int u;
    for ( u=0; u<XRT_CALL_STACK_LENGTH(); u++ ) {
        std::cout << indent << XRT_CALL_STACK_GET_STR(u) << std::endl;
        indent = indent + ".";
    }
}



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


struct WaterBRDF : public picogen::graphics::material::abstract::IBRDF {
private:
    real *drop_x;
    real *drop_z;
    real *amplitude;
    real *frequency;
    real attenuation;
    unsigned int num_drops;
public:
    WaterBRDF() : drop_x(NULL), drop_z(NULL), amplitude(NULL), frequency(NULL), num_drops(0) {}

    virtual ~WaterBRDF() {
        if ( drop_x != NULL ) delete drop_x;
        drop_x = NULL;
        if ( drop_z != NULL ) delete drop_z;
        drop_z = NULL;
        if ( amplitude != NULL ) delete amplitude;
        amplitude = NULL;
        if ( frequency != NULL ) delete frequency;
        frequency = NULL;
        num_drops = 0;
    }
    void Init( unsigned int n, real l, real f, real r, real b, real attenuation ) {
        if ( drop_x != NULL ) delete drop_x;
        if ( drop_z != NULL ) delete drop_z;
        if ( amplitude != NULL ) delete amplitude;
        amplitude = NULL;
        if ( frequency != NULL ) delete frequency;
        frequency = NULL;
        drop_x = new real[n];
        drop_z = new real[n];
        amplitude = new real[n];
        frequency = new real[n];
        num_drops = n;
        for ( unsigned int u=0; u<n; u++ ) {
            drop_x[u] = ((real)rand()/(real)RAND_MAX) * (r-l) + l;
            drop_z[u] = ((real)rand()/(real)RAND_MAX) * (b-f) + f;
            amplitude[u] = ((real)rand()/(real)RAND_MAX)*0.5+0.5;
            frequency[u] = 2.0*powf( ((real)rand()/(real)RAND_MAX)*1.0, 3 );
            //amplitude[u] = 1.0 / (1.0+frequency[u]);
        }
        this->attenuation = attenuation;
    }

    virtual bool RandomSample(
        param_out(real,brdf),
        param_out(real,p),
        param_out(bool,specular),
        param_out(Ray,r_out),
        param_in (Ray,r_in),
        param_in (Vector3d,N)
    ) const {
        using picogen::misc::constants::pi;

        r_out.x() = r_in.x();
        //real f = 0.5+0.4*sin( r_in.x()[0]*0.05 );
        const real x = r_in.x()[0];
        const real z = r_in.x()[2];
        real f = 0.0;
        for ( unsigned int u=0; u<num_drops; u++ ) {
            const real p = x-drop_x[u];
            const real q = z-drop_z[u];
            const real d = sqrt(p*p+q*q);
            f += ( ( sin( d*frequency[u] ) ) * amplitude[u] ) / (1+attenuation*d);
        }
        f = fabs(1+f)+0.025;
        r_out.w() = r_in.w() - N.normal()*2*(r_in.w()*N.normal());
        r_out.w()[1] *= f;
        r_out.w() = r_out.w().normal();

        p = fabs(N.normal()*r_out.w().normal());
        brdf = 1.0;
        specular = true;
        return true;
    }
};








void insert_quad(
    picogen::graphics::objects::abstract::ITriScene *scene,
    picogen::misc::geometrics::Vector3d base,
    picogen::misc::prim::real radius,
    picogen::misc::geometrics::Vector3d &A,
    picogen::misc::geometrics::Vector3d &B,
    picogen::misc::geometrics::Vector3d &C,
    picogen::misc::geometrics::Vector3d &D
) {
    scene->Insert( base+A*radius, base+B*radius, base+C*radius );
    scene->Insert( base+B*radius, base+D*radius, base+C*radius );
}

// this function renders a box by mapping the cube to the unit sphere
// (probably  in  a  naive way,   but  should work  better  than  the
// latitude/longitude function)
void insert_sphere(
    picogen::graphics::objects::abstract::ITriScene *scene,
    int detail,
    picogen::misc::functions::abstract::vector_to_scalar *function,
    picogen::misc::geometrics::Vector3d center,
    picogen::misc::prim::real radius,
    unsigned int mask = 0xFFFFFFFF
) {
    using picogen::misc::prim::real;
    using picogen::misc::geometrics::Vector3d;
    // hmm, how do we write as less as possible ...
    const static real uv[6][2][3] = { // have six pairs of u/v-axes
        { { 1, 0, 0 }, { 0, 0, -1 } },
        { { 1, 0, 0 }, { 0, 0,  1 } },

        { { 0, 0, -1 }, { 0, -1, 0 } },
        { { 0, 0,  1 }, { 0, -1, 0 } },

        { { -1, 0, 0 }, { 0, -1, 0 } },
        { { 1, 0, 0 }, { 0, -1, 0 } },
    };
    const static real base[6][3] = {
        { -0.5, +0.5, +0.5 },
        { -0.5, -0.5, -0.5 },

        { -0.5, 0.5, 0.5 },
        {  0.5, 0.5, -0.5 },

        {  0.5, 0.5, 0.5 },
        {  -0.5, 0.5, -0.5 },
    };

    //real stepSize=1.0/(real)detail;
    unsigned int side;

    for ( side=0; side<6; side++ ) { // for each of the six planes
        if ( ((mask>>side)&1) == 0 )
            continue;
        int U,V;
        for ( V=0; V<detail; V++ ) {
            const real v  = (real)V     / (real)detail;
            const real v_ = (real)(V+1) / (real)detail;
            for ( U=0; U<detail; U++ ) {
                const real u  = (real)U     / (real)detail;
                const real u_ = (real)(U+1) / (real)detail;
                //real A[3], B[3], C[3], D[3];

                Vector3d A(
                    base[side][0]  +  uv[side][0][0] * u  +  uv[side][1][0] * v,
                    base[side][1]  +  uv[side][0][1] * u  +  uv[side][1][1] * v,
                    base[side][2]  +  uv[side][0][2] * u  +  uv[side][1][2] * v
                );
                A = A.normal();

                Vector3d B(
                    base[side][0]  +  uv[side][0][0] * u_  +  uv[side][1][0] * v,
                    base[side][1]  +  uv[side][0][1] * u_  +  uv[side][1][1] * v,
                    base[side][2]  +  uv[side][0][2] * u_  +  uv[side][1][2] * v
                );
                B = B.normal();

                Vector3d C(
                    C[0] = base[side][0]  +  uv[side][0][0] * u  +  uv[side][1][0] * v_,
                    C[1] = base[side][1]  +  uv[side][0][1] * u  +  uv[side][1][1] * v_,
                    C[2] = base[side][2]  +  uv[side][0][2] * u  +  uv[side][1][2] * v_
                );
                C = C.normal();

                Vector3d D(
                    D[0] = base[side][0]  +  uv[side][0][0] * u_  +  uv[side][1][0] * v_,
                    D[1] = base[side][1]  +  uv[side][0][1] * u_  +  uv[side][1][1] * v_,
                    D[2] = base[side][2]  +  uv[side][0][2] * u_  +  uv[side][1][2] * v_
                );
                D = D.normal();


                real tmp;
                tmp = 1.0+function->f( A[0], A[1], A[2] );
                A[0] *= tmp;
                A[1] *= tmp;
                A[2] *= tmp;
                tmp = 1.0+function->f( B[0], B[1], B[2] );
                B[0] *= tmp;
                B[1] *= tmp;
                B[2] *= tmp;
                tmp = 1.0+function->f( C[0], C[1], C[2] );
                C[0] *= tmp;
                C[1] *= tmp;
                C[2] *= tmp;
                tmp = 1.0+function->f( D[0], D[1], D[2] );
                D[0] *= tmp;
                D[1] *= tmp;
                D[2] *= tmp;

                insert_quad( scene, center, radius, A, B, C, D );
            }
        }
    }
}


void insert_cube(
    picogen::graphics::objects::abstract::ITriScene *scene,
    int detail,
    picogen::misc::functions::abstract::vector_to_scalar *function,
    picogen::misc::geometrics::Vector3d center,
    picogen::misc::prim::real radius,
    unsigned int mask = 0xFFFFFFFF
) {
    using picogen::misc::prim::real;
    using picogen::misc::geometrics::Vector3d;
    // hmm, how do we write as less as possible ...
    const static real uv[6][2][3] = { // have six pairs of u/v-axes
        { { 1, 0, 0 }, { 0, 0, -1 } },
        { { 1, 0, 0 }, { 0, 0,  1 } },

        { { 0, 0, -1 }, { 0, -1, 0 } },
        { { 0, 0,  1 }, { 0, -1, 0 } },

        { { -1, 0, 0 }, { 0, -1, 0 } },
        { { 1, 0, 0 }, { 0, -1, 0 } },
    };
    const static real base[6][3] = {
        { -0.5, +0.5, +0.5 },
        { -0.5, -0.5, -0.5 },

        { -0.5, 0.5, 0.5 },
        {  0.5, 0.5, -0.5 },

        {  0.5, 0.5, 0.5 },
        {  -0.5, 0.5, -0.5 },
    };

    //real stepSize=1.0/(real)detail;
    unsigned int side;

    for ( side=0; side<6; side++ ) { // for each of the six planes
        if ( ((mask>>side)&1) == 0 )
            continue;
        int U,V;
        for ( V=0; V<detail; V++ ) {
            const real v  = (real)V     / (real)detail;
            const real v_ = (real)(V+1) / (real)detail;
            for ( U=0; U<detail; U++ ) {
                const real u  = (real)U     / (real)detail;
                const real u_ = (real)(U+1) / (real)detail;
                //real A[3], B[3], C[3], D[3];

                Vector3d A(
                    base[side][0]  +  uv[side][0][0] * u  +  uv[side][1][0] * v,
                    base[side][1]  +  uv[side][0][1] * u  +  uv[side][1][1] * v,
                    base[side][2]  +  uv[side][0][2] * u  +  uv[side][1][2] * v
                );
                //A = A.normal();

                Vector3d B(
                    base[side][0]  +  uv[side][0][0] * u_  +  uv[side][1][0] * v,
                    base[side][1]  +  uv[side][0][1] * u_  +  uv[side][1][1] * v,
                    base[side][2]  +  uv[side][0][2] * u_  +  uv[side][1][2] * v
                );
                //B = B.normal();

                Vector3d C(
                    C[0] = base[side][0]  +  uv[side][0][0] * u  +  uv[side][1][0] * v_,
                    C[1] = base[side][1]  +  uv[side][0][1] * u  +  uv[side][1][1] * v_,
                    C[2] = base[side][2]  +  uv[side][0][2] * u  +  uv[side][1][2] * v_
                );
                //C = C.normal();

                Vector3d D(
                    D[0] = base[side][0]  +  uv[side][0][0] * u_  +  uv[side][1][0] * v_,
                    D[1] = base[side][1]  +  uv[side][0][1] * u_  +  uv[side][1][1] * v_,
                    D[2] = base[side][2]  +  uv[side][0][2] * u_  +  uv[side][1][2] * v_
                );
                //D = D.normal();


                // TODO
                real tmp;
                tmp = 1.0+function->f( A[0], A[1], A[2] );
                A[0] *= tmp;
                A[1] *= tmp;
                A[2] *= tmp;
                tmp = 1.0+function->f( B[0], B[1], B[2] );
                B[0] *= tmp;
                B[1] *= tmp;
                B[2] *= tmp;
                tmp = 1.0+function->f( C[0], C[1], C[2] );
                C[0] *= tmp;
                C[1] *= tmp;
                C[2] *= tmp;
                tmp = 1.0+function->f( D[0], D[1], D[2] );
                D[0] *= tmp;
                D[1] *= tmp;
                D[2] *= tmp;

                insert_quad( scene, center, radius, A, B, C, D );
            }
        }
    }
}







class WaterShader : public picogen::graphics::material::abstract::IShader {
public:
    virtual ~WaterShader() {};
    virtual void Shade(
        picogen::graphics::image::color::Color &color,
        const picogen::misc::geometrics::Vector3d &normal,
        const picogen::misc::geometrics::Vector3d &position
    ) const {
        using picogen::graphics::image::color::Color;
        using picogen::misc::prim::real;
        const real albedo = 0.7;
        //const Color A(0.7,1.0,0.9);
        const Color A(1.0,1.0,1.0);
        /*const Color B(1.0,0.0,0.0);

        const real n = normal[1];
        const real f = 0.0;//pow(n,7);
        color = (A*(1.0-f) + B*(f))*albedo;*/
        color = A*albedo;
    }
};


#ifdef POSIX
static std::string getPicogenSaveDirectory() {
    //>> oh this is a real kludge to find out the home/~-dir
    //   and i am not sure if this is the way it has to be done
    std::string dirname;
    FILE *f = popen( "echo ~", "r" );
    if ( NULL != f ) {
        char buff[1024];
        while ( NULL != fgets( buff, sizeof( buff ), f ) ) {
            const unsigned int len = strlen(buff);
            if ( len > 0 ) {
                if ( buff[len-1] == '\n' )
                    buff[len-1] = '\0';
                dirname = std::string( buff ) + "/.picogen/";
                break;
            }
        }
        fflush( f );
        pclose( f );
    }
    return dirname;
}
#endif

int init_sys() {
    static const char *line = "---------------------------------\n";
    printf( "%s%s\n%s", line, COMPILATION_INFO, line );
    printf( "picogen's directory for file saving: '%s'\n", getPicogenSaveDirectory().c_str() );
    printf( "%s", line );
    return 0;
}


#include <picogen/misc/functional.h>

using namespace picogen;
using namespace misc;

int main_seb( int argc, char *argv[] ) {
    using namespace functional;
    using namespace std;
    //using namespace RR;

    /*Function_R1_R1 f(
        neg_( parameter_(0) )
    );
    for( float x=1; x<=6; x+=1 ){
        printf( " f(%.5f) = %.5f\n", x, f(x) );
    }*/


    //(fac (? (<= x 1.0 )
    //      1.0
    //      (* x (fac (- x 1.0) ) ) ) )


    /*Function_R2_R1 fac(
        "(?(<=x 1.0)\n"
        "   1.0\n"
        "   (*x(self(-x 1) )))\n"
    );

    for( float x=1; x<=8; x+=1 ){
        printf( " f(%.5f) = %.5f\n", x, fac(x) );
    }*/

    // (len (sqrt (+ (^ x 2) (^ y 2) ) ) )
    /*Function_R2_R1 len(
        sqrt_( add_( pow_(parameter_(0),constant_(2.0)),
                     pow_(parameter_(1),constant_(2.0))
                   ) ) )
    ;*/
    while ( 1 ) {
        //std::string code;
        //cin >> code;
        try {
            char l[1024];
            fgets(l,1024,stdin);
            Function_R2_R1 len( l );

            for ( float v=0.0; v<=10.0; v+=1.0 ) {
                for ( float u=0.0; u<=10.0; u+=1.0 ) {
                    float x = len(u,v);
                    printf( "%s%0.4f ", x>=0.0?" ":"", x );
                }
                printf( "\n" );
            }
        } catch ( const functional_general_exeption &e ) {
            cerr << e.getMessage() << endl;
        }
    }
    return 0;
    //picogen::PicoPico();
    //return 0;

    //ShaderCC_GCC( "../../../test-files/shader/shader-0001", "terrain" );
    //return 0;
    //bool front = true;//fork() != 0;
    XRT_CALL_STACK_PUSH( "int main( int argc. char *argv[] )" );
    try {
        init_sys();
        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        // output version/build info
        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        std::cout << "xrt "
                  << XRT_VERSION_MAJOR << "."
                  << XRT_VERSION_MINOR << "."
                  << XRT_VERSION_PATCH;
#ifdef XRT_DEBUG
        std::cout << " debug-build";
#else
#endif
        std::cout << std::endl;

        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        // setup screen and rendersystem
        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        using namespace picogen;
        using misc::prim::real;
        using misc::geometrics::Transformation;
        const real planet_radius = 100000;//12000*1000;
        bool useAA = true;
        const int WIDTH = 1280,//-(1280/4),
                          HEIGHT = 1024;//-(512/4);
        fprintf( stderr, "Requested resolution is %ix%i; AA is %s\n", WIDTH, HEIGHT, useAA?"on":"off" );
        graphics::samplers::screen::XYIterator<
        misc::templates::surface<graphics::image::color::AverageColor>,
        graphics::cameras::FromPointToRect,
        graphics::samplers::ray::Simple
        > renderer;
        renderer.camera().DefineCamera( (real)WIDTH/(real)HEIGHT, 1, 0.7 );


        renderer.surface().reset( WIDTH*2, HEIGHT*2 );

        // TODO: this is a hack for defining the name of the auto-save-file
        std::string checkpointFile;
        while (1) {
            std::cout << "Do you want to load a previous buffer (yes/no)? ";// Type in filename, then: ";
            checkpointFile = "no";
            //std::cin >> checkpointFile;
            if ( checkpointFile == "yes" ) {
                std::cout << "Type in a filename: ";
                std::cin >> checkpointFile;
                FILE *f = fopen( checkpointFile.c_str(), "r" );
                if ( f != NULL ) {
                    fclose( f );
                    renderer.surface().loadBinaryFromFile( checkpointFile );
                    break;
                } else {
                    f = fopen( checkpointFile.c_str(), "w" );
                    if ( f != NULL ) {
                        fclose( f );
                        std::cout << "File '" << checkpointFile << "' could not be opened, but created it" << std::endl;
                        break;
                    } else {
                        std::cout << "File '" << checkpointFile << "' could neither be opened, nor be created, type in another filename, please." << std::endl;
                    }
                };
            } else if ( checkpointFile == "no" ) {
                checkpointFile = getPicogenSaveDirectory() + "auto.frgb";
                std::cout << "Using standard file '"<< checkpointFile << "'" << std::endl;
                break;
            }
        }


        if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
            fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
            return (1);
        }
        atexit(SDL_Quit);
        SDL_Surface *screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
        if ( !screen ) {
            fprintf( stderr, "Unable to set video-mode: %s\n", SDL_GetError() );
            return (1);
        }


        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        // setup scene
        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        Vector3d base(0.1,planet_radius,0.1);
        Lambertian a;

        picogen::graphics::objects::Sphere sphere[1];
        sphere[0].setPosition( Vector3d(0,0,0) );
        sphere[0].setRadius( planet_radius );
        sphere[0].setEmittance( 0.0 );
        sphere[0].setColor( Color(1.0,0.8,0.6) );
        a.mirror = 0.0;
        sphere[0].setBRDF( &a );


        //graphics::objects::TriBIH trilist;
        //Lambertian lamb;
        //GroundShader groundShader;
        //lamb.mirror = 0.0;

        //trilist.SetBRDF( &lamb );
        //trilist.SetShader( &groundShader );
        //trilist2.SetBRDF( &a );
        graphics::objects::LinearList list;


        Lambertian heightBRDF;

        /*picogen::misc::functions::vector_to_scalar::PerlinNoise heightShaderFunc;
        TerrainShader heightShader(&heightShaderFunc);
        {
            heightShaderFunc.setPersistence(0.96);
            heightShaderFunc.setBaseFrequency( 1.0 );
            heightShaderFunc.setOctaves(15);
            heightShaderFunc.setDomainScale(0.0125);
            heightShaderFunc.setRangeScale(5.0);
            heightShaderFunc.setMode( heightShaderFunc.normal );
            heightShaderFunc.setMod( heightShaderFunc.mod_normal );
            heightShaderFunc.setEvalScale( 1.8 );
            heightShaderFunc.seed( 3421 );
        }*/

        graphics::objects::ZBNCubemap cubemap;
        int cbsize = 1280;
        cubemap.getSurface(cubemap.x_negative).resize(cbsize,cbsize);
        cubemap.getSurface(cubemap.x_positive).resize(cbsize,cbsize);
        cubemap.getSurface(cubemap.y_negative).resize(cbsize,cbsize);
        cubemap.getSurface(cubemap.y_positive).resize(cbsize,cbsize);
        cubemap.getSurface(cubemap.z_negative).resize(cbsize,cbsize);
        cubemap.getSurface(cubemap.z_positive).resize(cbsize,cbsize);

        {
            ShaderCC_GCC heightShader( "../../../test-files/shader/grassy-terrain" );

            graphics::objects::SimpleHeightField heightField;
            {
                using namespace picogen::misc::functions;
                uv_to_scalar::PerlinNoise function4;
                function4.setPersistence(0.95);
                function4.setBaseFrequency( 2.0 );
                function4.setOctaves(16);
                function4.setDomainScale(3.0);
                function4.setRangeScale(0.0125);
                function4.setMode( function4.ridged );
                function4.setMod( function4.mod_heterofractal );
                function4.setEvalScale( 1.9 );
                function4.seed( 5 );
                uv_to_scalar::Power moon_multifractal( &function4, 0.4 );
                //insert_sphere( &trilist, 200, &moon_multifractal, Vector3d(0,0,0), planet_radius, 1 );
                //trilist.Invalidate();
                //list.Insert( &trilist );
                // ++ TEH GROUND ++++++++++++++++++++++++++++++++++++++++++++++
                heightBRDF.mirror = 0.0;
                heightField.SetBRDF( &heightBRDF );
                heightField.SetShader( &heightShader );
                heightField.SetBox( base+Vector3d(-10000,-3000,-10000), base+Vector3d(10000,0.0,10000) );
                heightField.Init( 4096, &moon_multifractal, 0.1, false );
                //list.Insert( &heightField );

                // -- TEH GROUND ----------------------------------------------
            }


            cubemap.render( base+Vector3d(5000, -1300,-10000), &heightField );
        }
        list.Insert( &cubemap );

        //graphics::objects::Snow snow;
        //list.Insert( &snow );
        list.Invalidate();

//		using namespace picogen::misc::functions;
//
//		/*vector_to_scalar::Sine function1;
//			function1.domain_scale = 10.0;
//			function1.range_scale = 1.0;
//			function1.setMode( function1.normal );*/
//		/*vector_to_scalar::Elias_one function3;
//			function3.seed( 250, 311 );
//			function3.T = 0.001;
//			function3.setMode( function3.normal );*/
//		vector_to_scalar::PerlinNoise function3;
//			function3.setPersistence(0.99);
//			function3.setBaseFrequency( 1.0 );
//			function3.setOctaves(5);
//			function3.setDomainScale(3.0);
//			function3.setRangeScale(0.025);
//			function3.setMode( function3.ridged );
//			function3.setMod( function3.mod_heterofractal );
//			function3.setEvalScale( 1.99 );
//		int tmp = 423;
//		//printf( "seed?" );
//		//scanf( "%d", &tmp );
//		function3.seed( tmp );
//		//*/
//		vector_to_scalar::Power multifractal( &function3, 1.0 );
//		//uv_to_scalar::Negate multifractal( &_multifractal );
//		vector_to_scalar::PerlinNoise function4;
//			function4.setPersistence(0.96);
//			function4.setBaseFrequency( 1.0 );
//			function4.setOctaves(12);
//			function4.setDomainScale(7.5);
//			function4.setRangeScale(0.0125);
//			function4.setMode( function4.ridged );
//			function4.setMod( function4.mod_heterofractal );
//			function4.setEvalScale( 1.8 );
//			function4.seed( 42 );
//		vector_to_scalar::Power moon_multifractal( &function4, 1.0 );
//		//insert_sphere( &trilist, 150, &function1, base+Vector3d(-2.6,1.2,0.3), 2.0*0.6 );
//		//insert_sphere( &trilist, 150, &function2, base+Vector3d(-0.0,1.2,2.0), 2.0*0.6 );
//		const Vector3d harbingerPosition = base+Vector3d(0.0,300.2,0);
//		const Vector3d harbingerVelocity = Vector3d(0.4,-0.7,2.7).normal()*350.0;
//		insert_sphere( &trilist, 4, &moon_multifractal, base+Vector3d(33000,23000,-80000), 40000.2*0.58 );
//			function4.seed( 7 );
//			function4.setRangeScale(0.6);
//		insert_sphere( &trilist, 3, &multifractal, base+Vector3d(-20000,20000,-80000), 20000.2*0.45 );
//			function4.setRangeScale(3.9);
//			function4.seed( 154111 );
//		//insert_sphere( &trilist, 20, &function4, base+Vector3d(-130000,50000,-390000), 140000.2*0.85 );
//
//		/*insert_sphere( &trilist2, 5, &function4, harbingerPosition+Vector3d(100,20,170), 30.2*0.65 );
//		insert_sphere( &trilist2, 5, &function4, harbingerPosition+Vector3d(-100,20,130), 20.2*0.65 );
//		insert_sphere( &trilist2, 5, &function4, harbingerPosition+Vector3d(180,-40,100), 39.2*0.65 );
//		insert_sphere( &trilist2, 5, &function4, harbingerPosition+Vector3d(-30,20,120), 20.2*0.65 );	*/
//		unsigned int c;
//		if(0) for( c=0; c<450000; c++ ){
//		    // some displacement
//			real x = ( -0.5 + (real)rand() / (real)RAND_MAX ) * 2000;
//			real y = ( -0.5 + (real)rand() / (real)RAND_MAX ) * 100;
//			real z = ( -0.5 + (real)rand() / (real)RAND_MAX ) * 2000;
//
//			real r = (  0.0 + (real)rand() / (real)RAND_MAX ) * 45+10;
//			real d = (  0.0 + (real)rand() / (real)RAND_MAX ) * 16000+30000;
//
//            vector_to_scalar::Constant c(0);
//			insert_sphere( &trilist, 1, &c,
//                base+Vector3d(33000,23000,-80000)
//				+Vector3d(x,y,z).normal()*d,
//				r
//			);
//		}
//
//		trilist.Invalidate();
//		trilist2.Invalidate();
//
//		CClouds clouds( planet_radius, 1000 );
//
//
//		// ++ TEH GROUND ++++++++++++++++++++++++++++++++++++++++++++++
//		graphics::objects::SimpleHeightField heightField;
//		Lambertian heightBRDF;
//		GroundShader heightShader;
//		heightBRDF.mirror = 0.0;
//		heightField.SetBRDF( &heightBRDF );
//		heightField.SetShader( &heightShader );
//		heightField.SetBox( base+Vector3d(-10000,-500,-10000), base+Vector3d(10000,500.0,10000) );
//		//heightField.Init( 4096/8+1, &multifractal );
//		// -- TEH GROUND ----------------------------------------------
//
//		// ++ TEH WATER ++++++++++++++++++++++++++++++++++++++++++++++
//		graphics::objects::SimpleHeightField waterField;
//		WaterBRDF waterBRDF;
//		WaterShader waterShader;
//		//waterBRDF.mirror = 1.0;
//		waterBRDF.Init( 6*100, -10000, -10000, 10000, 10000, 0.000001 );
//		waterField.SetBRDF( &waterBRDF );
//		waterField.SetShader( &waterShader );
        //const real waterLevel = 0;
//		waterField.SetBox( base+Vector3d(-20000,waterLevel,-20000), base+Vector3d(20000,waterLevel+10.0,20000) );
//		uv_to_scalar::Constant wf(0);
//		waterField.Init( 16+1, &wf );
//		// -- TEH WATER ----------------------------------------------
//
//		graphics::objects::VelocityInstance vi1;
//		vi1.SetOffset( Vector3d(0.0,0.0,0.0) );
//		vi1.SetVelocity( Vector3d(100,0.5,30) );
//		vi1.SetTimeExponent( 1.0 );
//		vi1.SetIntersectable( &waterField );
//		/*graphics::objects::VelocityInstance vi2;
//		vi2.SetOffset( Vector3d(0.0,0.0,0.0) );
//		vi2.SetVelocity( harbingerVelocity*0.005 );
//		vi2.SetTimeExponent( 8.0 );
//		vi2.SetIntersectable( &trilist );
//		graphics::objects::VelocityInstance vi3;
//		vi3.SetOffset( Vector3d(0.0,0.0,0.0) );
//		vi3.SetVelocity( Vector3d(-1.7,1.4,4.1).normal()*-750 );
//		vi3.SetTimeExponent( 3.0 );
//		vi3.SetIntersectable( &trilist2 );*/
//
//		graphics::objects::LinearList list;
//
//		//list.Insert( &heightField );
//		list.Insert( &waterField );
//		list.Insert( &trilist );
//		/*
//		list.Insert( &trilist2 );*/
//		//list.Insert( &clouds );
//		list.Invalidate();


        /*intersection_t i_tmp;
        Ray r_tmp;
        r_tmp.x() = base+Vector3d(-9000,10000,-9000);
        r_tmp.w() = Vector3d(0.01,-1.0,0.01).normal();
        bool didSo = list.Intersect( i_tmp, r_tmp );

        Vector3d pos = r_tmp.x() + r_tmp.w()*i_tmp.t + i_tmp.normal*0.0 + Vector3d(0,250,0);
        printf( "camera-pos = %.2f, %.2f, %.2f (t=%.2f, any=%s)\n", (float)pos[0], (float)pos[1], (float)pos[2], i_tmp.t, didSo?"yes":"no" );
        renderer.transformation() =
        	Transformation().setToRotationY( 3.14159*0.25 ) *
        	Transformation().setToTranslation( pos );
        */
        renderer.transformation() =
            //Transformation().setToRotationX( 3.14159*0.125*0.5 ) *
            Transformation().setToRotationX( 3.14159*0.0 ) *
            Transformation().setToRotationY( 3.14159*-0.1 ) *
            Transformation().setToTranslation( base+Vector3d(900,0,-900) )
            ;
        renderer.path_integrator().SetIntersectable( &list );

        //> setup sky
        graphics::objects::Preetham preetham;


        preetham.SetTurbidity( 4.7 );
        preetham.SetSunSolidAngleFactor(0.5);
        const real L = 0.2;
        /*preetham.SetColorFilter( Color(1.0,0.8,0.7)*1.0*L );  // > red filter
        preetham.SetSunColor( Color(1.0,0.3,0.2)*3200.0*3.4*L );//*/
        preetham.SetColorFilter( Color(1.0,1.0,1.0)*1.0*L );
        preetham.SetSunColor( Color(1.0,0.9,0.8)*43500.0*1.3*L*0.02 );//*/
        preetham.SetSunDirection( Vector3d(200.5,2.25,1.7).normal() );
        preetham.EnableFogHack( 1, 0.00082*0.05, 50000 );
        preetham.Invalidate();

        //> setup clouds
        //CloudShader cloud_shader;
        //cloud_shader.SetSky( &preetham );
        renderer.path_integrator().SetSky( &preetham );
        //clouds.SetSkyShader( &preetham );
        //clouds.SetCloudShader( &cloud_shader );

        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        // prepare and run loop
        // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
        unsigned int pixelsPerContinue = WIDTH*10;
        unsigned int runCount = 1;
        //clock_t startTime = clock();
        bool done = false;
        bool queryingForReallyQuit = false;
        unsigned int numManDumbs = 0;
        float scale = 1;
        float exp_tone = 1.0;
        float saturation = 1.0;

        srand( time(NULL) );


        renderer.SetNumPixelsPerContinue( pixelsPerContinue );//*(HEIGHT/1) );
        renderer.BeginRender();
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
                    if ( event.key.keysym.sym == SDLK_c ) {
                        pixelsPerContinue-=WIDTH;
                        renderer.SetNumPixelsPerContinue( pixelsPerContinue );
                    }
                    if ( event.key.keysym.sym == SDLK_v ) {
                        pixelsPerContinue+=WIDTH;
                        renderer.SetNumPixelsPerContinue( pixelsPerContinue );
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

            bool cont = renderer.Continue();
            if ( cont ) {
                runCount++;
                renderer.OneMoreRun();
                //if( 0 == (runCount%3) )
                renderer.surface().saveBinaryToFile( checkpointFile );
            } else if ( queryingForReallyQuit ) {
                SDL_WM_SetCaption( "Really Quit? (Y/N) ", "Really Quit? (Y/N) " );
            } else {
                char s[256];
                //float t = (static_cast<float>(clock()-startTime))/static_cast<float>(CLOCKS_PER_SEC);
                //sprintf( s, "%uspp:%ux%u/%.2fsec/color{scale%.2f", runCount, WIDTH, HEIGHT , t, scale );
                sprintf( s, "%uspp;color.{*%.2f,t%.2f,s%.2f};refresh{%.2fl}", runCount, scale, exp_tone, saturation, (float)pixelsPerContinue/(float)WIDTH );
                fprintf( stderr, "\rrendering pass %u (%s)", 1+runCount, s );
                SDL_WM_SetCaption( s, "..." );
            }

            SDL_Flip(screen);
            draw(screen,renderer.surface(),scale, exp_tone, saturation);
            /*unsigned int numSamples;
            for( numSamples=0; numSamples<10000; numSamples ++ ){
            	unsigned int u = rand() % surf.width();
            	unsigned int v = rand() % surf.height();
            	if( (rand()%500000) > u*u+v*v )
            		surf(u,v) += base_types::color(1,1,1);
            }*/
        }

    } catch ( picogen::misc::exceptions::base b ) {
        std::cout << "error: ´" << b.message << "´" << std::endl;
        std::cout << "in line " << b.line << ", file ´" << b.filename << "´" << std::endl;
        dump_call_stack();
    } catch ( ... ) {
        std::cout << "error: undefined exception" << std::endl;
        dump_call_stack();
    }
    XRT_CALL_STACK_POP();
    return 0;
}
