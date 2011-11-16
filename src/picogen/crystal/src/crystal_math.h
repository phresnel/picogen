//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#ifndef CRYSTAL_MATH_H
#define CRYSTAL_MATH_H


#include <cmath>
namespace crystal {
        typedef float real;

        using std::sqrt;
        using std::pow;
        using std::fabs;
}

namespace crystal {
        struct rgb_t {
                real r, g, b; // 0=black, 1=white, outside values allowed
        };
}


namespace crystal {
        class Radiance {
        public:
                static Radiance FromRgb (real r, real g, real b)
                {
                        return Radiance(r,g,b);
                }

                static Radiance Gray (real f)
                {
                        return Radiance(f,f,f);
                }

                static Radiance Black ()
                {
                        return Gray(0);
                }

                static Radiance White ()
                {
                        return Gray(1);
                }

                explicit operator rgb_t() const
                {
                        return { r_, g_, b_ };
                }

                Radiance& operator += (Radiance const &rhs)
                {
                        r_ += rhs.r_;
                        g_ += rhs.g_;
                        b_ += rhs.b_;
                        return *this;
                }

                Radiance& operator -= (Radiance const &rhs)
                {
                        r_ -= rhs.r_;
                        g_ -= rhs.g_;
                        b_ -= rhs.b_;
                        return *this;
                }

                Radiance& operator *= (Radiance const &rhs)
                {
                        r_ *= rhs.r_;
                        g_ *= rhs.g_;
                        b_ *= rhs.b_;
                        return *this;
                }

                Radiance& operator /= (Radiance const &rhs)
                {
                        r_ /= rhs.r_;
                        g_ /= rhs.g_;
                        b_ /= rhs.b_;
                        return *this;
                }

                Radiance& operator *= (real rhs)
                {
                        r_ *= rhs;
                        g_ *= rhs;
                        b_ *= rhs;
                        return *this;
                }

                Radiance& operator /= (real rhs)
                {
                        r_ /= rhs;
                        g_ /= rhs;
                        b_ /= rhs;
                        return *this;
                }

        private:
                Radiance (real r, real g, real b) : r_(r), g_(g), b_(b)
                {
                }
                real r_, g_, b_;
        };

        inline Radiance operator + (Radiance lhs, Radiance const &rhs)
        {
                return lhs += rhs;
        }

        inline Radiance operator - (Radiance lhs, Radiance const &rhs)
        {
                return lhs -= rhs;
        }

        inline Radiance operator * (Radiance lhs, Radiance const &rhs)
        {
                return lhs *= rhs;
        }

        inline Radiance operator / (Radiance lhs, Radiance const &rhs)
        {
                return lhs /= rhs;
        }

        inline Radiance operator * (Radiance lhs, real rhs)
        {
                return lhs *= rhs;
        }

        inline Radiance operator / (Radiance lhs, real rhs)
        {
                return lhs /= rhs;
        }
}


namespace crystal {
        struct CameraSample {
        public:
                CameraSample (real imageX, real imageY, real lensU, real lensV)
                        : imageX_(imageX), imageY_(imageY)
                        , lensU_(lensU), lensV_(lensV)
                {
                }

                real imageX() const /* [0..height) */ { return imageX_; }
                real imageY() const /* [0..width)  */ { return imageY_; }
                real lensU () const /* [0..1)      */ { return lensU_;  }
                real lensV () const /* [0..1)      */ { return lensV_;  }
        private:
                real imageX_, imageY_, lensU_, lensV_;
        };
}



namespace crystal {
        class Ray {};
}

#endif // CRYSTAL_MATH_H
