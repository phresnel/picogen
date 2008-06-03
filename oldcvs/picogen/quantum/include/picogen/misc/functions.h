/***************************************************************************
 *            functions.h
 *
 *  Fri Oct 12 19:29:55 2007
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

// NOTE this file tagged obsolete, take namespace functional in functional.h instead

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

#define float real

namespace picogen {
    namespace misc {

        /*
        namespace xrt{
        namespace functions{*/

        namespace functions {
            namespace abstract {
                class scalar_to_scalar {
                    public:
                        virtual prim::real f (prim::real x) const = 0;
                        virtual ~scalar_to_scalar() {}
                };
                class uv_to_scalar {
                    public:
                        virtual prim::real f (prim::real u, prim::real v) const = 0;
                        virtual ~uv_to_scalar() {}
                };
                class vector_to_scalar {
                    public:
                        virtual prim::real f (prim::real x, prim::real y, prim::real z) const = 0;
                        virtual ~vector_to_scalar() {}
                };
            };

            namespace vector_to_scalar {

                class Constant : public abstract::vector_to_scalar {
                        typedef prim::real real;
                    public:
                        real r;
                        Constant() : r (0.0) {}
                        Constant (real R) : r (R) {}
                        virtual real f (real x, real y, real z) const {
                            return r;
                        }
                        virtual ~Constant() {}
                };

                class Sine : public abstract::vector_to_scalar, public abstract::scalar_to_scalar, public abstract::uv_to_scalar {
                        typedef prim::real real;
                    public:
                        typedef enum mode {
                            normal = 0,
                            ridged,
                            lava
                        };
                    private:
                        mode m_mode;
                    public:

                        virtual ~Sine() {}

                        real domain_scale;
                        real range_scale;

                        mode getMode() const      {
                            return m_mode;
                        }
                        void setMode (mode Mode) {
                            m_mode = Mode;
                        }

                        Sine() : m_mode (normal), domain_scale (0), range_scale (0) {}
                        Sine (real DomainScale, real RangeScale) : m_mode (normal), domain_scale (DomainScale), range_scale (RangeScale) {}
                        virtual real f (real x, real y, real z) const {
                            float A = sin (domain_scale*x) * sin (domain_scale*y) * sin (domain_scale*z);
                            switch (m_mode) {
                                case ridged:
                                    A = 1.0 - fabs (A);
                                    break;
                                case lava:
                                    A = fabs (A);
                                    break;
                                case normal:
                                default:
                                    break;
                            };
                            return range_scale*A;
                        }
                        virtual real f (real x, real y) const {
                            float A = sin (domain_scale*x) * sin (domain_scale*y);
                            switch (m_mode) {
                                case ridged:
                                    A = 1.0 - fabs (A);
                                    break;
                                case lava:
                                    A = fabs (A);
                                    break;
                                case normal:
                                default:
                                    break;
                            };
                            return A * range_scale;
                        }
                        virtual real f (real x) const {
                            float A = sin (domain_scale*x);
                            switch (m_mode) {
                                case ridged:
                                    A = 1.0 - fabs (A);
                                    break;
                                case lava:
                                    A = fabs (A);
                                    break;
                                case normal:
                                default:
                                    break;
                            };
                            return A * range_scale;
                        }
                };

                class Elias_one : public abstract::vector_to_scalar {
                    public:
                        typedef enum mode {
                            normal = 0,
                            ridged
                        };
                    private:
                        typedef prim::real real;
                        int m_num_planes;
                        typedef struct t_plane {
                            real p[3];
                            void rand() {
                                real len;
                                do {
                                    p[0] = -1.0 + (real) (2.0 * (double) ::rand() / (double) RAND_MAX);
                                    p[1] = -1.0 + (real) (2.0 * (double) ::rand() / (double) RAND_MAX);
                                    p[2] = -1.0 + (real) (2.0 * (double) ::rand() / (double) RAND_MAX);
                                } while ( (len=p[0]*p[0]+p[1]*p[1]+p[2]*p[2]) > 1.0);
                                len = 1.0 / sqrt (len);
                                p[0] *= len;
                                p[1] *= len;
                                p[2] *= len;
                            };
                            float dot (float x, float y, float z) {
                                return x*p[0] + y*p[1] + z*p[2];
                            }
                        } _t_plane;

                        t_plane *m_planes;
                        mode m_mode;
                    public:
                        real T;
                        Elias_one() : m_num_planes (0), m_planes (NULL), T (0.05) {};
                        virtual ~Elias_one() {
                            if (NULL != m_planes)
                                delete [] m_planes;
                            m_mode = normal;
                            m_planes = NULL;
                            m_num_planes = 0;
                        }
                        bool seed (int n, int rand) {
                            if (m_planes != NULL)
                                delete [] m_planes;
                            m_num_planes = 0;
                            m_planes = new t_plane[n];
                            if (m_planes == NULL)
                                return false;
                            m_num_planes = n;
                            int i;
                            srand (rand);
                            for (i=0; i<m_num_planes; i++) {
                                m_planes[i].rand();
                            }
                            return true;
                        }

                        mode getMode() const      {
                            return m_mode;
                        }
                        void setMode (mode Mode) {
                            m_mode = Mode;
                        }

                        virtual real f (real x, real y, real z) const {
                            real ret = 0;
                            int i;
                            for (i=0; i<m_num_planes; i++) {
                                if (0>m_planes[i].dot (x,y,z)) {
                                    ret += T;
                                } else {
                                    ret -= T;
                                }
                            }
                            return (m_mode == normal) ? (ret)
                                   : (m_mode == ridged) ? (1.0-fabs (ret))
                                   : ret
                                   ;
                        }
                };

// this is  a  dirty perlin noise implementation without
// directional gradients (similar to Hugo Elias';  gives
// pretty blocky artifacts)
                class PerlinNoise_Elias : public abstract::vector_to_scalar, public abstract::scalar_to_scalar, public abstract::uv_to_scalar {
                        typedef prim::real real;
                    public:
                        typedef enum mode {
                            normal = 0,
                            ridged,
                            lava
                        };
                    private:



                        inline static float cosine_interpolate (float p, float q, float x) {
                            float ft = x * 3.1415927;
                            float f = (1.0 - cos (ft)) * 0.5;
                            return  p* (1-f) + q*f;
                        }

                        inline static float linear_interpolate (float p, float q, float x) {
                            return  p* (1-x) + q*x;
                        }

                        inline static float ease_interpolate (float p, float q, float x) {
                            float f = 3*x*x - 2*x*x*x;
                            //float f = 6*x*x*x*x*x-15*x*x*x*x+10*x*x*x;
                            return p* (1-f) + q*f;
                        }

                        inline float noise (int i, int j, int k)  const {
                            return lot_vals[ (i+lot_ofs[ (j+lot_ofs[k&0xFE]) &0xFE]) &0xFE ];
                        }

                        inline float smoothed_noise (int i, int j, int k) const {
                            const float corners =
                                (noise (i-1,j-1,k-1) +
                                 noise (i+1,j-1,k-1) +
                                 noise (i-1,j+1,k-1) +
                                 noise (i+1,j+1,k-1) +
                                 noise (i-1,j-1,k+1) +
                                 noise (i+1,j-1,k+1) +
                                 noise (i-1,j+1,k+1) +
                                 noise (i+1,j+1,k+1)) / 32.0; // 25%

                            const float sides =
                                (noise (i-1,j,k) + noise (i+1,j,k) +
                                 noise (i,j-1,k) + noise (i,j+1,k) +
                                 noise (i,j,k-1) + noise (i,j,k+1)) / 12.0; // 50%

                            const float center = noise (i,j,k) / 4.0; // 25%

                            return sides + corners + center;
                        }

                        inline float interpolated_noise (float x, float y, float z) const {
                            const float trunc_x = (float) (int) x;
                            const float trunc_y = (float) (int) y;
                            const float trunc_z = (float) (int) z;
                            const float frac_x  = x-trunc_x;
                            const float frac_y  = y-trunc_y;
                            const float frac_z  = z-trunc_z;

                            const float a = smoothed_noise ( (int) trunc_x, (int) trunc_y, (int) trunc_z);
                            const float b = smoothed_noise (1+ (int) trunc_x, (int) trunc_y, (int) trunc_z);
                            const float c = smoothed_noise ( (int) trunc_x, 1+ (int) trunc_y, (int) trunc_z);
                            const float d = smoothed_noise (1+ (int) trunc_x, 1+ (int) trunc_y, (int) trunc_z);

                            const float p = smoothed_noise ( (int) trunc_x, (int) trunc_y, 1+ (int) trunc_z);
                            const float q = smoothed_noise (1+ (int) trunc_x, (int) trunc_y, 1+ (int) trunc_z);
                            const float r = smoothed_noise ( (int) trunc_x, 1+ (int) trunc_y, 1+ (int) trunc_z);
                            const float s = smoothed_noise (1+ (int) trunc_x, 1+ (int) trunc_y, 1+ (int) trunc_z);

                            const float A = ease_interpolate (a, b, frac_x);
                            const float B = ease_interpolate (c, d, frac_x);
                            const float X = ease_interpolate (A, B, frac_y);

                            const float P = ease_interpolate (p, q, frac_x);
                            const float Q = ease_interpolate (r, s, frac_x);
                            const float Y = ease_interpolate (P, Q, frac_y);

                            return ease_interpolate (X, Y, frac_z);
                        }

                        float perlin (float x, float y, float z) const {
                            float r = 0;
                            int n;
                            float fn;
                            float amp  = BaseAmplitude;
                            float freq = BaseFrequency;

                            switch (EvalMode) {
                                case ridged:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp* (0.5-fabs (interpolated_noise (x*freq, y*freq, z*freq)));
                                    }
                                    break;
                                case lava:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp* (fabs (interpolated_noise (x*freq, y*freq, z*freq)));
                                    }
                                    break;
                                case normal:
                                default:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp * interpolated_noise (x*freq, y*freq, z*freq);
                                    }
                                    break;

                            }
                            return r;
                        }

                        void fill_lots (int seed) {
                            srand (seed);
                            int i;
                            for (i=0; i<0xFF; i++) {
                                lot_ofs[i]  = rand() & 0xFE;
                                lot_vals[i] = -1.0 + 2.0 * (float) rand() / (float) RAND_MAX;
                            }
                        }

                    private:
                        float DomainScale, RangeScale;
                        float Persistence;
                        int   Iterations;
                        float BaseFrequency, BaseAmplitude;
                        mode  EvalMode;
                        float lot_vals[0xFF];
                        unsigned char lot_ofs[0xFF];
                        float EvalScale;

                    public:

                        PerlinNoise_Elias() :
                                DomainScale (1.0), RangeScale (0.1),
                                Persistence (1),
                                Iterations (4),
                                BaseFrequency (4.0), BaseAmplitude (0.5),
                                EvalMode (normal),
                                EvalScale (2.0) {
                            fill_lots (1);
                        }

                        void seed (int seed) {
                            fill_lots (seed);
                        }

                        void setMode (mode m) {
                            EvalMode = m;
                        }
                        void setBaseFrequency (float f) {
                            BaseFrequency = f;
                        }

                        /*void  enableRidge( bool t ) { EnableRidge = t; }
                        void  enableLava( bool t )  { EnableLava = t; }*/
                        void  setDomainScale (float f)   {
                            DomainScale = f;
                        }
                        float getDomainScale() const      {
                            return DomainScale;
                        }
                        void  setRangeScale (float f)    {
                            RangeScale = f;
                        }
                        float getRangeScale() const       {
                            return RangeScale;
                        }

                        void  setPersistence (float pers)   {
                            Persistence = pers;
                        }
                        float getPersistence() const         {
                            return Persistence;
                        }
                        void  setOctaves (unsigned int oct) {
                            Iterations = oct-1;
                        }
                        int   getOctaves() const             {
                            return Iterations+1;
                        }

                        void  setEvalScale (float f) {
                            EvalScale = f;
                        }
                        float getEvalScale() const    {
                            return EvalScale;
                        }

                        virtual real f (real x, real y, real z) const {
                            x = (x*0.5 + 0.5) *DomainScale;
                            y = (y*0.5 + 0.5) *DomainScale;
                            z = (z*0.5 + 0.5) *DomainScale;
                            float f = perlin (x, y, z);
                            return 2.0 * RangeScale * f;
                        }
                        virtual real f (real x, real y) const {
                            x = (x*0.5 + 0.5) *DomainScale;
                            y = (y*0.5 + 0.5) *DomainScale;
                            float f = perlin (x, y, 0.5);
                            return 2.0 * RangeScale * f;
                        }
                        virtual real f (real x) const {
                            x = (x*0.5 + 0.5) *DomainScale;
                            float f = perlin (x, 0.5, 0.5);
                            return 2.0 * RangeScale * f;
                        }
                };


