/***************************************************************************
 *            main.cc
 *
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
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
#include "picogen.h"
#include "picogen/kludges.h"
#include "picossdf/picossdf.h"

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

#include "picogen.h"
#include "picogen/kludges.h"
#include "compile_info.h"


void dump_call_stack()
{
	std::cout << "call stack: " << std::endl;
	std::string indent(".");
	int u;
	for( u=0; u<XRT_CALL_STACK_LENGTH(); u++ ){
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
)
{
	if( SDL_MUSTLOCK(p_target) && SDL_LockSurface(p_target)<0 )
		return;
	int x,y;
	for( y=0; y<p_target->h; y++ ){
	    // TODO FIX we are currently assuming a 32bit SDL buffer
	    // TODO get rid of pointer arithmetic
		Uint32 *bufp   = (Uint32*)p_target->pixels + y*(p_target->pitch>>2);
		for( x=0; x<p_target->w; x++ ){
			/*float d = 1.0 / source->density;
			float r = source->color[0]*scale*d;
			float g = source->color[1]*scale*d;
			float b = source->color[2]*scale*d;*/
			real accu_r=real(0), accu_g=real(0), accu_b=real(0);
			for( register int u=0; u<2; ++u ) for( register int v=0; v<2; ++v ){
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


	if( SDL_MUSTLOCK(p_target) )
		SDL_UnlockSurface(p_target);
	SDL_Flip(p_target);
}




struct Lambertian : public picogen::graphics::material::abstract::IBRDF
{
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
	) const
	{
		using picogen::misc::constants::pi;
		/*if( (static_cast<real>( rand() % 10000 ) / 10000.0)>0.9 )
			return false;*/
		r_out.x() = r_in.x();
		p = 1.0;
		brdf = 1;
		if( ((rand()%10000)/10000.0) < mirror ){
			r_out.w() = r_in.w() - N.normal()*2*(r_in.w()*N.normal());

			p = fabs(N.normal()*r_out.w().normal());
			brdf = 1.0;
			specular = true;
		}else{
			do{
				r_out.w() = Vector3d(
					static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
					static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
					static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0
				);
			}while( r_out.w().lengthSq()>1 || N*r_out.w()<0.0 );

			r_out.w() = r_out.w().normal();
			p = (1.0-mirror)/(2.0*pi);// / xrt::constants::pi;
			brdf = 1.0/pi;// / xrt::constants::pi;//r_out.w().normal() * N;// / xrt::constants::pi;
			specular = false;
		}
		return true;
	}
};


struct WaterBRDF : public picogen::graphics::material::abstract::IBRDF
{
	private:
		real *drop_x;
		real *drop_z;
		real *amplitude;
		real *frequency;
		real attenuation;
		unsigned int num_drops;
	public:
		WaterBRDF() : drop_x(NULL), drop_z(NULL), amplitude(NULL), frequency(NULL), num_drops(0) {}

