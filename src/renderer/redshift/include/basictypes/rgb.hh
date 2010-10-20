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

#ifndef RGB_H_INCLUDED_20090224
#define RGB_H_INCLUDED_20090224

#include <cmath>
#include "real.hh"

namespace redshift {

        struct /*__attribute__((deprecated))*/ SRgb {
                real_t r, g, b;

                SRgb () : r(0), g(0), b(0) {}

                SRgb (real_t r_, real_t g_, real_t b_)
                : r(r_) , g(g_), b(b_)
                {}
        };


        namespace color {

                struct XYZ;
                struct xyY;
                struct RGB;
                struct SRGB;

                struct XYZ {
                        real_t X, Y, Z;

                        XYZ () : X(0), Y(0), Z(0) {}
                        XYZ (real_t X, real_t Y, real_t Z)
                        : X(X), Y(Y), Z(Z)
                        {}

                        RGB toRGB() const ;
                };

                struct xyY {
                        real_t x, y, Y;

                        xyY () : x(0), y(0), Y(0) {}
                        xyY (real_t x, real_t y, real_t Y)
                        : x(x), y(y), Y(Y)
                        {}

                        RGB toRGB() const ;
                        XYZ toXYZ() const ;
                };

                struct RGB {
                        real_t R, G, B;

                        RGB () : R(0), G(0), B(0) {}
                        RGB (real_t R, real_t G, real_t B)
                        : R(R), G(G), B(B)
                        {}

                        XYZ toXYZ() const ;
                        SRGB toSRGB() const ;
                };

                struct SRGB {
                        real_t R, G, B;

                        SRGB () : R(0), G(0), B(0) {}
                        SRGB (real_t R, real_t G, real_t B)
                        : R(R), G(G), B(B)
                        {}
                };


                inline RGB XYZ::toRGB () const {
                        return RGB(
                         3.2404542*X + -1.5371385*Y + -0.4985314*Z,
                        -0.9692660*X +  1.8760108*Y +  0.0415560*Z,
                         0.0556434*X + -0.2040259*Y +  1.0572252*Z
                        );
                }

                inline XYZ RGB::toXYZ () const {
                        return XYZ(
                        0.412453f*R + 0.357580f*G + 0.180423f*B,
                        0.212671f*R + 0.715160f*G + 0.072169f*B,
                        0.019334f*R + 0.119193f*G + 0.950227f*B
                        );
                }

                inline SRGB RGB::toSRGB () const {
                        using std::pow;
                        SRGB srgb(R,G,B);
                        if ( srgb.R > 0.0031308 ) srgb.R = 1.055 * ( pow(srgb.R,(1/2.4) ) ) - 0.055;
                        else                 srgb.R = 12.92 * srgb.R;
                        if ( srgb.G > 0.0031308 ) srgb.G = 1.055 * ( pow(srgb.G,(1/2.4) ) ) - 0.055;
                        else                 srgb.G = 12.92 * srgb.G;
                        if ( srgb.B > 0.0031308 ) srgb.B = 1.055 * ( pow(srgb.B,(1/2.4) ) ) - 0.055;
                        else                 srgb.B = 12.92 * srgb.B;
                        return srgb;
                }

                inline RGB xyY::toRGB () const {
                        return toXYZ().toRGB();
                }

                inline XYZ xyY::toXYZ () const {
                        if (y==0)
                                return XYZ(0,0,0);
                        return XYZ(
                                x * ( Y / y ),
                                Y,
                                (1-x-y) * ( Y / y )
                        );
                }
        }

        // Deprecated, in future, Rgb should be a lightweight
        // class only to be used for color space conversions.
        struct /*__attribute__((deprecated))*/ Rgb {
                real_t r, g, b;

                Rgb () : r(0), g(0), b(0) {}

                explicit Rgb (real_t f) : r(f), g(f), b(f) {}

                Rgb (real_t r_, real_t g_, real_t b_)
                : r(r_) , g(g_), b(b_)
                {}

                Rgb (Rgb const &rgb)
                : r(rgb.r) , g(rgb.g), b(rgb.b)
                {}

                int numComponents() const { return 3; }
                enum { num_components = 3 };

                real_t operator [] (int index) const {
                        switch (index) {
                        case 0: return r;
                        case 1: return g;
                        case 2: return b;
                        };
                }
                real_t & operator [] (int index) {
                        switch (index) {
                        case 0: return r;
                        case 1: return g;
                        case 2: return b;
                        };
                }

