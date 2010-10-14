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

#ifndef COLOR_SER_HH_20101013
#define COLOR_SER_HH_20101013

#include "../color.hh"

namespace redshift_file {
        template<typename Arch>
        void WavelengthAmplitudePair::serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack(wavelength) & pack(amplitude);
        }
        
        template<typename Arch>
        void Spectrum::serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack(samples);
        }
        
        template<typename Arch>
        void Rgb::serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack(r) & pack(g) & pack(b);
        }
        
        template<typename Arch>
        void Color::serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack ("type", type, Typenames);

                switch (type) {
                case RGB:
                        arch & pack("rgb", rgb);
                        break;
                case Spectrum:
                        arch & pack("spectrum", spectrum);
                        break;
                }
        }
}

#endif // COLOR_SER_HH_20101013
