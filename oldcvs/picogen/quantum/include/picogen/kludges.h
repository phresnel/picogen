/***************************************************************************
 *            kludges.h
 *
 *  Fri Sep  7 01:31:26 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License
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
 
#ifndef _KLUDGES_H
#define _KLUDGES_H

#define float double
namespace cloud_system{
class PerlinNoise{
	private:
		float Persistence;
		int   Iterations;
		int   ofs_x, ofs_y;
		bool  interpolate_noise;
	
	
		inline static float cosine_interpolate( float p, float q, float x )
		{
			float ft = x * 3.1415927;
			float f = ( 1.0 - cos(ft) ) * 0.5;
			return  p*(1-f) + q*f;
		}


		inline float noise( int x, int y )  const
		{
			int n = (ofs_x+x) + (ofs_y+y) * 57;
			n = (n<<13) ^ n;
			return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0); 
		}
		inline float smoothed_noise( float fx, float fy ) const
		{
			int x = (int)fx;
			int y = (int)fy;
			if( interpolate_noise ){
				const float corners = ( noise(x-1, y-1)+noise(x+1, y-1)+noise(x-1, y+1)+noise(x+1, y+1) ) / 16.0;
    			const float sides   = ( noise(x-1, y)  +noise(x+1, y)  +noise(x, y-1)  +noise(x, y+1) ) /  8.0;
    			const float center  =  noise(x, y) / 4.0;
				return corners + sides + center;
			}			
			return noise(x, y);
		}

		inline float interpolated_noise( float x, float y ) const
		{
			const float trunc_x = (float)(int)x;
			const float trunc_y = (float)(int)y;
			const float frac_x  = x-trunc_x;
			const float frac_y  = y-trunc_y;
			const float a = smoothed_noise(   trunc_x,   trunc_y ); 
			const float b = smoothed_noise( 1+trunc_x,   trunc_y );
			const float c = smoothed_noise(   trunc_x, 1+trunc_y );
			const float d = smoothed_noise( 1+trunc_x, 1+trunc_y );			
			const float A = cosine_interpolate( a, b, frac_x );
			const float B = cosine_interpolate( c, d, frac_x );
			return cosine_interpolate( A, B, frac_y );
		}
		
		float perlin( float x, float y ) const
		{			
			float r = 0;
			int n;
			float fn;
			for( fn=0, n=0; n<Iterations; fn+=1, n+=1 ){
				const float freq = powf(2.0,fn);
				const float amp  = powf(Persistence,fn);
				r = r + amp * interpolated_noise(x*freq, y*freq);
			}
			return r;
		}
	
	public:
		PerlinNoise( float persistence, int octaves, int _ofs_x, int _ofs_y, bool interpolation=true ) :
			Persistence( persistence ),
			Iterations( octaves-1 ),
			ofs_x(_ofs_x), ofs_y(_ofs_y),
			/* TODO: user definition */ interpolate_noise(interpolation)
		{}
			
		PerlinNoise() : Persistence(1), Iterations(4), ofs_x(0), ofs_y(0) {}
		
		void  setPersistence( float pers )   { Persistence = pers; }
		float getPersistence()               { return Persistence; }
		void  setOctaves( unsigned int oct ) { Iterations = oct-1; }
		int   getOctaves()                   { return Iterations+1;}		
		void  setPerlinXOfs( int _ofs_x )    { ofs_x = _ofs_x; }
		int   getPerlinXOfs()                { return ofs_x; }
		void  setPerlinYOfs( int _ofs_y )    { ofs_y = _ofs_y; }		
		int   getPerlinYOfs()                { return ofs_y; }
			
		float operator () ( float u, float v ) const{
			return perlin( 10000.0+u, 10000.0+v );
		}
};

// Implements a  cloud-structure in the manner of  Hugo Elias
// on his homepage (http://freespace.virgin.net/hugo.elias/).
// Note that we do not do any precalculation, rather we do it
// fully procedurrally, so we are not limited in memory-space
// and/or screen resolution.
class PerlinBased{
	private:
		PerlinNoise m_noise;
		float m_scale;
		float m_ncover;		
		float m_smoothness;
	public:
		
		PerlinBased(
			float cover,
			float persistence, 
			float smoothness,			
			float scale,
			unsigned int octaves,
			bool interpolation = true
		) :	
			m_noise( persistence, octaves, 0, 0, interpolation ),
			m_scale( scale ),
			m_ncover( 1-cover ),
			m_smoothness( smoothness )
		{
		}
		
		void  setCover( float f )        { m_ncover = 1.0 - f;  }
		float getCover()                 { return 1.0 - m_ncover; }
		void  setPersistence( float f )  { m_noise.setPersistence( f ); }
		float getPersistence()           { return m_noise.getPersistence(); }
		void  setSmoothness( float f )   { m_smoothness = f; }
		float getSmoothness()            { return m_smoothness; }
		void  setScale( float f )        { m_scale = f; }
		float getScale()                 { return m_scale; }
		void  setOctaves( unsigned int o ) { m_noise.setOctaves( o ); }
		unsigned int getOctaves()        { return m_noise.getOctaves(); }
		void  setPerlinXOfs( int o )    { m_noise.setPerlinXOfs(o); }
		int   getPerlinXOfs()           { return m_noise.getPerlinXOfs(); }
		void  setPerlinYOfs( int o )    { m_noise.setPerlinYOfs(o); }
		int   getPerlinYOfs()           { return m_noise.getPerlinYOfs(); }
		

		float operator () ( float x, float y ) const
		{
			const float f = 0.5+0.5*m_noise( m_scale*x, m_scale*y ); //> get noise and map [-1..1] to [0..1]
			const float c = f<m_ncover ? 0 : f-m_ncover;
			const float t = 1 - powf( m_smoothness, c );
			return t;
		}
};

#undef float

}; // namespace cloud_system

#endif /* _KLUDGES_H */
