//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include <SDL/SDL.h>
#include <redshift/include/redshift.hh>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Maths.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <kallisto/common.hh>

namespace kallisto {
        template <typename int_t, unsigned int FRACT_BITS>
        std::ostream& operator<< (std::ostream& o, const fixed_point_t<int_t, FRACT_BITS> & f) {
                // TODO: crap. should read the current precision.
                if (f.bits>0) {
                        return o << (f.bits>>FRACT_BITS)
                                 << '.'
                                 << ((f.bits & ((1 << FRACT_BITS)-1))*1000000) / (1<<FRACT_BITS) ;
                } else {
                        return o << '-'
                                 << (-f.bits>>FRACT_BITS)
                                 << '.'
                                 << ((-f.bits & ((1 << FRACT_BITS)-1))*1000000) / (1<<FRACT_BITS) ;
                }
        }
}

namespace kallisto {
        /*typedef vector_t<int64_t, 16> CartesianPosition;
        typedef vector_t<int64_t, 16> CartesianDirection;*/

        /*template <coordinate_space_t S, typename POSITION_SCALAR_T,
                                                typename DIRECTION_SCALAR_T>
        struct ray_t {
                point_t<S, POSITION_SCALAR_T> position;
                vector_t<S, DIRECTION_SCALAR_T> direction;
        };*/
}


namespace kallisto {
        template <typename position_t, typename vector_t, typename radius_t>
        bool doesIntersect (ray_t<position_t,vector_t> const &ray, sphere_t<position_t,radius_t> const & sphere) {
typedef float real_t;


                const vector_t v = vector_cast<vector_t> (ray.position - sphere.center);
                const real_t b =
                        0 - (v.x * ray.direction.x)
                        - (v.y * ray.direction.y)
                        - (v.z * ray.direction.z);
                const real_t sqrV = v.x*v.x + v.y*v.y + v.z*v.z;

                const real_t det = (b * b) - sqrV + sphere.radius*sphere.radius;
                /*static int e = 0;
                if (++e < 10) {

                        std::cout
                                << "X:" << ((ray.position - sphere.center).x)
                                        << "," << ((ray.position - sphere.center).y) << "," << ((ray.position - sphere.center).z) << "::" << '\n'


                        << "v:" << v.x << "," << v.y << "," << v.z << "::" << '\n'
                        << "direction:" << ray.direction.x << "," << ray.direction.y << "," << ray.direction.z << "::" << '\n'
                        << "det:" << det << '\n'
                        << "b:" << b << "\n"
                        << "sqrV:" << sqrV << '\n'
                        << "(" << sphere.radius << ")"
                        << ::std::endl;
                }*/
                if (det > 0) {
                        const real_t det_ = sqrt (det);
                        const real_t i1 = b - det_;
                        const real_t i2 = b + det_;
                        if (i2>0) {
                                if (i1<0) {
                                        //currT = i2;
                                        //currSide = inside;
                                        //return true;
                                } else {
                                        //currT = i1;
                                        //currSide = outside;
                                        return true;
                                }
                        }
                }
                return false;
                /*
                int i;
                Vector3d v;
                real_t sqrV = 0;
                real_t b    = 0;

                for (i=0; i<3; i++) {
                    v[i]  = ray.x() [i] - this->x[i];
                    b    -= v[i] * ray.w() [i];
                    sqrV += v[i] * v[i];
                }

                real_t det = (b * b) - sqrV + r*r;
                real_t i1, i2;

                real_t currT = real_t_max;
                if (det > 0) {
                    det = sqrt (det);
                    i1 = b - det;
                    i2 = b + det;
                    if (i2>0) {
                        if (i1<0) {
                            currT = i2;
                            currSide = inside;
                        } else {
                            currT = i1;
                            currSide = outside;
                        }
                    }
                }
                if (currSide != noside) {
                    if (currSide == inside) {
                        intersection.normal = (x - ray (currT)).normal();
                    } else {
                        intersection.normal = (ray (currT) - x).normal();
                    }
                    intersection.t = currT;
                    intersection.color = color;
                    intersection.brdf = brdf;
                    intersection.L_e = L_e;
                }
                return currSide != noside;
                */
        }
}


namespace redshift {

