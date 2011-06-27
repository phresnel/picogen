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

#ifndef COLOR_HH_INCLUDED_20101013
#define COLOR_HH_INCLUDED_20101013

#include <vector>
#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

namespace picogen { namespace redshift_file {
        // Rgb.
        struct WavelengthAmplitudePair {
                double wavelength;
                double amplitude;

                template<typename Arch> void serialize (Arch &arch);
        };

        struct Spectrum {
                std::vector<WavelengthAmplitudePair> samples;
                template<typename Arch> void serialize (Arch &arch);
        };

        struct Rgb {
                double r,g,b;

                Rgb (double r, double g, double b) : r(r), g(g), b(b) {}
                Rgb () : r(1), g(1), b(1) {}

                template<typename Arch> void serialize (Arch &arch);
        };        

        struct Color {

                enum Type {
                        RGB, Spectrum
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Rgb rgb;
                redshift_file::Spectrum spectrum;

                Color () : type(RGB), rgb(0,1,1) {}
                Color (double r, double g, double b) : type(RGB), rgb(r,g,b) {}                

                template<typename Arch> void serialize (Arch &arch);
        };
} }

#endif // COLOR_HH_INCLUDED_20101013
