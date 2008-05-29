/***************************************************************************
 *            CloudShader_Bouthors.h
 *
 *  Fri May  2 11:02:00 2008
 *  Copyright  2008  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or (at your
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


// the following lines were my try to implement the cloud-shader by Antonoine Bouthors,
// but a) it failed, b) he did not write back when i mailed him my questions
// the code is just left for future reference, hence there is an explicit "#define _KLUDGES_H"
//
#ifndef _KLUDGES_H
#define _KLUDGES_H /* so this file is quasi-dead */
#endif

#ifndef _KLUDGES_H
#define _KLUDGES_H


#define float double
namespace cloud_system {
    class PerlinNoise {
        private:
            float Persistence;
            int   Iterations;
            int   ofs_x, ofs_y;
            bool  interpolate_noise;


            inline static float cosine_interpolate (float p, float q, float x) {
                float ft = x * 3.1415927;
                float f = (1.0 - cos (ft)) * 0.5;
                return  p* (1-f) + q*f;
            }


            inline float noise (int x, int y)  const {
                int n = (ofs_x+x) + (ofs_y+y) * 57;
                n = (n<<13) ^ n;
                return (1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
            }
            inline float smoothed_noise (float fx, float fy) const {
                int x = (int) fx;
                int y = (int) fy;
                if (interpolate_noise) {
                    const float corners = (noise (x-1, y-1) +noise (x+1, y-1) +noise (x-1, y+1) +noise (x+1, y+1)) / 16.0;
                    const float sides   = (noise (x-1, y)  +noise (x+1, y)  +noise (x, y-1)  +noise (x, y+1)) /  8.0;
                    const float center  =  noise (x, y) / 4.0;
                    return corners + sides + center;
                }
                return noise (x, y);
            }

            inline float interpolated_noise (float x, float y) const {
                const float trunc_x = (float) (int) x;
                const float trunc_y = (float) (int) y;
                const float frac_x  = x-trunc_x;
                const float frac_y  = y-trunc_y;
                const float a = smoothed_noise (trunc_x,   trunc_y);
                const float b = smoothed_noise (1+trunc_x,   trunc_y);
                const float c = smoothed_noise (trunc_x, 1+trunc_y);
                const float d = smoothed_noise (1+trunc_x, 1+trunc_y);
                const float A = cosine_interpolate (a, b, frac_x);
                const float B = cosine_interpolate (c, d, frac_x);
                return cosine_interpolate (A, B, frac_y);
            }

            float perlin (float x, float y) const {
                float r = 0;
                int n;
                float fn;
                for (fn=0, n=0; n<Iterations; fn+=1, n+=1) {
                    const float freq = powf (2.0,fn);
                    const float amp  = powf (Persistence,fn);
                    r = r + amp * interpolated_noise (x*freq, y*freq);
                }
                return r;
            }

        public:
            PerlinNoise (float persistence, int octaves, int _ofs_x, int _ofs_y, bool interpolation=true) :
                    Persistence (persistence),
                    Iterations (octaves-1),
                    ofs_x (_ofs_x), ofs_y (_ofs_y),
                    /** \todo user definition */ interpolate_noise (interpolation) {}

            PerlinNoise() : Persistence (1), Iterations (4), ofs_x (0), ofs_y (0) {}

            void  setPersistence (float pers)   {
                Persistence = pers;
            }
            float getPersistence()               {
                return Persistence;
            }
            void  setOctaves (unsigned int oct) {
                Iterations = oct-1;
            }
            int   getOctaves()                   {
                return Iterations+1;
            }
            void  setPerlinXOfs (int _ofs_x)    {
                ofs_x = _ofs_x;
            }
            int   getPerlinXOfs()                {
                return ofs_x;
            }
            void  setPerlinYOfs (int _ofs_y)    {
                ofs_y = _ofs_y;
            }
            int   getPerlinYOfs()                {
                return ofs_y;
            }

            float operator () (float u, float v) const {
                return perlin (10000.0+u, 10000.0+v);
            }
    };

// Implements a  cloud-structure in the manner of  Hugo Elias
// on his homepage (http://freespace.virgin.net/hugo.elias/).
// Note that we do not do any precalculation, rather we do it
// fully procedurrally, so we are not limited in memory-space
// and/or screen resolution.
    class PerlinBased {
        private:
            PerlinNoise m_noise;
            float m_scale;
            float m_ncover;
            float m_smoothness;
        public:

