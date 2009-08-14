

/***************************************************************************
 *            CloudsBouthors.h
 *
 *  2008-05-27 13:00:00
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

#ifndef _CLOUDSBOUTHORS_H
#define _CLOUDSBOUTHORS_H

namespace picogen {
    namespace graphics {
        namespace objects {

//class CloudShader
//{
//  real /*H,*/ K, r_e, N_0, P_f, theta_f;
//  picogen::graphics::objects::abstract::ISky *sky_shader;
//
//  real lot_b[10];
//  real lot_c[10];
//  real lot_K_c[10];
//  real lot_t[10];
//  real lot_r[10];
//
//  real P( real theta ) const
//  {
//   // i am not sure about this, maybe one of the two:
//   // -> http://scienceworld.wolfram.com/physics/PhaseFunction.html
//   // -> http://omlc.ogi.edu/pubs/prahl-pubs/prahl88/node10.html
//   return 1.0 / ( 4.0 * picogen::constants::pi );
//  }
//
//  real P_S( real theta ) const
//  {
//   //return P(theta) / (1.0-P_f);
//   //return (theta<theta_f) ? ( P(theta) / (1.0-P_f) ) : 0.0;
//   return (theta<theta_f) ? ( P(theta) / (1.0-P_f) ) : 0.0;
//  }
//  real P_F( real theta ) const
//  {
//   //return P(theta) / P_f;
//   return (theta<theta_f) ? ( P(theta) / P_f ) : 0.0;
//  }
//
//  real lookup( real *lot, real theta ) const
//  {
//   const int u = ( (int)( theta * (180.0/picogen::constants::pi) ) ) / 10;
//   return lot[
//         u < 0 ? 0
//       : u > 9 ? 9
//       : u
//   ];
//  }
//
//  template <typename T> T min( T f, T g ) const{ return f<=g?f:g; }
//  template <typename T> T max( T f, T g ) const{ return f>=g?f:g; }
//
// public:
//  CloudShader()
//  {
//   //H     = 100;              // 100 meters
//   theta_f = 5.0 * (picogen::constants::pi/180.0);
//   N_0     = 300 * 1000000.0;  // 300/cm^3
//   r_e     = 7.0 / 1000000.0;   // 7 micro-meters
//   P_f     = 0.51;             // 51% for typical configurations
//   real sigma = ( 1.0 - P_f ) * picogen::constants::pi * r_e * r_e;
//   K = sigma*N_0;
//
//   lot_b[0]=1.1796;  lot_c[0]=0.0138;  lot_K_c[0]=0.0265;  lot_t[0]=0.8389;  lot_r[0]=0.0547;
//   lot_b[1]=1.1293;  lot_c[1]=0.0154;  lot_K_c[1]=0.0262;  lot_t[1]=0.8412;  lot_r[1]=0.0547;
//   lot_b[2]=1.1382;  lot_c[2]=0.0131;  lot_K_c[2]=0.0272;  lot_t[2]=0.8334;  lot_r[2]=0.0552;
//   lot_b[3]=1.0953;  lot_c[3]=0.0049;  lot_K_c[3]=0.0294;  lot_t[3]=0.8208;  lot_r[3]=0.0564;
//   lot_b[4]=0.9808;  lot_c[4]=0.0012;  lot_K_c[4]=0.0326;  lot_t[4]=0.8010;  lot_r[4]=0.0603;
//   lot_b[5]=0.9077;  lot_c[5]=0.0047;  lot_K_c[5]=0.0379;  lot_t[5]=0.7774;  lot_r[5]=0.0705;
//   lot_b[6]=0.7987;  lot_c[6]=0.0207;  lot_K_c[6]=0.0471;  lot_t[6]=0.7506;  lot_r[6]=0.0984;
//   lot_b[7]=0.6629;  lot_c[7]=0.0133;  lot_K_c[7]=0.0616;  lot_t[7]=0.7165;  lot_r[7]=0.1700;
//   lot_b[8]=0.5043;  lot_c[8]=0.0280;  lot_K_c[8]=0.0700;  lot_t[8]=0.7149;  lot_r[8]=0.3354;
//   lot_b[9]=0.3021;  lot_c[9]=0.0783;  lot_K_c[9]=0.0700;  lot_t[9]=0.1000;  lot_r[9]=0.9500;
//  }
//
//  void SetSky( picogen::graphics::objects::abstract::ISky *skyShader ) { sky_shader = skyShader; }
//
//  Color
//  //real
//  Shade( real &alpha, const Vector3d &_V, const Vector3d &_N, const Vector3d &_L, const real H )
//  const
//  {
//      using namespace std;
//   const Vector3d V  = _V.normal();
//   const Vector3d L  = _L.normal();
//   const Vector3d N  = _N.normal();
//
//   const real   theta_v  = acos( V * N ); // cos ( angle(V,N) )
//   const real   theta_l  = acos( L * N ); // cos ( angle(V,N) )
//   const real   theta_vl = acos( V * L ); // cos ( angle(V,N) )
//
//   const real   mu_v   = fabs( cos( theta_v ) );//V * N; // cos ( angle(V,N) )
//   const real   mu_l   = fabs( cos( theta_l ) );//L * N;
//   const real   mu_v_minus_mu_l = fabs( mu_v-mu_l );
//
//   //std::cout << theta_v << " " << theta_l << " " << mu_v << " " << mu_l << std::endl;
//   //std::cout << V[0] << " " << V[1] << " " << V[2] << std::endl;
//
//   const real    H_v   = fabs(H / mu_v); //> the paper did mention ("thickness", so I take the absolute here)
//   const real    H_l   = fabs(H / mu_l); //>  . . . . . . .
//   //const real    H_v_minus_H_l = fabs(H_v-H_l); //>  . . . . . . .
//
//   //printf( "H_v=%f, H_l=%f, mu_v=%f, mu_l=%f\n", (float)H_v, (float)H_l, (float)mu_v, (float)mu_l );
//
//   const real ext_Hv     = exp( -K*H_v ); // extinction( H_v )
//   const real ext_Hl     = exp( -K*H_l ); // extinction( H_l )
//   const real ext_Hl_plus_Hv = exp( -K*( H_l + H_v ) );
//   //std::cout << ext_Hv << ":" << ext_Hl << " -- " << std::endl;
//
//   alpha = 1.0 - ext_Hv;
//   real I;
//
//   if( 0 && V*L>=0.001 ){
//    //> single scattering
//    //const real   I1_r = ( (K*P(theta_vl)*mu_l) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);
//    const real   I1_r = ( (P(theta_vl)*mu_l) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);
//    //> double scattering
//    //const real   I2_r = 0; // ( (pow(K,2)*pow(P(theta_vl)*mu_l,2)) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);
//    const real   I2_r = 0; // ( (pow(P(theta_vl)*mu_l,2)) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);
//
//                I = 0;
//                I += I1_r;
//                I += I2_r;
//   }else{
//
//
//    //> 0-scattering
//    const real  I0_td   = ext_Hv * ( (V-L).lengthSq()<=0.001 ? 1.0 : 0.0 ); // ext_Hv * kronecker(V-L)
//    const real  I0_tF   = ext_Hv * pow( P_F( theta_vl ), K*H_v );
//    const real  I0_t    = I0_td + I0_tF;
//    //cerr << I0_td << ":" << I0_tF << endl;
//
//    //> single scattering
//    //const real i1_t_A = K*P(theta_vl)*mu_l;
//    const real i1_t_A = P(theta_vl)*mu_l;
//    const real i1_t_B = mu_v_minus_mu_l;
//    const real i1_t_C = fabs(ext_Hv-ext_Hl);
//    const real   I1_t = max( 0.0, double(( i1_t_A / i1_t_B ) * i1_t_C) );
//
//    //> double scattering
//    //const real   I2_t = max( 0.0, ( (pow(K,2) * pow(P(theta_vl),2) * mu_l) / (mu_v-mu_l) ) * (ext_Hv-ext_Hl) );
//    const real   I2_t = max( 0.0, ( (pow(P(theta_vl),2) * mu_l) / (mu_v-mu_l) ) * (ext_Hv-ext_Hl) );
//
//    //> 3+ scattering
//    /*
//    const real b = lookup( lot_b, theta_l );
//    const real c = lookup( lot_c, theta_l );
//    const real t = lookup( lot_t, theta_l );
//    const real K_c = lookup( lot_K_c, theta_l );
//    const real r = lookup( lot_r, theta_l );
//
//    const real s = K_c*exp(-K_c*H);
//    const real beta = 0.9961;
//
//    const real ext_H      = exp( -K_c*H ); // extinction( H_v )
//    const real ext_2H     = exp( -K_c*2*H ); // extinction( H_v )
//    const real ext_3H     = exp( -K_c*3*H ); // extinction( H_v )
//    */
//    //const real ext_Hl     = exp( -K_c*H_l ); // extinction( H_v )
//
//    /*
//    const real T0  = ext_Hl;
//    const real T1  = t*H*s;
//    const real T2a = 0.5*pow(K_c*t*H,2)*ext_H;
//    const real T2b = 0.25*r*r * ( 1 + ( 2*H*K_c - 1 )*ext_2H) * ext_3H;
//    const real T2  = T2a + T2b;
//    const real Tms = (b + (1-b)*exp(-c*H)) * ( beta / (H-(H-1)*beta ) );
//    const real T3 = Tms - T0 - T1 - T2;
//    */
//    //std::cout << T0 << ", " << T1 << ", " << T2 << ", " << T3 << ", " << Tms << std::endl;
//    //std::cout << T2a << ", " << T2b<< std::endl << std::endl;
//    //std::cout << b << ", " << c << ", " << t << ", " << K_c << ", " << r << "(" << theta_l << ")" << std::endl;
//    //const real I3_t = T3 + mu_l / (4*pi*mu_v);
//
//
//    //> sum together
//    // I = I0_t+I1_t+I2_t+I3_t;
//    //I =  I0_t + I1_t + I2_t;// + 1000000*I2_t;
//    I = 0;
//    I += 0*I0_t;
//    I += 1*I1_t;
//    I += 0*I2_t;
//   }
//   real f = (I)*1;
//   f = f < 0.0 ? 0.0 : f;
//   return Color( f, f, f );//xrt::base_types::color( I0_t, I0_t, I0_t );
//  }
//};


            class CloudShader {

                private:
                    /// \todo only typedef in the actually needed types
                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;
                    typedef ::picogen::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                    typedef ::picogen::graphics::objects::abstract::ISky ISky;
                    typedef ::picogen::graphics::color::Color Color;
                    typedef ::picogen::geometrics::Transformation Transformation;

                private:
                    const real theta_f;
                    const real N_0;
                    const real r_e;
                    const real P_f;
                    const real sigma_s;
                    const real K;

                private:

                    real P (real theta) const {
                        // i am not sure about this, maybe one of the two:
                        // -> http://scienceworld.wolfram.com/physics/PhaseFunction.html
                        // -> http://omlc.ogi.edu/pubs/prahl-pubs/prahl88/node10.html
                        return 1.0 / (4.0 * picogen::constants::pi);
                    }

                    real P_S (real theta) const {
                        return theta < theta_f
                               ? P (theta) / (1.0-P_f)
                               : 0.0
                               ;
                    }

                    real P_F (real theta) const {
                        return theta < theta_f
                               ? P (theta) / P_f
                               : 0.0
                               ;
                    }

                    real kronecker (real theta) const {
                        // note that the paper mentions the function wrt ||V-L||,
                        // but with that we have a 'black information hole', so we compare
                        // angles instead (like in P_F() and P_S())
                        return theta < theta_f ? 1.0 : 0.0;
                    }

                    real tau (real x) const {
                        return exp (-K * x);
                    }

                    real s (real x) const {
                        return K * exp (-K * x);
                    }

                public:

                    CloudShader()
                            :
                            theta_f (5.0 * (picogen::constants::pi/180.0)),
                            N_0 (300 * 1000000.0),       // 300/cm^3
                            r_e (7.0 / 1000000.0),       // 7 micro-meters
                            P_f (0.51),                  // 51% for typical configurations
                            sigma_s ( (1.0 - P_f) * picogen::constants::pi * r_e * r_e),
                            K (sigma_s  * N_0) {         // sigma * N_0
                    }


                    Color shade (real &alpha, const Vector3d &_V, const Vector3d &_N, const Vector3d &_L, const real H) const {
                        using namespace std;
                        const Vector3d V  = _V.normal();
                        const Vector3d L  = _L.normal();
                        const Vector3d N  = _N.normal();

                        const real   theta_v  = acos (V * N);  // cos ( angle(V,N) )
                        const real   theta_l  = acos (L * N);  // cos ( angle(V,N) )
                        const real   theta_vl = acos (V * L);  // cos ( angle(V,N) )

                        const real   mu_v   = fabs (cos (theta_v));  //V * N; // cos ( angle(V,N) )
                        const real   mu_l   = fabs (cos (theta_l));  //L * N;
//            const real   mu_v_minus_mu_l = mu_v-mu_l;

                        const real    H_v   = fabs (H / mu_v); //> the paper did mention ("thickness", so I take the absolute here)
                        const real    H_l   = fabs (H / mu_l); //>  . . . . . . .

                        //> here we go
                        // const real I0_td = tau( H_v ) * (1.0-kronecker( theta_vl ));
                        // const real I0_tF = tau( H_v ) * pow( P_F(theta_vl), K*H_v );
                        // const real I0_t  = I0_td + I0_tF;

                        //==-----------------------------------------------------------------------------
                        //> i tried and tried, but the above 3 lines only yield unplausible 0-scatter
                        //> notes: for H < 1, I0_td and I0_tF yield similar data, but for H > 5,
                        //         I0_tF (the one within the sun-angle) is far darker
                        //==-----------------------------------------------------------------------------
#if 0
#define dump(x) cout << x << ", ";
#define dumpnl() cout << endl;
#else
#define dump(x) ;
#define dumpnl() ;
#endif

                        const real I0_t = tau (H_v);


                        /*
                        const real I1_r = ( (P(theta_vl)*mu_l) / (mu_v+mu_l) )  *  (1.0-tau(H_l+H_v));
                        const real I1_t = ( (P(theta_vl)*mu_l) / (mu_v-mu_l) )  *  ( tau(H_v) - tau(H_l) );
                        */

                        const real I1_r = ( ( (P (theta_vl) / (1.0-P_f)) *mu_l) / (mu_v+mu_l))  * (1.0-tau (H_l+H_v));
                        const real I1_t = ( ( (P (theta_vl) / (1.0-P_f)) *mu_l) / (mu_v-mu_l))  * (tau (H_v) - tau (H_l));
                        if (0) if (rand() % 10000) printf ("%.5f,%.5f\n", I1_r, I1_t);

                        real I = 0
                                 + I1_r * 1
                                 ;

                        alpha = 1 - 0*I0_t;
                        return Color (1,1,1) *I;
                    }

            };

            class CloudsBouthors : public picogen::graphics::objects::abstract::IIntersectable {
                private:
                    /// \todo only typedef in the actually needed types
                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;
                    typedef ::picogen::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                    typedef ::picogen::graphics::objects::abstract::ISky ISky;
                    typedef ::picogen::graphics::color::Color Color;
                    typedef ::picogen::geometrics::Transformation Transformation;


                private:
                    const picogen::graphics::objects::Preetham *sky;
                    const picogen::graphics::objects::abstract::IIntersectable *intersectable;
                    CloudShader cloudShader;

                public:

                    CloudsBouthors (
                        const picogen::graphics::objects::Preetham *sky = 0,
                        const picogen::graphics::objects::abstract::IIntersectable *intersectable = 0
                    ) : sky (sky), intersectable (intersectable), cloudShader() {
                    }

                    void setSky (const picogen::graphics::objects::Preetham *sky) {
                        this->sky = sky;
                    }

                    void setIntersectable (const picogen::graphics::objects::abstract::IIntersectable *intersectable) {
                        this->intersectable = intersectable;
                    }

                    virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
                        if (!intersectable->intersect (intersection, ray))
                            return false;

                        //const Vector3d L = sky->getSunDirection();
                        Vector3d L;
                        sky->getSunDirection (L);
                        //intersection.L_e = 0.0;//intersection.L_e < 0.0 ? 0.0 : intersection.L_e;
                        real alpha;
                        intersection.L_e = 0.0;
                        intersection.color =
                            //sky->GetSunColor() *
                            cloudShader.shade (
                                alpha,
                                -ray.w().normal(),  // V_
                                intersection.normal, // N_
                                -L,  // L_
                                100 // H_
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
                        return true;
                    }

//    CClouds( real planet_radius, real height ) :
//        x(0.0,0.0,0.0),
//        r(planet_radius+height),
//        //L_e(0.2),
//        color(1.0,1.0,1.0),
//        lambertian(0.0),
//        perlin_sky(
//            0.5,   // cover -> 0.0==no clouds, 0.5==sky partially covered, 1.0==sky covered almost completely
//            0.8,    // persistence -> the smaller the smoother, around 0..1, but feel free to try out other values
//            0.1,   // smoothness
//            8*0.0008, 8, // scale and octave
//            0 // use interpolation (slooooow)
//        ),
//        sky_shader(0),
//        cloud_shader(0)
//    {
//        perlin_sky.setPerlinXOfs( 20 );
//    }
//
//    void setSky( picogen::graphics::objects::Preetham *skyShader )
//    {
//        sky_shader = skyShader;
//    }
//
//    virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const
//    {
//        using namespace picogen::constants;
//
//        side currSide = noside;
//        int i;
//        Vector3d v;
//        real sqrV = 0;
//        real b    = 0;
//
//        for( i=0; i<3; i++ ){
//            v[i]  = ray.x()[i] - this->x[i];
//            b    -= v[i] * ray.w()[i];
//            sqrV += v[i] * v[i];
//        }
//
//        real det = (b * b) - sqrV + r*r;
//        real i1, i2;
//
//        real currT = real_max;
//        if (det > 0){
//            det = sqrt( det );
//            i1 = b - det;
//            i2 = b + det;
//            if ( i2>0 ){
//                if( i1<0 ){
//                    currT = i2;
//                    currSide = inside;
//                }else{
//                    currT = i1;
//                    currSide = outside;
//                }
//            }
//        }
//        if( currT>epsilon && currSide != noside ){
//            intersection.t = currT;
//            const Vector3d i = ray(intersection.t);
//            const float perlin = (double)perlin_sky(i[0],i[2]);
//            intersection.normal = ( (currSide==inside)?(x-i):(i-x) ).normal();
//            intersection.brdf = &lambertian;
//
//            if( sky_shader != 0 && cloud_shader != 0 ){
//                const Vector3d L = sky_shader->GetSunDirection();
//                //intersection.L_e = 0.0;//intersection.L_e < 0.0 ? 0.0 : intersection.L_e;
//                real alpha;
//                intersection.L_e = 0.0;
//                intersection.color =
//                    sky_shader->GetSunColor()
//                    * cloud_shader->Shade(
//                        alpha,
//                        -ray.w().normal(),
//                        /*vector3d(0,-1,0),*/ intersection.normal,
//                        -L,
//                        perlin*150.0
//                    )
//                ;
//                //printf( "%f ", (float)perlin );
//                if( ( (double)rand() / (double)RAND_MAX ) > alpha )
//                    return false;
//                /*real a,b,c;
//                intersection.color.to_rgb( a, b, c );
//                intersection.L_e = 0.0;//a*a + b*b + c*c;
//                a = 10;//a/intersection.L_e;
//                b = 10;//b/intersection.L_e;
//                c = 10;//c/intersection.L_e;
//                intersection.color.from_rgb( a, b, c );*/
//
//                //intersection.color = xrt::base_types::color(0,I,0);
//            }
//            return true;
//        }
//        return false;
//    }
            };

        }
    }
} // namespace picogen{ namespace graphics{ namespace objects{


#endif // ifdef _CLOUDSBOUTHORS_H