		virtual ~WaterBRDF()
		{
			if( drop_x != NULL ) delete drop_x; drop_x = NULL;
			if( drop_z != NULL ) delete drop_z; drop_z = NULL;
			if( amplitude != NULL ) delete amplitude; amplitude = NULL;
			if( frequency != NULL ) delete frequency; frequency = NULL;
			num_drops = 0;
		}
		void Init( unsigned int n, real l, real f, real r, real b, real attenuation )
		{
			if( drop_x != NULL ) delete drop_x;
			if( drop_z != NULL ) delete drop_z;
			if( amplitude != NULL ) delete amplitude; amplitude = NULL;
			if( frequency != NULL ) delete frequency; frequency = NULL;
			drop_x = new real[n];
			drop_z = new real[n];
			amplitude = new real[n];
			frequency = new real[n];
			num_drops = n;
			for( unsigned int u=0; u<n; u++ ){
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
		) const
		{
			using picogen::misc::constants::pi;

			r_out.x() = r_in.x();
			//real f = 0.5+0.4*sin( r_in.x()[0]*0.05 );
			const real x = r_in.x()[0];
			const real z = r_in.x()[2];
			real f = 0.0;
			for( unsigned int u=0; u<num_drops; u++ ){
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





class CloudShader
{
		real /*H,*/ K, r_e, N_0, P_f, theta_f;
		picogen::graphics::objects::abstract::ISky *sky_shader;

		real lot_b[10];
		real lot_c[10];
		real lot_K_c[10];
		real lot_t[10];
		real lot_r[10];

		real P( real theta ) const
		{
			// i am not sure about this, maybe one of the two:
			// -> http://scienceworld.wolfram.com/physics/PhaseFunction.html
			// -> http://omlc.ogi.edu/pubs/prahl-pubs/prahl88/node10.html
			return 1.0 / ( 4.0 * picogen::misc::constants::pi );
		}

		real P_S( real theta ) const
		{
			return P(theta) / (1.0-P_f);
			return (theta<theta_f) ? ( P(theta) / (1.0-P_f) ) : 0.0;
		}
		real P_F( real theta ) const
		{
			return P(theta) / P_f;
			return (theta<theta_f) ? ( P(theta) / P_f ) : 0.0;
		}

		real lookup( real *lot, real theta ) const
		{
			const int u = ( (int)( theta * (180.0/picogen::misc::constants::pi) ) ) / 10;
			return lot[
			      u < 0 ? 0
			    : u > 9 ? 9
			    : u
			];
		}

		template <typename T> T min( T f, T g ){ return f<=g?f:g; }
		template <typename T> T max( T f, T g ){ return f>=g?f:g; }

	public:
		CloudShader()
		{
			//H     = 100;              // 100 meters
			theta_f = 5.0 * (picogen::misc::constants::pi/180.0);
			N_0     = 300 * 1000000.0;  // 300/cm^3
			r_e     = 7.0 / 1000000.0;   // 7 micro-meters
			P_f     = 0.51;             // 51% for typical configurations
			real sigma = ( 1.0 - P_f ) * picogen::misc::constants::pi * r_e * r_e;
			K = sigma*N_0;

			lot_b[0]=1.1796;  lot_c[0]=0.0138;  lot_K_c[0]=0.0265;  lot_t[0]=0.8389;  lot_r[0]=0.0547;
			lot_b[1]=1.1293;  lot_c[1]=0.0154;  lot_K_c[1]=0.0262;  lot_t[1]=0.8412;  lot_r[1]=0.0547;
			lot_b[2]=1.1382;  lot_c[2]=0.0131;  lot_K_c[2]=0.0272;  lot_t[2]=0.8334;  lot_r[2]=0.0552;
			lot_b[3]=1.0953;  lot_c[3]=0.0049;  lot_K_c[3]=0.0294;  lot_t[3]=0.8208;  lot_r[3]=0.0564;
			lot_b[4]=0.9808;  lot_c[4]=0.0012;  lot_K_c[4]=0.0326;  lot_t[4]=0.8010;  lot_r[4]=0.0603;
			lot_b[5]=0.9077;  lot_c[5]=0.0047;  lot_K_c[5]=0.0379;  lot_t[5]=0.7774;  lot_r[5]=0.0705;
			lot_b[6]=0.7987;  lot_c[6]=0.0207;  lot_K_c[6]=0.0471;  lot_t[6]=0.7506;  lot_r[6]=0.0984;
			lot_b[7]=0.6629;  lot_c[7]=0.0133;  lot_K_c[7]=0.0616;  lot_t[7]=0.7165;  lot_r[7]=0.1700;
			lot_b[8]=0.5043;  lot_c[8]=0.0280;  lot_K_c[8]=0.0700;  lot_t[8]=0.7149;  lot_r[8]=0.3354;
			lot_b[9]=0.3021;  lot_c[9]=0.0783;  lot_K_c[9]=0.0700;  lot_t[9]=0.1000;  lot_r[9]=0.9500;
		}

		void SetSky( picogen::graphics::objects::abstract::ISky *skyShader ) { sky_shader = skyShader; }

		Color
		//real
		Shade( real &alpha, const Vector3d &_V, const Vector3d &_N, const Vector3d &_L, const real H )
		{
			const Vector3d V  = _V.normal();
			const Vector3d L  = _L.normal();
			const Vector3d N  = _N.normal();

			const real   theta_v  = acos( V * N ); // cos ( angle(V,N) )
			const real   theta_l  = acos( L * N ); // cos ( angle(V,N) )
			const real   theta_vl = acos( V * L ); // cos ( angle(V,N) )

			const real   mu_v   = fabs( cos( theta_v ) );//V * N; // cos ( angle(V,N) )
			const real   mu_l   = fabs( cos( theta_l ) );//L * N;
			const real   mu_v_minus_mu_l = fabs( mu_v-mu_l );

			//std::cout << theta_v << " " << theta_l << " " << mu_v << " " << mu_l << std::endl;
			//std::cout << V[0] << " " << V[1] << " " << V[2] << std::endl;

			const real    H_v   = fabs(H / mu_v); //> the paper did mention ("thickness", so I take the absolute here)
			const real    H_l   = fabs(H / mu_l); //>  . . . . . . .
			//const real    H_v_minus_H_l = fabs(H_v-H_l); //>  . . . . . . .

			//printf( "H_v=%f, H_l=%f, mu_v=%f, mu_l=%f\n", (float)H_v, (float)H_l, (float)mu_v, (float)mu_l );

			const real ext_Hv     = exp( -K*H_v ); // extinction( H_v )
			const real ext_Hl     = exp( -K*H_l ); // extinction( H_l )
			const real ext_Hl_plus_Hv = exp( -K*( H_l + H_v ) );
			//std::cout << ext_Hv << ":" << ext_Hl << " -- " << std::endl;

			alpha = 1.0 - ext_Hv;
			real I;

			if( 0 && V*L>=0.0 ){
				//> single scattering
				const real   I1_r   = ( (K*P(theta_vl)*mu_l) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);
				//> double scattering
				const real   I2_r = ( (pow(K,2)*pow(P(theta_vl)*mu_l,2)) / (mu_v+mu_l) ) * (1-ext_Hl_plus_Hv);

				I = I1_r + I2_r;
			}else{


				//> 0-scattering
				const real  I0_td   = ext_Hv;// * ( (V-L).lengthSq()<=epsilon ? 1.0 : 0.0 ); // ext_Hv * kronecker(V-L)
				const real  I0_tF   = ext_Hv * pow( P_F( theta_vl ), K*H_v );
				const real   I0_t   = I0_td + I0_tF;
				/*printf( "I0_td=%f, I0_tF=%f*pow(%f,%f*%f)=%f, I0_t=%f\n",
					(float)I0_td,
					ext_Hv, P_F(theta_vl), K, H_v, (float)I0_tF,
					(float)I0_t
				);*/


				//> single scattering
				const real i1_t_A = K*P(theta_vl)*mu_l;
				const real i1_t_B = mu_v_minus_mu_l;
				const real i1_t_C = fabs(ext_Hv-ext_Hl);
				const real   I1_t   = max( 0.0, double(( i1_t_A / i1_t_B ) * i1_t_C) );
				/*printf(
					"I1_t = ( (%f) / (%f) ) * (%f) = %f, A/B=%f\n",
					(float)(i1_t_A), (float)(i1_t_B),  (float)i1_t_C, (float)I1_t, (float)(i1_t_A/i1_t_B)
				);//*/


				//> double scattering
				const real   I2_t = max( 0.0, ( (pow(K,2) * pow(P(theta_vl),2) * mu_l) / (mu_v-mu_l) ) * (ext_Hv-ext_Hl) );
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
			real f = (I)*1;
			f = f < 0.0 ? 0.0 : f;
			return Color( f, f, f );//xrt::base_types::color( I0_t, I0_t, I0_t );
		}
};

struct CClouds : public picogen::graphics::objects::abstract::IIntersectable
{
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

		CClouds( real planet_radius, real height ) :
			x(0.0,0.0,0.0),
			r(planet_radius+height),
			//L_e(0.2),
			color(1.0,1.0,1.0),
			lambertian(0.0),
			perlin_sky(
				0.5,   // cover -> 0.0==no clouds, 0.5==sky partially covered, 1.0==sky covered almost completely
				0.8,    // persistence -> the smaller the smoother, around 0..1, but feel free to try out other values
				0.1,   // smoothness
				8*0.0008, 8, // scale and octave
				0 // use interpolation (slooooow)
			),
			sky_shader(NULL),
			cloud_shader(NULL)
		{
			perlin_sky.setPerlinXOfs( 20 );
		}

		void SetSkyShader( picogen::graphics::objects::Preetham *skyShader )
		{
			sky_shader = skyShader;
		}

		void SetCloudShader( CloudShader *cloudShader )
		{
			cloud_shader = cloudShader;
		}

		virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const
		{
			using namespace picogen::misc::constants;

			side currSide = noside;
			int i;
			Vector3d v;
			real sqrV = 0;
			real b    = 0;

			for( i=0; i<3; i++ ){
				v[i]  = ray.x()[i] - this->x[i];
				b    -= v[i] * ray.w()[i];
				sqrV += v[i] * v[i];
			}

			real det = (b * b) - sqrV + r*r;
			real i1, i2;

			real currT = real_max;
			if (det > 0){
				det = sqrt( det );
				i1 = b - det;
				i2 = b + det;
				if ( i2>0 ){
					if( i1<0 ){
						currT = i2;
						currSide = inside;
					}else{
						currT = i1;
						currSide = outside;
					}
				}
			}
			if( currT>epsilon && currSide != noside ){
				intersection.t = currT;
				const Vector3d i = ray(intersection.t);
				const float perlin = (double)perlin_sky(i[0],i[2]);
				intersection.normal = ( (currSide==inside)?(x-i):(i-x) ).normal();
				intersection.brdf = &lambertian;

				if( sky_shader != NULL && cloud_shader != NULL ){
					const Vector3d L = sky_shader->GetSunDirection();
					//intersection.L_e = 0.0;//intersection.L_e < 0.0 ? 0.0 : intersection.L_e;
					real alpha;
					intersection.L_e = 0.0;
					intersection.color =
						sky_shader->GetSunColor()
						* cloud_shader->Shade(
							alpha,
							-ray.w().normal(),
							/*vector3d(0,-1,0),*/ intersection.normal,
							-L,
							perlin*150.0
						)
					;
					//printf( "%f ", (float)perlin );
					if( ( (double)rand() / (double)RAND_MAX ) > alpha )
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






void insert_quad(
	picogen::graphics::objects::abstract::ITriScene *scene,
	picogen::misc::geometrics::Vector3d base,
	picogen::misc::prim::real radius,
	picogen::misc::geometrics::Vector3d &A,
	picogen::misc::geometrics::Vector3d &B,
	picogen::misc::geometrics::Vector3d &C,
	picogen::misc::geometrics::Vector3d &D
 )
{
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
 )
{
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

	for( side=0; side<6; side++ ){ // for each of the six planes
		if( ((mask>>side)&1) == 0 )
			continue;
		int U,V;
		for( V=0; V<detail; V++ ){
			const real v  = (real)V     / (real)detail;
			const real v_ = (real)(V+1) / (real)detail;
			for( U=0; U<detail; U++ ){
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
				tmp = 1.0+function->f( A[0], A[1], A[2] );  A[0] *= tmp; A[1] *= tmp; A[2] *= tmp;
				tmp = 1.0+function->f( B[0], B[1], B[2] );  B[0] *= tmp; B[1] *= tmp; B[2] *= tmp;
				tmp = 1.0+function->f( C[0], C[1], C[2] );  C[0] *= tmp; C[1] *= tmp; C[2] *= tmp;
				tmp = 1.0+function->f( D[0], D[1], D[2] );  D[0] *= tmp; D[1] *= tmp; D[2] *= tmp;

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
 )
{
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

	for( side=0; side<6; side++ ){ // for each of the six planes
		if( ((mask>>side)&1) == 0 )
			continue;
		int U,V;
		for( V=0; V<detail; V++ ){
			const real v  = (real)V     / (real)detail;
			const real v_ = (real)(V+1) / (real)detail;
			for( U=0; U<detail; U++ ){
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
				tmp = 1.0+function->f( A[0], A[1], A[2] );  A[0] *= tmp; A[1] *= tmp; A[2] *= tmp;
				tmp = 1.0+function->f( B[0], B[1], B[2] );  B[0] *= tmp; B[1] *= tmp; B[2] *= tmp;
				tmp = 1.0+function->f( C[0], C[1], C[2] );  C[0] *= tmp; C[1] *= tmp; C[2] *= tmp;
				tmp = 1.0+function->f( D[0], D[1], D[2] );  D[0] *= tmp; D[1] *= tmp; D[2] *= tmp;

				insert_quad( scene, center, radius, A, B, C, D );
			}
		}
	}
}


class TerrainShader : public picogen::graphics::material::abstract::IShader{
        picogen::misc::functions::abstract::vector_to_scalar *uvFunc;
    public:
        TerrainShader( picogen::misc::functions::abstract::vector_to_scalar *uvFunc=NULL ) : uvFunc(uvFunc) {}
        virtual ~TerrainShader() {}
        void setFunction( picogen::misc::functions::abstract::vector_to_scalar *uvFunc ){
            this->uvFunc = uvFunc;
        }
		virtual void Shade(
			picogen::graphics::image::color::Color &color,
			const picogen::misc::geometrics::Vector3d &normal,
			const picogen::misc::geometrics::Vector3d &position
		) const
		{
		    using namespace picogen::graphics::image::color;
		    if( NULL == uvFunc ){
		        color = ((int)( fabs(position[0]*0.1) )%2) == ((int)( fabs(position[2]*0.1) )%2)
                      ? color = Color(1,1,1)
                      : color = Color(0,0,0)
                ;
                return;
		    }
		    float f = uvFunc->f( position[0], position[1], position[2] );
		    f = f < 0 ? 0 : f > 1 ? 1 : f;
		    const Color A(0.25,0.25,0.25);
			const Color B(2.5,2.5,2.5);

		    color = A*f + B*(1-f);
		}
};

class HackGroundShader : public picogen::graphics::material::abstract::IShader{
	public:
		virtual ~HackGroundShader() {};
		virtual void Shade(
			picogen::graphics::image::color::Color &color,
			const picogen::misc::geometrics::Vector3d &normal,
			const picogen::misc::geometrics::Vector3d &position
		) const
		{
			using picogen::graphics::image::color::Color;
			using picogen::misc::prim::real;
			const real albedo = 1.0;
			const Color A(1.0,0.0,0.0);
			const Color B(1.0,1.0,1.0);

			//const Color A(1.0,1.0,1.0);
			//const Color B(0.8,0.5,0.2);

			const real n = normal[1];
			const real f = pow(n,4);
			/*const real ft = n * 3.1415927;
			const real f = ( 1.0 - cos(ft) ) * 0.5;*/
			/*const real n2 = n*n;
			const real n3 = n2*n;
			const real n4 = n3*n;
			const real n5 = n4*n;
			const real f = 6*n5-15*n4+10*n3; // 7 muls*/

			color = (A*(1.0-f) + B*(f))*albedo;
		}
};

class WaterShader : public picogen::graphics::material::abstract::IShader{
	public:
		virtual ~WaterShader() {};
		virtual void Shade(
			picogen::graphics::image::color::Color &color,
			const picogen::misc::geometrics::Vector3d &normal,
			const picogen::misc::geometrics::Vector3d &position
		) const
		{
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
static std::string getPicogenSaveDirectory(){
    //>> oh this is a real kludge to find out the home/~-dir
    //   and i am not sure if this is the way it has to be done
    std::string dirname;
    FILE *f = popen( "echo ~", "r" );
    if( NULL != f ){
        char buff[1024];
        while( NULL != fgets( buff, sizeof( buff ), f ) ){
            const unsigned int len = strlen(buff);
            if( len > 0 ){
                if( buff[len-1] == '\n' )
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

int init_sys(){
    static const char *line = "---------------------------------\n";
    printf( "%s%s\n%s", line, COMPILATION_INFO, line );
    printf( "picogen's directory for file saving: '%s'\n", getPicogenSaveDirectory().c_str() );
    printf( "%s", line );
    return 0;
}
int main( int argc, char *argv[] )
{
    //bool front = true;//fork() != 0;
	XRT_CALL_STACK_PUSH( "int main( int argc. char *argv[] )" );
	try{
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
		const int WIDTH = 640,//-(1280/4),
		          HEIGHT = 480;//-(512/4);
		fprintf( stderr, "Requested resolution is %ix%i; AA is %s\n", WIDTH, HEIGHT, useAA?"on":"off" );
		graphics::samplers::screen::XYIterator<
			misc::templates::surface<graphics::image::color::AverageColor>,
			graphics::cameras::FromPointToRect,
			graphics::samplers::ray::Simple
		> renderer;
		renderer.camera().DefineCamera( (real)WIDTH/(real)HEIGHT, 1, 0.6 );


		renderer.surface().reset( WIDTH*2, HEIGHT*2 );

		// TODO: this is a hack for defining the name of the auto-save-file
		std::string checkpointFile;
		while(1){
            std::cout << "Do you want to load a previous buffer (yes/no)? ";// Type in filename, then: ";
            std::cin >> checkpointFile;
            if( checkpointFile == "yes" ){
                std::cout << "Type in a filename: ";
                std::cin >> checkpointFile;
                FILE *f = fopen( checkpointFile.c_str(), "r" );
                if( f != NULL ){
                    fclose( f );
                    renderer.surface().loadBinaryFromFile( checkpointFile );
                    break;
                }else{
                    f = fopen( checkpointFile.c_str(), "w" );
                    if( f != NULL ){
                        fclose( f );
                        std::cout << "File '" << checkpointFile << "' could not be opened, but created it" << std::endl;
                        break;
                    }else{
                        std::cout << "File '" << checkpointFile << "' could neither be opened, nor be created, type in another filename, please." << std::endl;
                    }
                };
            }else if( checkpointFile == "no" ){
                checkpointFile = getPicogenSaveDirectory() + "auto.frgb";
                std::cout << "Using standard file '"<< checkpointFile << "'" << std::endl;
                break;
            }
		}


		if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
			fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
			return (1);
		}
		atexit(SDL_Quit);
		SDL_Surface *screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
		if( !screen ){
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

        picogen::misc::functions::vector_to_scalar::PerlinNoise heightShaderFunc;
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
        }

        graphics::objects::SimpleHeightField heightField;
        {
            using namespace picogen::misc::functions;
            uv_to_scalar::PerlinNoise function4;
                function4.setPersistence(0.96);
                function4.setBaseFrequency( 1.0 );
                function4.setOctaves(12);
                function4.setDomainScale(3.0);
                function4.setRangeScale(0.0125);
                function4.setMode( function4.ridged );
                function4.setMod( function4.mod_heterofractal );
                function4.setEvalScale( 1.8 );
                function4.seed( 42*4 );
            uv_to_scalar::Power moon_multifractal( &function4, 1.0 );
            //insert_sphere( &trilist, 200, &moon_multifractal, Vector3d(0,0,0), planet_radius, 1 );
            //trilist.Invalidate();
            //list.Insert( &trilist );
            // ++ TEH GROUND ++++++++++++++++++++++++++++++++++++++++++++++
            heightBRDF.mirror = 0.0;
            heightField.SetBRDF( &heightBRDF );
            heightField.SetShader( &heightShader );
            heightField.SetBox( base+Vector3d(-1000,-200,-1000), base+Vector3d(1000,400.0,1000) );
            heightField.Init( 128, &moon_multifractal );
            list.Insert( &heightField );

            // -- TEH GROUND ----------------------------------------------
        }

        graphics::objects::Snow snow;
        //snow.SetTimeExponent( 2.0 );
        //snow.SetVelocity( Vector3d(0,0.5,0) );
        list.Insert( &snow );
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
			Transformation().setToRotationX( -0.1 ) *
			Transformation().setToRotationY( -3.14159*0.25 ) *
			Transformation().setToTranslation( base+Vector3d(900,-100,-900) )
		;
		renderer.path_integrator().SetIntersectable( &list );

		//> setup sky
		graphics::objects::Preetham preetham;


		preetham.SetTurbidity( 1.8 );
		preetham.SetSunSolidAngleFactor(1.0);
		const real L = 0.2;
		/*preetham.SetColorFilter( Color(1.0,0.8,0.7)*1.0*L );  // > red filter
		preetham.SetSunColor( Color(1.0,0.3,0.2)*3200.0*3.4*L );//*/
		preetham.SetColorFilter( Color(0.9,0.9,1.0)*1.0*L );
		preetham.SetSunColor( Color(1.0,0.8,0.4)*43500.0*1.3*L*0.1 );//*/
		preetham.SetSunDirection( Vector3d(0.5,0.15,0.5).normal() );
		preetham.EnableFogHack( 1, 0.00082*3, 50000 );
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
		while(!done ){
			SDL_Event event;
			while ( SDL_PollEvent(&event) ){
				if( event.type == SDL_QUIT ){
					done = true;
				}
				if( event.type == SDL_KEYDOWN ){
					if( queryingForReallyQuit ){
						if ( event.key.keysym.sym == SDLK_y ){
							done = true;
							queryingForReallyQuit = false;
						}else if( event.key.keysym.sym == SDLK_n ){
							done = false;
							queryingForReallyQuit = false;
						}
					}else if ( event.key.keysym.sym == SDLK_ESCAPE ){
						queryingForReallyQuit = 1;
					}
					if ( event.key.keysym.sym == SDLK_d ){
						char s[256];
						sprintf( s, "screendump.%.4d.bmp", numManDumbs++ );
						SDL_SaveBMP( screen, s );
					}
					if( event.key.keysym.sym == SDLK_c ){ pixelsPerContinue-=WIDTH; renderer.SetNumPixelsPerContinue( pixelsPerContinue ); }
					if( event.key.keysym.sym == SDLK_v ){ pixelsPerContinue+=WIDTH; renderer.SetNumPixelsPerContinue( pixelsPerContinue ); }

					if ( event.key.keysym.sym == SDLK_5 ){ scale = 1.0;	}
					if ( event.key.keysym.sym == SDLK_6 ){ scale += 0.01; }
					if ( event.key.keysym.sym == SDLK_7 ){ scale += 0.1; }
					if ( event.key.keysym.sym == SDLK_8 ){ scale += 1.0; }
					if ( event.key.keysym.sym == SDLK_9 ){ scale += 10.0; }
					if ( event.key.keysym.sym == SDLK_4 ){ scale -= 0.01; }
					if ( event.key.keysym.sym == SDLK_3 ){ scale -= 0.1; }
					if ( event.key.keysym.sym == SDLK_2 ){ scale -= 1.0; }
					if ( event.key.keysym.sym == SDLK_1 ){ scale -= 10.0; }

					if ( event.key.keysym.sym == SDLK_t ){ exp_tone = 1.0;	}
					if ( event.key.keysym.sym == SDLK_z ){ exp_tone += 0.01; }
					if ( event.key.keysym.sym == SDLK_u ){ exp_tone += 0.1; }
					if ( event.key.keysym.sym == SDLK_i ){ exp_tone += 1.0; }
					if ( event.key.keysym.sym == SDLK_o ){ exp_tone += 10.0; }
					if ( event.key.keysym.sym == SDLK_r ){ exp_tone -= 0.01; }
					if ( event.key.keysym.sym == SDLK_e ){ exp_tone -= 0.1; }
					if ( event.key.keysym.sym == SDLK_w ){ exp_tone -= 1.0; }
					if ( event.key.keysym.sym == SDLK_q ){ exp_tone -= 10.0; }

					if ( event.key.keysym.sym == SDLK_g ){ saturation -= 0.1;	}
					if ( event.key.keysym.sym == SDLK_h ){ saturation -= 0.01;	}
					if ( event.key.keysym.sym == SDLK_j ){ saturation = 1.0;	}
					if ( event.key.keysym.sym == SDLK_k ){ saturation += 0.01;	}
					if ( event.key.keysym.sym == SDLK_l ){ saturation += 0.1;	}

				}
			}

			bool cont = renderer.Continue();
			if( cont ){
				runCount++;
				renderer.OneMoreRun();
				//if( 0 == (runCount%3) )
					renderer.surface().saveBinaryToFile( checkpointFile );
			}else if( queryingForReallyQuit ){
				SDL_WM_SetCaption( "Really Quit? (Y/N) ", "Really Quit? (Y/N) " );
			}else{
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

	}catch( picogen::misc::exceptions::base b ){
		std::cout << "error: ´" << b.message << "´" << std::endl;
		std::cout << "in line " << b.line << ", file ´" << b.filename << "´" << std::endl;
		dump_call_stack();
	}catch( ... ){
		std::cout << "error: undefined exception" << std::endl;
		dump_call_stack();
	}
	XRT_CALL_STACK_POP();
	return 0;
}