        class Color {
                float r, g, b;
        public:
                Color () : r(0.0f), g(0.0f), b(0.0f) {}
                Color (Color const & c) : r(c.r), g(c.g), b(c.b) {}
                Color (float const r_, float const g_, float const b_) : r(r_), g(g_), b(b_) {}
                Color & operator = (Color const & c) { r = c.r; g = c.g; b = c.b; return *this; }

                void toRgb (float &r, float &g, float &b) const {
                        r = this->r;
                        g = this->g;
                        b = this->b;
                }
        };

        /*class Planet {
        public:

        private;
                point_t center;
                Color
        };*/

        // A planet that is "frozen", as in "which can be rendered" :S
        class PreviewPlanet {
        public:
                PreviewPlanet (CartesianPoint const &center,
                        real_t const &radius, Color const &color)
                : center(center), radius(radius), color (color)
                {}

                CartesianPoint getCenter()   const   { return center; }
                real_t getRadius()  const   { return radius; }
                Color getColor()      const   { return color; }

        private:
                CartesianPoint center;
                real_t  radius; // to be used e.g. in a quick-preview
                Color     color;
        };
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ray tracer.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class TrayRacer {
        int width, height;

        ::std::vector< ::redshift::PreviewPlanet> planets;
public:
        TrayRacer (int const width, int const height)
        : width (width), height (height) {
                using namespace redshift;
                for (int64_t i=1; i<15; ++i) {
                        PreviewPlanet pp (
                                CartesianPoint(
                                        kallisto::scalar_cast<fixed_point_t>(i*1000L),
                                        kallisto::scalar_cast<fixed_point_t>(0.0),
                                        kallisto::scalar_cast<fixed_point_t>(i*100000L)
                                ),
                                10000,
                                Color(1.0,0.5,0.5)
                        );
                        planets.push_back (pp);
                }
        }
        redshift::Color operator () (int const x, int const y) const {
                using namespace redshift;
                using namespace std;

                const real_t fx = real_t(-0.5) + static_cast<real_t>(x) / static_cast<real_t>(width);
                const real_t fy = real_t(-0.5) + static_cast<real_t>(y) / static_cast<real_t>(height);
                kallisto::ray_t<CartesianPoint, CartesianVector> ray = {
                        CartesianPoint (
                                kallisto::scalar_cast<fixed_point_t>(0),
                                kallisto::scalar_cast<fixed_point_t>(0),
                                kallisto::scalar_cast<fixed_point_t>(0)),
                        CartesianVector (fx, fy, 0.5),
                };
                const real_t d = 1.0 / sqrt(ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z);
                ray.direction = ray.direction * d;

                const real_t ticks = static_cast<real_t> (SDL_GetTicks()) * 0.001f;

                float f = -2;
                for (vector<PreviewPlanet>::const_iterator it=planets.begin();
                    it != planets.end(); ++it) {

                        //f += 2.5;
                        const kallisto::sphere_t<CartesianPoint, float> sphere = {
                                /*CartesianPoint (
                                        kallisto::scalar_cast<fixed_point_t>(f),
                                        kallisto::scalar_cast<fixed_point_t>(3.0*sin (ticks+f*0.15)),
                                        kallisto::scalar_cast<fixed_point_t>(10)
                                ), 1.25f*/
                                it->getCenter(), it->getRadius()
                        };

                        const bool does = doesIntersect (ray, sphere);
                        if (does) return it->getColor();
                }
                return Color (ray.direction.x, ray.direction.y, ray.direction.z);
        }
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Things.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<class t_surface>
void draw (SDL_Surface *p_target, const t_surface &surface) {
        if (SDL_MUSTLOCK (p_target) && SDL_LockSurface (p_target)<0 )
                return;
        int x,y;
        for (y=0; y<p_target->h; y++) {
            // TODO FIX we are currently assuming a 32bit SDL buffer
                Uint32 *bufp = (Uint32*)p_target->pixels + y*(p_target->pitch>>2);
                for (x=0; x<p_target->w; x++){
                        float r, g, b;
                        surface (x,y).toRgb (r, g, b);
                        r = r<0?0:r>1?1:r;
                        g = g<0?0:g>1?1:g;
                        b = b<0?0:b>1?1:b;
                        *(bufp++) = SDL_MapRGB(p_target->format,
                                (unsigned char)(255.0*r),
                                (unsigned char)(255.0*g),
                                (unsigned char)(255.0*b));
                }
        }

        if (SDL_MUSTLOCK (p_target))
                SDL_UnlockSurface (p_target);
        SDL_Flip (p_target);
}


int main ( int argc, char** argv ) {
        using namespace std;


        //////////////////////////////////////////////////////////////////////
        // Basic SDl setup.
        //////////////////////////////////////////////////////////////////////
        // Initialize SDL video.
        if (SDL_Init (SDL_INIT_VIDEO) < 0) {
                cerr <<  "unable to init SDL:\n" << SDL_GetError() << endl;
                return 1;
        }
        atexit(SDL_Quit);

        // Create a new window.
        const unsigned int width = 256, height = width;
        SDL_Surface* screen = SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        if (0 == screen) {
                cerr << "Unable to set video to " << width << "x" << height << ":\n" << SDL_GetError() << endl;
                return 1;
        }

        //////////////////////////////////////////////////////////////////////
        // Setup renderer here.
        //////////////////////////////////////////////////////////////////////
        TrayRacer trayRacer (width, height);



        //////////////////////////////////////////////////////////////////////
        // Program main loop
        //////////////////////////////////////////////////////////////////////
        {
                // Fps.
                unsigned int fpsNumFrames = 0;
                float fpsFPS = 0.0f;
                clock_t fpsLastMeasure = clock();

                bool done = false;
                SDL_WM_SetCaption ("Pipapo.", "Pipapo.");
                while (!done /*&& ++f<100*/ ) {

                        // Lame fps checking.
                        if (clock() - fpsLastMeasure > 2*CLOCKS_PER_SEC){
                                fpsLastMeasure = clock();
                                fpsFPS = static_cast<float>(fpsNumFrames) * 0.5;
                                fpsNumFrames = 0;
                                //sprintf( caption, "fps:%.2f", fpsFPS );
                                std::cout << fpsFPS << "fps" << std::endl;
                                //SDL_WM_SetCaption( caption, "..." ); // Can set caption with this.
                        }

                        // message processing loop
                        SDL_Event event;
                        while (SDL_PollEvent(&event)) {
                                // check for messages
                                switch (event.type) {
                                        // exit if the window is closed
                                        case SDL_QUIT:
                                                done = true;
                                                break;

                                        // check for keydowns
                                        case SDL_KEYDOWN: {
                                                // Exit if ESCAPE is pressed.
                                                if (event.key.keysym.sym == SDLK_ESCAPE)
                                                done = true;
                                                break;
                                        }
                                        // check for keyups
                                        case SDL_KEYUP: {
                                                // Exit if ESCAPE is pressed.
                                                if (event.key.keysym.sym == SDLK_ESCAPE)
                                                done = true;
                                                break;
                                        }


                                        /*case SDL_MOUSEMOTION: {
                                                Uint8 keystate = SDL_GetMouseState(NULL, NULL);

                                                if( keystate & SDL_BUTTON(1) ){
                                                        lumiX += 0.01*static_cast<float>(event.motion.xrel);
                                                        lumiY -= 0.01*static_cast<float>(event.motion.yrel);
                                                }
                                        } break;*/
                                }
                        }

                        draw (screen, trayRacer);
                        ++fpsNumFrames;
                }
        }

        // All is well ;)
        cout << "exited cleanly." << endl;
        return 0;
}


/*

void test () {
        using namespace std;
        using namespace kallisto;
        using namespace redshift;
        vector_t<CARTESIAN,double> a (1,0,0), b(0,1,0), c(0,0.7,0.7);
        a = b + c;
        cout << a.x << ", " << a.y << ", " << a.z << endl;

        a = 5.0 * b;
        cout << a.x << ", " << a.y << ", " << a.z << endl;

        {
                //vector_t<int> a, b;
                //a = kallisto::vector_cast <vector_t<int> > (b);
                //cout << a.x << ", " << a.y << ", " << a.z << endl;

                //vector_t<float> a2, b2;
                //a2 = vector_cast <vector_t<float> > (b2);
                //cout << a2.x << ", " << a2.y << ", " << a2.z << endl;

                vector_t<fixed_point_t<int,8> > a3, b3;
                a3 = vector_cast <vector_t<fixed_point_t<int,8> > > (c);
                cout << a3.x << ", " << a3.y << ", " << a3.z << endl;

                if (false) {
                        typedef fixed_point_t<int64_t, 2> a_t;
                        typedef fixed_point_t<int64_t, 16> b_t;
                        a_t a = {(1<<2) | 1}, a_ = a;
                        b_t b = {(1<<16) | 32768};
                        cout << a << '\n';
                        cout << a_ << '\n';
                        cout << a+a+a+a+a << '\n';
                        cout << b << '\n';
                        cout << scalar_cast <a_t> (a) << '\n';
                        cout << "is fixed point = " << traits::is_fixed_point<fixed_point_t<int8_t, 2> >::value << endl;
                        cout << "is fixed point = " << traits::is_fixed_point<float>::value << endl;
                        cout << "----" << '\n';
                        cout << typeid (traits::get_vector_element_type<vector_t<int > >::type).name() << '\n';
                        cout << "----" << '\n';
                        cout << typeid (traits::get_fixed_point_basetype<b_t>::type).name() << '\n';
                        cout << traits::get_fixed_point_fract_bits<b_t>::value << '\n';
                        cout << "----" << '\n';
                        cout << "is vector = " << traits::is_vector<int>::value << endl;
                        cout << "is vector = " << traits::is_vector<vector_t<int> >::value << endl;
                        vector_t<b_t> ia;
                        //ia = vector_cast <vector_t<b_t> > (c);
                        cout << ia.x << ", " << ia.y << ", " << ia.z << endl;
                }
                cout << "----" << '\n';
                if (false) {
                        typedef fixed_point_t<int64_t, 24> a_t;
                        typedef fixed_point_t<int64_t, 8> b_t;
                        a_t a = scalar_cast <a_t> (-3.14159265);//{-((1<<8) | 127)};
                        b_t b = {(0<<8) | 0};
                        cout << "scalar_cast<>'s" << '\n';

                        cout << "a=" << a << ", type=" << typeid(a).name() << endl;
                        cout << "a=" << scalar_cast<a_t>(a) << ", type=" << typeid(scalar_cast<a_t>(a)).name() << endl;
                        cout << "a_0=" << scalar_cast<fixed_point_t<int64_t, 0> >(a) << endl;

                        cout << "a_1=" << scalar_cast<fixed_point_t<int64_t, 1> >(a) << endl;
                        cout << "a_2=" << scalar_cast<fixed_point_t<int64_t, 2> >(a) << endl;
                        cout << "a_3=" << scalar_cast<fixed_point_t<int64_t, 3> >(a) << endl;
                        cout << "a_4=" << scalar_cast<fixed_point_t<int64_t, 4> >(a) << endl;
                        cout << "a_5=" << scalar_cast<fixed_point_t<int64_t, 5> >(a) << endl;
                        cout << "a_6=" << scalar_cast<fixed_point_t<int64_t, 6> >(a) << endl;
                        cout << "a_7=" << scalar_cast<fixed_point_t<int64_t, 7> >(a) << endl;
                        cout << "a_8=" << scalar_cast<fixed_point_t<int64_t, 8> >(a) << endl;
                        cout << "a_9=" << scalar_cast<fixed_point_t<int64_t, 9> >(a) << endl;
                        cout << "a_10=" << scalar_cast<fixed_point_t<int64_t, 10> >(a) << endl;
                        cout << "a_11=" << scalar_cast<fixed_point_t<int64_t, 11> >(a) << endl;
                        cout << "a_12=" << scalar_cast<fixed_point_t<int64_t, 12> >(a) << endl;
                        cout << "a_13=" << scalar_cast<fixed_point_t<int64_t, 13> >(a) << endl;
                        cout << "a_16=" << scalar_cast<fixed_point_t<int64_t, 16> >(a) << endl;
                        cout << "a=" << scalar_cast<float>(a) << ", type=" << typeid(scalar_cast<float>(a)).name() << endl;
                        cout << "a=" << scalar_cast<double>(a) << ", type=" << typeid(scalar_cast<double>(a)).name() << endl;
                        cout << "a=" << scalar_cast<int>(a) << ", type=" << typeid(scalar_cast<int>(a)).name() << endl;

                        cout << "----" << '\n';
                }
        }

        long double d = b * c;
        cout << d << endl;
}
*/
