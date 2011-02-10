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


#ifndef COLOR_SERIALIZATION_HH_20101012
#define COLOR_SERIALIZATION_HH_20101012

#include "../color.hh"
#include "cosyfloat.ser.hh"

namespace cosyscene {

//------------------------------------------------------------------------------
template<typename Arch>
inline void WavelengthAmplitudePair::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack(wavelength_) & pack(amplitude_);
}


//------------------------------------------------------------------------------
template<typename Arch>
inline void Spectrum::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack(samples_);
}

//------------------------------------------------------------------------------
template<typename Arch>
inline void Rgb::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack(r_) & pack(g_) & pack(b_);
}

//------------------------------------------------------------------------------
template<typename Arch>
inline void Color::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack ("type", kind_, Typenames);

        switch (kind_) {
        case Rgb: arch & pack ("parameters", rgb_); break;
        case Spectrum: arch & pack ("parameters", spectrum_); break;
        }
}


} // namespace cosyscene

#endif // COLOR_SERIALIZATION_HH_20101012