            PerlinBased (
                float cover,
                float persistence,
                float smoothness,
                float scale,
                unsigned int octaves,
                bool interpolation = true
            ) :
                    m_noise (persistence, octaves, 0, 0, interpolation),
                    m_scale (scale),
                    m_ncover (1-cover),
                    m_smoothness (smoothness) {
            }

            void  setCover (float f)        {
                m_ncover = 1.0 - f;
            }
            float getCover()                 {
                return 1.0 - m_ncover;
            }
            void  setPersistence (float f)  {
                m_noise.setPersistence (f);
            }
            float getPersistence()           {
                return m_noise.getPersistence();
            }
            void  setSmoothness (float f)   {
                m_smoothness = f;
            }
            float getSmoothness()            {
                return m_smoothness;
            }
            void  setScale (float f)        {
                m_scale = f;
            }
            float getScale()                 {
                return m_scale;
            }
            void  setOctaves (unsigned int o) {
                m_noise.setOctaves (o);
            }
            unsigned int getOctaves()        {
                return m_noise.getOctaves();
            }
            void  setPerlinXOfs (int o)    {
                m_noise.setPerlinXOfs (o);
            }
            int   getPerlinXOfs()           {
                return m_noise.getPerlinXOfs();
            }
            void  setPerlinYOfs (int o)    {
                m_noise.setPerlinYOfs (o);
            }
            int   getPerlinYOfs()           {
                return m_noise.getPerlinYOfs();
            }


            float operator () (float x, float y) const {
                const float f = 0.5+0.5*m_noise (m_scale*x, m_scale*y);  //> get noise and map [-1..1] to [0..1]
                const float c = f<m_ncover ? 0 : f-m_ncover;
                const float t = 1 - powf (m_smoothness, c);
                return t;
            }
    };





    class CloudShader {
            real /*H,*/ K, r_e, N_0, P_f, theta_f;
            picogen::graphics::objects::abstract::ISky *sky_shader;

            real lot_b[10];
            real lot_c[10];
            real lot_K_c[10];
            real lot_t[10];
            real lot_r[10];

            real P (real theta) const {
                // i am not sure about this, maybe one of the two:
                // -> http://scienceworld.wolfram.com/physics/PhaseFunction.html
                // -> http://omlc.ogi.edu/pubs/prahl-pubs/prahl88/node10.html
                return 1.0 / (4.0 * picogen::misc::constants::pi);
            }

            real P_S (real theta) const {
                return P (theta) / (1.0-P_f);
                return (theta<theta_f) ? (P (theta) / (1.0-P_f)) : 0.0;
            }
            real P_F (real theta) const {
                return P (theta) / P_f;
                return (theta<theta_f) ? (P (theta) / P_f) : 0.0;
            }

            real lookup (real *lot, real theta) const {
                const int u = ( (int) (theta * (180.0/picogen::misc::constants::pi))) / 10;
                return lot[
                           u < 0 ? 0
                           : u > 9 ? 9
                           : u
                       ];
            }

            template <typename T> T min (T f, T g) {
                return f<=g?f:g;
            }
            template <typename T> T max (T f, T g) {
                return f>=g?f:g;
            }