                operator SRgb () const {
                        using std::pow;
                        SRgb srgb(r,g,b);
                        if ( srgb.r > 0.0031308 ) srgb.r = 1.055 * ( pow(srgb.r,(1/2.4) ) ) - 0.055;
                        else                 srgb.r = 12.92 * srgb.r;
                        if ( srgb.g > 0.0031308 ) srgb.g = 1.055 * ( pow(srgb.g,(1/2.4) ) ) - 0.055;
                        else                 srgb.g = 12.92 * srgb.g;
                        if ( srgb.b > 0.0031308 ) srgb.b = 1.055 * ( pow(srgb.b,(1/2.4) ) ) - 0.055;
                        else                 srgb.b = 12.92 * srgb.b;
                        return srgb;
                }

                /*operator Yxy () const {

                }*/

                real_t Y () const {
                        return 0.299*r + 0.567*g + 0.114*b;
                }

                Rgb &operator = (Rgb const &rhs) {
                        this->r = rhs.r;
                        this->g = rhs.g;
                        this->b = rhs.b;
                        return *this;
                }

                Rgb toRgb () const {
                        return Rgb (r, g, b);
                }

                static Rgb fromRgb (real_t r, real_t g, real_t b) {
                        return Rgb(r,g,b);
                }

                void fromRgb (Rgb const & v) {
                        this->r = v.r;
                        this->g = v.g;
                        this->b = v.b;
                }

                void fromYxy (real_t Y, real_t x, real_t y) {
                        real_t CIE_X = x * ( Y / y );
                        real_t CIE_Y = Y;
                        real_t CIE_Z = (1-x-y) * ( Y / y );

                        /*CIE_X /= 100.0;
                        CIE_Y /= 100.0;
                        CIE_Z /= 100.0;*/

                        r = 2.565*CIE_X -1.167*CIE_Y -0.398*CIE_Z;
                        g = -1.022*CIE_X +1.978*CIE_Y +0.044*CIE_Z;
                        b =  0.075*CIE_X -0.252*CIE_Y +1.177*CIE_Z;
/*
                        if ( r > 0.0031308 ) r = 1.055 * ( pow(r,(1/2.4) ) ) - 0.055;
                        else                 r = 12.92 * r;
                        if ( g > 0.0031308 ) g = 1.055 * ( pow(g,(1/2.4) ) ) - 0.055;
                        else                 g = 12.92 * g;
                        if ( b > 0.0031308 ) b = 1.055 * ( pow(b,(1/2.4) ) ) - 0.055;
                        else                 b = 12.92 * b;*/
                }

                Rgb & operator += (Rgb const &rhs) {
                        r += rhs.r;
                        g += rhs.g;
                        b += rhs.b;
                        return *this;
                }
        };

        inline Rgb exp (Rgb const &v) {
                return Rgb (std::exp(v.r),std::exp(v.g),std::exp(v.b));
        }

        inline Rgb saturate (Rgb const &v, real_t min_, real_t max_) {
                return Rgb (
                        v.r<min_ ? min_:v.r>max_ ? max_:v.r,
                        v.g<min_ ? min_:v.g>max_ ? max_:v.g,
                        v.b<min_ ? min_:v.b>max_ ? max_:v.b
                );
        }

        inline Rgb operator * (Rgb const &lhs, real_t rhs) {
                return Rgb (
                        lhs.r * rhs,
                        lhs.g * rhs,
                        lhs.b * rhs
                );
        }

        inline Rgb operator * (real_t lhs, Rgb const &rhs) {
                return Rgb (
                        lhs * rhs.r,
                        lhs * rhs.g,
                        lhs * rhs.b
                );
        }

        inline Rgb operator / (Rgb const & lhs, real_t rhs) {
                return Rgb (
                        lhs.r / rhs,
                        lhs.g / rhs,
                        lhs.b / rhs
                );
        }

        inline Rgb multiplyComponents (Rgb const&lhs, Rgb const&rhs) {
                return Rgb (
                        lhs.r * rhs.r,
                        lhs.g * rhs.g,
                        lhs.b * rhs.b
                );
        }

        inline Rgb operator * (Rgb const&lhs, Rgb const&rhs) {
                return Rgb (
                        lhs.r * rhs.r,
                        lhs.g * rhs.g,
                        lhs.b * rhs.b
                );
        }

        inline Rgb operator + (Rgb const&lhs, Rgb const&rhs) {
                return Rgb (
                        lhs.r + rhs.r,
                        lhs.g + rhs.g,
                        lhs.b + rhs.b
                );
        }

        inline Rgb operator - (Rgb const&lhs) {
                return Rgb (-lhs.r,-lhs.g,-lhs.b);
        }
}



#endif // RGB_H_INCLUDED_20090224
