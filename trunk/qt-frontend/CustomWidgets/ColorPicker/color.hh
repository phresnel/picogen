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

#ifndef COLOR_HH
#define COLOR_HH

#include <cmath>

namespace color {
        struct rgbf {
                float r, g, b;
                rgbf (float r, float g, float b)
                : r(r), g(g), b(b) {}
                rgbf () : r(0), g(0), b(0) {}
        };
        struct hsvf {
                float h, s, v;
                hsvf (float h, float s, float v)
                : h(h), s(s), v(v) {}
                hsvf () : h(0), s(0), v(0) {}
        };

        const float pi =
          (float)3.141592653589793238462643383279502884197169399375105820974944;

        inline rgbf hsv_to_rgb (float h, float s, float v) {
                const float
                        f = h / 60 - std::floor (h / 60),
                        p = v * (1 - s),
                        q = v * (1 - f * s),
                        t = v * (1- (1 - f) * s)
                ;
                switch ((int)floor (h / 60.0) % 6) {
                case 0: return rgbf (v, t, p);
                case 1: return rgbf (q, v, p);
                case 2: return rgbf (p, v, t);
                case 3: return rgbf (p, q, v);
                case 4: return rgbf (t, p, v);
                case 5: return rgbf (v, p, q);
                }
                // can't happen
                throw;//return rgbf_to_rgb (0, 0, 0);
        }

        inline hsvf rgb_to_hsv (float r, float g, float b) {
                const float
                        min_ = r < g ? r : g,
                        min  = min_ < b ? min_ : b,
                        max_ = r > g ? r : g,
                        max  = max_ > b ? max_ : b,
                        range = max - min
                ;
                const float
                        H_ =
                                range<=0.f ? 0.f :
                                max==r ? 60.f * (0 + (g-b)/range) :
                                max==g ? 60.f * (2 + (b-r)/range) :
                                /*max==b ?*/ 60.f * (4 + (r-g)/range),
                        H = H_<0?H_+360:H_
                ;
                const float S = max<=0
                        ? 0
                        : range/max
                ;
                const float V = max;

                return hsvf (H,S,V);
        }
        inline hsvf rgb_to_hsv (rgbf const & col) {
                return rgb_to_hsv(col.r, col.g, col.b);
        }
        inline rgbf hsv_to_rgb (hsvf const & col) {
                return hsv_to_rgb(col.h, col.s, col.v);
        }
}



#endif // COLOR_HH
