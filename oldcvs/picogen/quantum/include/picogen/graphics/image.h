/***************************************************************************
 *            images.h
 *
 *  Fri Oct 12 18:53:22 2007
 *  Copyright  2007  Sebastian Mach
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


#ifndef _IMAGE_H
#define _IMAGE_H

namespace picogen {
    namespace graphics {
        namespace image {
            namespace color {
                class Color {
                    private:
                        typedef ::picogen::misc::prim::real real;
                        real r,g,b;
                    public:
                        Color (real R, real G, real B) : r (R), g (G), b (B) {
                            XRT_CALL_STACK_PUSH ("color::color( real R, real G, real B )");
                            XRT_CALL_STACK_POP();
                        }
                        Color (const Color &col) : r (col.r), g (col.g), b (col.b) {
                            XRT_CALL_STACK_PUSH ("color::color()");
                            XRT_CALL_STACK_POP();
                        }
                        Color() : r (0), g (0), b (0) {
                            XRT_CALL_STACK_PUSH ("color::color()");
                            XRT_CALL_STACK_POP();
                        }
                        inline void from_rgb (real R, real G, real B) {
                            XRT_CALL_STACK_PUSH ("inline color::from_rgb( real R, real G, real B )");
                            r = R;
                            g = G;
                            b = B;
                            XRT_CALL_STACK_POP();
                        }
                        inline void from_Yxy (real Y, real x, real y) {
                            real CIE_X = x * ( Y / y );
                            real CIE_Y = Y;
                            real CIE_Z = (1-x-y) * ( Y / y );

                            /*CIE_X /= 100.0;
                            CIE_Y /= 100.0;
                            CIE_Z /= 100.0;*/

                            r = 2.565*CIE_X -1.167*CIE_Y -0.398*CIE_Z;
                            g = -1.022*CIE_X +1.978*CIE_Y +0.044*CIE_Z;
                            b =  0.075*CIE_X -0.252*CIE_Y +1.177*CIE_Z;

                            if ( r > 0.0031308 ) r = 1.055 * ( pow(r,(1/2.4) ) ) - 0.055;
                            else                 r = 12.92 * r;
                            if ( g > 0.0031308 ) g = 1.055 * ( pow(g,(1/2.4) ) ) - 0.055;
                            else                 g = 12.92 * g;
                            if ( b > 0.0031308 ) b = 1.055 * ( pow(b,(1/2.4) ) ) - 0.055;
                            else                 b = 12.92 * b;
                        }
                        inline void from_rgb (const real * const rgb) {
                            XRT_CALL_STACK_PUSH ("inline color::from_rgb( real R, real G, real B )");
                            r = rgb[0];
                            g = rgb[1];
                            b = rgb[2];
                            XRT_CALL_STACK_POP();
                        }
                        inline void to_rgb (real &R, real &G, real &B) const {
                            XRT_CALL_STACK_PUSH ("inline color::to_rgb( real &R, real &G, real &B ) const");
                            R = r;
                            G = g;
                            B = b;
                            XRT_CALL_STACK_POP();
                        }
                        inline void to_rgb (real * const rgb) const {
                            XRT_CALL_STACK_PUSH ("inline void color::to_rgb( real * const rgb ) const");
                            rgb[0] = r;
                            rgb[1] = g;
                            rgb[2] = b;
                            XRT_CALL_STACK_POP();
                        }
                        inline Color operator + (const Color &col) const {
                            return Color (r+col.r, g+col.g, b+col.b);
                        }
                        inline Color operator += (const Color &col) {
                            return Color (r+=col.r, g+=col.g, b+=col.b);
                        }
                        inline Color operator * (const Color &col) const {
                            return Color (r*col.r, g*col.g, b*col.b);
                        }
                        inline Color operator *= (const Color &col) {
                            return Color (r*=col.r, g*=col.g, b*=col.b);
                        }
                        inline Color operator * (real f) const {
                            return Color (r*f, g*f, b*f);
                        }
                        inline Color operator *= (real f) {
                            return Color (r*=f, g*=f, b*=f);
                        }
                };
                class AverageColor {
                    private:
                        typedef ::picogen::misc::prim::real real;
                        Color m_color;
                        real  m_alpha;
                    private:
                        AverageColor (const Color &col, real alpha) : m_color (col), m_alpha (alpha) {}
                    public:
                        AverageColor() : m_color (0,0,0), m_alpha (0) {}
                        AverageColor (real r, real g, real b) : m_color (r,g,b), m_alpha (0) {}
                        AverageColor (const Color &col) : m_color (col), m_alpha (1) {}
                        inline int getAlpha() const {
                            return (int) m_alpha;
                        }
                        inline AverageColor operator + (const Color &A) const {
                            return AverageColor (m_color+A, m_alpha+1);
                        }
                        inline AverageColor operator += (const Color &A) {
                            return AverageColor (m_color+=A, m_alpha+=1);
                        }
                        inline AverageColor operator = (const Color &A) {
                            return AverageColor (m_color=A, m_alpha=1);
                        }
                        inline operator Color () const {
                            return m_color * (1.0/m_alpha);
                        }
                        inline Color avg() const {
                            return m_alpha==0 ?
                                   m_color * (1.0/m_alpha) :
                                   Color (1,0,0);
                        }
                };
            } // namespace color
        } //  namespace image
    } // namespace graphics
} //namespace picogen

#endif /* _IMAGE_H */