// a perlin noise implementation using gradients
// (as Ken Perlin originally invented, afaik)
                class PerlinNoise
                            : public abstract::vector_to_scalar,
                            public abstract::scalar_to_scalar,
                            public abstract::uv_to_scalar {
                        typedef prim::real real;
                    public:
                        typedef enum mode {
                            normal = 0,
                            ridged,
                            ridged_2,
                            lava
                        };
                        typedef enum mod {
                            mod_normal = 0,
                            mod_heterofractal
                        };
                    private:

                        inline static float ease_interpolate (float p, float q, float x) {
                            //return p*(1-x) + q*x;
                            //float f = 3*x*x - 2*x*x*x;

                            const float x2 = x*x;
                            const float x3 = x2*x;
                            const float x4 = x3*x;
                            const float x5 = x4*x;
                            float f = 6*x5-15*x4+10*x3; // 7 muls

                            //float f = 6*x*x*x*x*x-15*x*x*x*x+10*x*x*x; // 12 muls
                            return p* (1-f) + q*f;
                        }

                        inline float dot (int i, int j, int k, const float *vec) const {
                            const float *vg = lot_vals[ (i+lot_ofs[ (j+lot_ofs[k&0xFE]) &0xFE]) &0xFE ];
                            const float ilen = 1.0;//1.0 / sqrt( vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] );
                            return vg[0]*vec[0]*ilen + vg[1]*vec[1]*ilen + vg[2]*vec[2]*ilen;
                        }

                        inline float interpolated_noise (float x, float y, float z) const {
                            // calculate grid min- and max-coordinates
                            const int trunc_a[] = { (int) x, (int) y, (int) z };
                            const int trunc_b[] = { 1+trunc_a[0], 1+trunc_a[1], 1+trunc_a[2] };
                            const float frac[]  = { x- (float) trunc_a[0], y- (float) trunc_a[1], z- (float) trunc_a[2] };
                            // calculate for each grid corner the diff to (x,y,z)
                            const float a[] = { x- (float) trunc_a[0], y- (float) trunc_a[1], z- (float) trunc_a[2] };
                            const float b[] = { x- (float) trunc_b[0], y- (float) trunc_a[1], z- (float) trunc_a[2] };
                            const float c[] = { x- (float) trunc_a[0], y- (float) trunc_b[1], z- (float) trunc_a[2] };
                            const float d[] = { x- (float) trunc_b[0], y- (float) trunc_b[1], z- (float) trunc_a[2] };
                            const float e[] = { x- (float) trunc_a[0], y- (float) trunc_a[1], z- (float) trunc_b[2] };
                            const float f[] = { x- (float) trunc_b[0], y- (float) trunc_a[1], z- (float) trunc_b[2] };
                            const float g[] = { x- (float) trunc_a[0], y- (float) trunc_b[1], z- (float) trunc_b[2] };
                            const float h[] = { x- (float) trunc_b[0], y- (float) trunc_b[1], z- (float) trunc_b[2] };
                            // calculate dot-products to the gradients at trunc_(a|b)
                            const float p = dot (trunc_a[0], trunc_a[1], trunc_a[2], a);
                            const float q = dot (trunc_b[0], trunc_a[1], trunc_a[2], b);
                            const float r = dot (trunc_a[0], trunc_b[1], trunc_a[2], c);
                            const float s = dot (trunc_b[0], trunc_b[1], trunc_a[2], d);
                            const float t = dot (trunc_a[0], trunc_a[1], trunc_b[2], e);
                            const float u = dot (trunc_b[0], trunc_a[1], trunc_b[2], f);
                            const float v = dot (trunc_a[0], trunc_b[1], trunc_b[2], g);
                            const float w = dot (trunc_b[0], trunc_b[1], trunc_b[2], h);


                            const float A = ease_interpolate (p, q, frac[0]);
                            const float B = ease_interpolate (r, s, frac[0]);
                            const float X = ease_interpolate (A, B, frac[1]);

                            const float P = ease_interpolate (t, u, frac[0]);
                            const float Q = ease_interpolate (v, w, frac[0]);
                            const float Y = ease_interpolate (P, Q, frac[1]);

                            return ease_interpolate (X, Y, frac[2]);
                        }

                        float perlin (float x, float y, float z, bool forceNoMode=false) const {
                            float r = 0;
                            int n;
                            float fn;
                            float amp  = BaseAmplitude;
                            float freq = BaseFrequency;
                            float Persistence = this->Persistence;

                            /*if( !forceNoMode && Modifier == mod_heterofractal ){
                             Persistence *= pow( perlin( x, y, z, true ), 1.5 );
                            }*/

                            switch (EvalMode) {
                                case ridged:
                                    if (Modifier == mod_heterofractal) {
                                        for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                            float f = Persistence* (1.0-fabs (interpolated_noise (x*freq, y*freq, z*freq)));
                                            amp *= pow (f,5);
                                            freq *= EvalScale;
                                            r += amp;
                                        }
                                    } else {
                                        for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                            amp *= Persistence;
                                            freq *= EvalScale;
                                            r += amp* (1.0-fabs (interpolated_noise (x*freq, y*freq, z*freq)));
                                        }
                                    }
                                    break;
                                case ridged_2:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp*pow (1.0-fabs (interpolated_noise (x*freq, y*freq, z*freq)),2.0);
                                    }
                                    break;
                                case lava:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp* (fabs (interpolated_noise (x*freq, y*freq, z*freq)));
                                    }
                                    break;
                                case normal:
                                default:
                                    for (fn=1, n=0; n<Iterations; fn+=1, n+=1) {
                                        amp *= Persistence;
                                        freq *= EvalScale;
                                        r += amp * interpolated_noise (x*freq, y*freq, z*freq);
                                    }
                                    break;

                            }
                            return r;
                        }

                        void fill_lots (int seed) {
                            srand (seed);
                            int i;
                            for (i=0; i<0xFF; i++) {
                                lot_ofs[i]  = rand() & 0xFE;

                                real len;
                                do {
                                    len  = pow (lot_vals[i][0] = -1.0 + 2.0 * (real) rand() / (real) RAND_MAX, 2.0);
                                    len += pow (lot_vals[i][1] = -1.0 + 2.0 * (real) rand() / (real) RAND_MAX, 2.0);
                                    len += pow (lot_vals[i][2] = -1.0 + 2.0 * (real) rand() / (real) RAND_MAX, 2.0);
                                } while (1 < len);
                                len = 1.0 / sqrt (len);
                                lot_vals[i][0] *= len;
                                lot_vals[i][1] *= len;
                                lot_vals[i][2] *= len;
                            }
                        }

                    private:
                        float DomainScale, RangeScale;
                        float Persistence;
                        int   Iterations;
                        float BaseFrequency, BaseAmplitude;
                        mode  EvalMode;
                        float lot_vals[0xFF][3];
                        unsigned char lot_ofs[0xFF];
                        float EvalScale;
                        mod   Modifier;

                    public:

                        PerlinNoise() :
                                DomainScale (1.0), RangeScale (0.1),
                                Persistence (1),
                                Iterations (4),
                                BaseFrequency (4.0), BaseAmplitude (0.5),
                                EvalMode (normal),
                                EvalScale (2.0),
                                Modifier (mod_normal) {
                            fill_lots (1);
                        }

                        void seed (int seed) {
                            fill_lots (seed);
                        }

                        void setMode (mode m) {
                            EvalMode = m;
                        }
                        void setMod (mod m) {
                            Modifier = m;
                        }
                        void setBaseFrequency (float f) {
                            BaseFrequency = f;
                        }

                        /*void  enableRidge( bool t ) { EnableRidge = t; }
                        void  enableLava( bool t )  { EnableLava = t; }*/
                        void  setDomainScale (float f)   {
                            DomainScale = f;
                        }
                        float getDomainScale() const      {
                            return DomainScale;
                        }
                        void  setRangeScale (float f)    {
                            RangeScale = f;
                        }
                        float getRangeScale() const       {
                            return RangeScale;
                        }

                        void  setPersistence (float pers)   {
                            Persistence = pers;
                        }
                        float getPersistence() const         {
                            return Persistence;
                        }
                        void  setOctaves (unsigned int oct) {
                            Iterations = oct-1;
                        }
                        int   getOctaves() const             {
                            return Iterations+1;
                        }

                        void  setEvalScale (float f) {
                            EvalScale = f;
                        }
                        float getEvalScale() const    {
                            return EvalScale;
                        }

                        virtual real f (real x, real y, real z) const {
                            x = (x*0.5 + 0.5) *DomainScale;
                            y = (y*0.5 + 0.5) *DomainScale;
                            z = (z*0.5 + 0.5) *DomainScale;
                            float f = perlin (x+10000.0, y+10000.0, z+10000.0);
                            return 2.0 * RangeScale * f;
                        }
                        virtual real f (real u, real v) const {
                            u = (u*0.5 + 0.5) *DomainScale;
                            v = (v*0.5 + 0.5) *DomainScale;
                            float f = perlin (u+10000.0, 0.5, v+10000.0);
                            return 2.0 * RangeScale * f;
                        }
                        virtual real f (real x) const {
                            x = (x*0.5 + 0.5) *DomainScale;
                            float f = perlin (x+10000.0, 0.5, 0.5);
                            return 2.0 * RangeScale * f;
                        }
                };

                class Sum : public abstract::vector_to_scalar {
                        abstract::vector_to_scalar *a, *b;
                    public:
                        Sum() : a (NULL), b (NULL) {}
                        Sum (abstract::vector_to_scalar *_a, abstract::vector_to_scalar *_b) : a (_a), b (_b) {}
                        void setA (abstract::vector_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (abstract::vector_to_scalar *a) {
                            this->b = b;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y, ::picogen::misc::prim::real z) const {
                            return (NULL!=a?a->f (x,y,z) :0.0) + (NULL!=b?b->f (x,y,z) :0.0);
                        }
                };

                class Product : public abstract::vector_to_scalar {
                        abstract::vector_to_scalar *a, *b;
                    public:
                        Product() : a (NULL), b (NULL) {}
                        Product (abstract::vector_to_scalar *_a, abstract::vector_to_scalar *_b) : a (_a), b (_b) {}
                        void setA (abstract::vector_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (abstract::vector_to_scalar *a) {
                            this->b = b;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y, ::picogen::misc::prim::real z) const {
                            return (NULL!=a?a->f (x,y,z) :0.0) * (NULL!=b?b->f (x,y,z) :0.0);
                        }
                };

                class Power : public abstract::vector_to_scalar {
                        abstract::vector_to_scalar *a;
                        ::picogen::misc::prim::real p;
                    public:
                        Power() : a (NULL), p (1.0) {}
                        Power (abstract::vector_to_scalar *_a, ::picogen::misc::prim::real _p) : a (_a), p (_p) {}
                        void setA (abstract::vector_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (::picogen::misc::prim::real p) {
                            this->p = p;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y, ::picogen::misc::prim::real z) const {
                            return pow ( (NULL!=a?a->f (x,y,z) :0.0), p);
                        }
                };

#if XRT_INCLUDE_EXPERIMENTAL
                class InputDistorter : public abstract::vector_to_scalar {
                        typedef prim::real real;
                        // EXPERIMENTAL: this class is experimental
                        abstract::scalar_to_scalar *Distortion;
                        abstract::vector_to_scalar *Function;

                    public:
                        InputDistorter (abstract::scalar_to_scalar *distortion, abstract::vector_to_scalar *function) :
                                Distortion (distortion), Function (function) {}
                        InputDistorter() : Distortion (NULL), Function (NULL) {}
                        void setDistortion (abstract::scalar_to_scalar *dist) {
                            Distortion = dist;
                        }
                        void setFunction (abstract::vector_to_scalar *func) {
                            Function = func;
                        }

                        virtual real f (real x, real y, real z) const {
                            if (Distortion == NULL || Function == NULL)
                                return 0;
                            return Function->f (Distortion->f (x), Distortion->f (y), Distortion->f (z));
                        }
                };
#endif // XRT_INCLUDE_EXPERIMENTAL
            };

            namespace uv_to_scalar {
                typedef vector_to_scalar::PerlinNoise_Elias PerlinNoise_Elias;
                typedef vector_to_scalar::PerlinNoise       PerlinNoise;
                typedef vector_to_scalar::Sine              Sine;

                class Constant : public abstract::uv_to_scalar {
                        typedef prim::real real;
                    public:
                        real r;
                        Constant() : r (0.0) {}
                        Constant (real R) : r (R) {}
                        virtual real f (real x, real y) const {
                            return r;
                        }
                        virtual ~Constant() {}
                };

                class Negate : public abstract::uv_to_scalar {
                        abstract::uv_to_scalar *a;
                    public:
                        Negate() : a (NULL) {}
                        Negate (abstract::uv_to_scalar *_a) : a (_a) {}
                        void setA (abstract::uv_to_scalar *a) {
                            this->a = a;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y) const {
                            return - (NULL!=a?a->f (x,y) :0.0);
                        }
                };

                class Sum : public abstract::uv_to_scalar {
                        abstract::uv_to_scalar *a, *b;
                    public:
                        Sum() : a (NULL), b (NULL) {}
                        Sum (abstract::uv_to_scalar *_a, abstract::uv_to_scalar *_b) : a (_a), b (_b) {}
                        void setA (abstract::uv_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (abstract::uv_to_scalar *a) {
                            this->b = b;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y) const {
                            return (NULL!=a?a->f (x,y) :0.0) + (NULL!=b?b->f (x,y) :0.0);
                        }
                };
                class Product : public abstract::uv_to_scalar {
                        abstract::uv_to_scalar *a, *b;
                    public:
                        Product() : a (NULL), b (NULL) {}
                        Product (abstract::uv_to_scalar *_a, abstract::uv_to_scalar *_b) : a (_a), b (_b) {}
                        void setA (abstract::uv_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (abstract::uv_to_scalar *a) {
                            this->b = b;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y) const {
                            return (NULL!=a?a->f (x,y) :0.0) * (NULL!=b?b->f (x,y) :0.0);
                        }
                };

                class Power : public abstract::uv_to_scalar {
                        abstract::uv_to_scalar *a;
                        ::picogen::misc::prim::real p;
                    public:
                        Power() : a (NULL), p (1.0) {}
                        Power (abstract::uv_to_scalar *_a, ::picogen::misc::prim::real _p) : a (_a), p (_p) {}
                        void setA (abstract::uv_to_scalar *a) {
                            this->a = a;
                        }
                        void setB (::picogen::misc::prim::real p) {
                            this->p = p;
                        }
                        virtual ::picogen::misc::prim::real f (::picogen::misc::prim::real x, ::picogen::misc::prim::real y) const {
                            return pow ( (NULL!=a?a->f (x,y) :0.0), p);
                        }
                };
            };
            namespace scalar_to_scalar {
                /// \todo remove aliases and implement dedicated 1d-to-1d-functions
                typedef vector_to_scalar::PerlinNoise_Elias PerlinNoise_Elias;
                typedef vector_to_scalar::PerlinNoise       PerlinNoise;
                typedef vector_to_scalar::Sine              Sine;
            };

        } // namespace functions


#undef float

    }//namespace misc {
}//namespace picogen {

#endif /* _FUNCTIONS_H */