        public:
            CloudShader() {
                //H     = 100;              // 100 meters
                theta_f = 5.0 * (picogen::misc::constants::pi/180.0);
                N_0     = 300 * 1000000.0;  // 300/cm^3
                r_e     = 7.0 / 1000000.0;   // 7 micro-meters
                P_f     = 0.51;             // 51% for typical configurations
                real sigma = (1.0 - P_f) * picogen::misc::constants::pi * r_e * r_e;
                K = sigma*N_0;

                lot_b[0]=1.1796;
                lot_c[0]=0.0138;
                lot_K_c[0]=0.0265;
                lot_t[0]=0.8389;
                lot_r[0]=0.0547;
                lot_b[1]=1.1293;
                lot_c[1]=0.0154;
                lot_K_c[1]=0.0262;
                lot_t[1]=0.8412;
                lot_r[1]=0.0547;
                lot_b[2]=1.1382;
                lot_c[2]=0.0131;
                lot_K_c[2]=0.0272;
                lot_t[2]=0.8334;
                lot_r[2]=0.0552;
                lot_b[3]=1.0953;
                lot_c[3]=0.0049;
                lot_K_c[3]=0.0294;
                lot_t[3]=0.8208;
                lot_r[3]=0.0564;
                lot_b[4]=0.9808;
                lot_c[4]=0.0012;
                lot_K_c[4]=0.0326;
                lot_t[4]=0.8010;
                lot_r[4]=0.0603;
                lot_b[5]=0.9077;
                lot_c[5]=0.0047;
                lot_K_c[5]=0.0379;
                lot_t[5]=0.7774;
                lot_r[5]=0.0705;
                lot_b[6]=0.7987;
                lot_c[6]=0.0207;
                lot_K_c[6]=0.0471;
                lot_t[6]=0.7506;
                lot_r[6]=0.0984;
                lot_b[7]=0.6629;
                lot_c[7]=0.0133;
                lot_K_c[7]=0.0616;
                lot_t[7]=0.7165;
                lot_r[7]=0.1700;
                lot_b[8]=0.5043;
                lot_c[8]=0.0280;
                lot_K_c[8]=0.0700;
                lot_t[8]=0.7149;
                lot_r[8]=0.3354;
                lot_b[9]=0.3021;
                lot_c[9]=0.0783;
                lot_K_c[9]=0.0700;
                lot_t[9]=0.1000;
                lot_r[9]=0.9500;
            }

            void SetSky (picogen::graphics::objects::abstract::ISky *skyShader) {
                sky_shader = skyShader;
            }

