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

namespace redshift {

        struct Rgb {
                real_t r, g, b;

                Rgb () : r(0), g(0), b(0) {}

                Rgb (real_t r_, real_t g_, real_t b_)
                : r(r_) , g(g_), b(b_)
                {}

                Rgb (Rgb const &rgb)
                : r(rgb.r) , g(rgb.g), b(rgb.b)
                {}

                Rgb toRgb () const {
                        return Rgb (r, g, b);
                }
                
                void fromRgb (Rgb const & v) {
                        this->r = v.r;
                        this->g = v.g;
                        this->b = v.b;
                }
        };
        
        inline Rgb saturate (Rgb const &v, real_t min, real_t max) {
                return Rgb (
                        v.r<min ? min:v.r>max ? max:v.r,
                        v.g<min ? min:v.g>max ? max:v.g,
                        v.b<min ? min:v.b>max ? max:v.b
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
}

#endif // RGB_H_INCLUDED_20090224
