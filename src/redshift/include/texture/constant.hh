//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef CONSTANT_HH_INCLUDED_20100817
#define CONSTANT_HH_INCLUDED_20100817

#include "../basictypes/texture.hh"

namespace redshift {
        namespace texture {
                class ConstantColor : public ColorTexture {
                public:
                        ConstantColor (Color const &color, real_t alpha=1)
                        : color_(color), alpha_(alpha)
                        {}
                        virtual ~ConstantColor() {}

                        Color color(DifferentialGeometry const &) const {
                                return color_;
                        }

                        real_t alpha (DifferentialGeometry const &) const {
                                return alpha_;
                        }

                private:
                        Color color_;
                        real_t alpha_;
                };

                class ConstantScalar : public ScalarTexture {
                public:
                        ConstantScalar (real_t value)
                                : value_(value)
                                {}
                        virtual ~ConstantScalar() {}

                        real_t value(DifferentialGeometry const &) const {
                                return value_;
                        }
                private:
                        real_t value_;
                };
        }
}

#endif // CONSTANT_HH_INCLUDED_20100817