            Color
            //real
            Shade (real &alpha, const Vector3d &_V, const Vector3d &_N, const Vector3d &_L, const real H) {
                const Vector3d V  = _V.normal();
                const Vector3d L  = _L.normal();
                const Vector3d N  = _N.normal();

                const real   theta_v  = acos (V * N);  // cos ( angle(V,N) )
                const real   theta_l  = acos (L * N);  // cos ( angle(V,N) )
                const real   theta_vl = acos (V * L);  // cos ( angle(V,N) )

                const real   mu_v   = fabs (cos (theta_v));  //V * N; // cos ( angle(V,N) )
                const real   mu_l   = fabs (cos (theta_l));  //L * N;
                const real   mu_v_minus_mu_l = fabs (mu_v-mu_l);

                //std::cout << theta_v << " " << theta_l << " " << mu_v << " " << mu_l << std::endl;
                //std::cout << V[0] << " " << V[1] << " " << V[2] << std::endl;

                const real    H_v   = fabs (H / mu_v); //> the paper did mention ("thickness", so I take the absolute here)
                const real    H_l   = fabs (H / mu_l); //>  . . . . . . .
                //const real    H_v_minus_H_l = fabs(H_v-H_l); //>  . . . . . . .

                //printf( "H_v=%f, H_l=%f, mu_v=%f, mu_l=%f\n", (float)H_v, (float)H_l, (float)mu_v, (float)mu_l );

                const real ext_Hv     = exp (-K*H_v);  // extinction( H_v )
                const real ext_Hl     = exp (-K*H_l);  // extinction( H_l )
                const real ext_Hl_plus_Hv = exp (-K* (H_l + H_v));
                //std::cout << ext_Hv << ":" << ext_Hl << " -- " << std::endl;

                alpha = 1.0 - ext_Hv;
                real I;

                if (0 && V*L>=0.0) {
                    //> single scattering
                    const real   I1_r   = ( (K*P (theta_vl) *mu_l) / (mu_v+mu_l)) * (1-ext_Hl_plus_Hv);
                    //> double scattering
                    const real   I2_r = ( (pow (K,2) *pow (P (theta_vl) *mu_l,2)) / (mu_v+mu_l)) * (1-ext_Hl_plus_Hv);

                    I = I1_r + I2_r;
                } else {


                    //> 0-scattering
                    const real  I0_td   = ext_Hv;// * ( (V-L).lengthSq()<=epsilon ? 1.0 : 0.0 ); // ext_Hv * kronecker(V-L)
                    const real  I0_tF   = ext_Hv * pow (P_F (theta_vl), K*H_v);
                    const real   I0_t   = I0_td + I0_tF;
                    /*printf( "I0_td=%f, I0_tF=%f*pow(%f,%f*%f)=%f, I0_t=%f\n",
                     (float)I0_td,
                     ext_Hv, P_F(theta_vl), K, H_v, (float)I0_tF,
                     (float)I0_t
                    );*/


                    //> single scattering
                    const real i1_t_A = K*P (theta_vl) *mu_l;
                    const real i1_t_B = mu_v_minus_mu_l;
                    const real i1_t_C = fabs (ext_Hv-ext_Hl);
                    const real   I1_t   = max (0.0, double ( (i1_t_A / i1_t_B) * i1_t_C));
                    /*printf(
                     "I1_t = ( (%f) / (%f) ) * (%f) = %f, A/B=%f\n",
                     (float)(i1_t_A), (float)(i1_t_B),  (float)i1_t_C, (float)I1_t, (float)(i1_t_A/i1_t_B)
                    );//*/


                    //> double scattering
                    const real   I2_t = max (0.0, ( (pow (K,2) * pow (P (theta_vl),2) * mu_l) / (mu_v-mu_l)) * (ext_Hv-ext_Hl));
                    /*
                    printf( "I2_t = ( (A) / (%f) ) * (%f) = %f\nA = %f*%f*%f = %f\n\n",
                     mu_v-mu_l,  ext_Hv-ext_Hl, I1_t,
                     pow(K,2), pow(P(theta_vl),2), mu_l, (pow(K,2) * pow(P(theta_vl),2) * mu_l)
                    );//*/


                    //> 3+ scattering
                    /*
                    const real b = lookup( lot_b, theta_l );
                    const real c = lookup( lot_c, theta_l );
                    const real t = lookup( lot_t, theta_l );
                    const real K_c = lookup( lot_K_c, theta_l );
                    const real r = lookup( lot_r, theta_l );

                    const real s = K_c*exp(-K_c*H);
                    const real beta = 0.9961;

                    const real ext_H      = exp( -K_c*H ); // extinction( H_v )
                    const real ext_2H     = exp( -K_c*2*H ); // extinction( H_v )
                    const real ext_3H     = exp( -K_c*3*H ); // extinction( H_v )
                    */
                    //const real ext_Hl     = exp( -K_c*H_l ); // extinction( H_v )

                    /*
                    const real T0  = ext_Hl;
                    const real T1  = t*H*s;
                    const real T2a = 0.5*pow(K_c*t*H,2)*ext_H;
                    const real T2b = 0.25*r*r * ( 1 + ( 2*H*K_c - 1 )*ext_2H) * ext_3H;
                    const real T2  = T2a + T2b;
                    const real Tms = (b + (1-b)*exp(-c*H)) * ( beta / (H-(H-1)*beta ) );
                    const real T3 = Tms - T0 - T1 - T2;
                    */
                    //std::cout << T0 << ", " << T1 << ", " << T2 << ", " << T3 << ", " << Tms << std::endl;
                    //std::cout << T2a << ", " << T2b<< std::endl << std::endl;
                    //std::cout << b << ", " << c << ", " << t << ", " << K_c << ", " << r << "(" << theta_l << ")" << std::endl;
                    //const real I3_t = T3 + mu_l / (4*pi*mu_v);


                    //> sum together
                    // I = I0_t+I1_t+I2_t+I3_t;
                    I =  I0_t + I1_t + I2_t;// + 1000000*I2_t;
                    //printf( "It{1,2,3,4}=%f, %f, %f, %f\n", (float)I0_t, (float)I1_t, (float)I2_t, (float)I3_t );
                }
                real f = (I) *1;
                f = f < 0.0 ? 0.0 : f;
                return Color (f, f, f); //xrt::base_types::color( I0_t, I0_t, I0_t );
            }
    };

    struct CClouds : public picogen::graphics::objects::abstract::IIntersectable {
private:

private:
        Vector3d x;
        real     r;//, L_e;
        Color    color;
        Lambertian lambertian;

        cloud_system::PerlinBased perlin_sky;
        picogen::graphics::objects::Preetham *sky_shader;
        CloudShader *cloud_shader;

public:

        CClouds (real planet_radius, real height) :
                x (0.0,0.0,0.0),
                r (planet_radius+height),
                //L_e(0.2),
                color (1.0,1.0,1.0),
                lambertian (0.0),
                perlin_sky (
                    0.5,   // cover -> 0.0==no clouds, 0.5==sky partially covered, 1.0==sky covered almost completely
                    0.8,    // persistence -> the smaller the smoother, around 0..1, but feel free to try out other values
                    0.1,   // smoothness
                    8*0.0008, 8, // scale and octave
                    0 // use interpolation (slooooow)
                ),
                sky_shader (NULL),
                cloud_shader (NULL) {
            perlin_sky.setPerlinXOfs (20);
        }

        void SetSkyShader (picogen::graphics::objects::Preetham *skyShader) {
            sky_shader = skyShader;
        }

        void SetCloudShader (CloudShader *cloudShader) {
            cloud_shader = cloudShader;
        }

        virtual bool Intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
            using namespace picogen::misc::constants;

            side currSide = noside;
            int i;
            Vector3d v;
            real sqrV = 0;
            real b    = 0;

            for (i=0; i<3; i++) {
                v[i]  = ray.x() [i] - this->x[i];
                b    -= v[i] * ray.w() [i];
                sqrV += v[i] * v[i];
            }

            real det = (b * b) - sqrV + r*r;
            real i1, i2;

            real currT = real_max;
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
            if (currT>epsilon && currSide != noside) {
                intersection.t = currT;
                const Vector3d i = ray (intersection.t);
                const float perlin = (double) perlin_sky (i[0],i[2]);
                intersection.normal = ( (currSide==inside) ? (x-i) : (i-x)).normal();
                intersection.brdf = &lambertian;

                if (sky_shader != NULL && cloud_shader != NULL) {
                    const Vector3d L = sky_shader->GetSunDirection();
                    //intersection.L_e = 0.0;//intersection.L_e < 0.0 ? 0.0 : intersection.L_e;
                    real alpha;
                    intersection.L_e = 0.0;
                    intersection.color =
                        sky_shader->GetSunColor()
                        * cloud_shader->Shade (
                            alpha,
                            -ray.w().normal(),
                            /*vector3d(0,-1,0),*/ intersection.normal,
                            -L,
                            perlin*150.0
                        )
                        ;
                    //printf( "%f ", (float)perlin );
                    if ( ( (double) rand() / (double) RAND_MAX) > alpha)
                        return false;
                    /*real a,b,c;
                    intersection.color.to_rgb( a, b, c );
                    intersection.L_e = 0.0;//a*a + b*b + c*c;
                    a = 10;//a/intersection.L_e;
                    b = 10;//b/intersection.L_e;
                    c = 10;//c/intersection.L_e;
                    intersection.color.from_rgb( a, b, c );*/

                    //intersection.color = xrt::base_types::color(0,I,0);
                }
                return true;
            }
            return false;
        }
    };



#undef float

}; // namespace cloud_system

#endif /* _KLUDGES_H */
