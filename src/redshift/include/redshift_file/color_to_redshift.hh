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

#ifndef COLOR_TO_REDSHIFT_HH_20101014
#define COLOR_TO_REDSHIFT_HH_20101014

#include "redshift/include/basictypes/spectrum.hh"

// redshift_file fwd+Co.
namespace redshift_file {
        class Spectrum;
        class Rgb;
        class Color;
}

#include "redshift/include/smart_ptr.hh"

namespace redshift_file {
        redshift::Color toRedshift (Spectrum const &);
        redshift::Color toRedshift (Rgb const &, redshift::SpectrumKind kind);
        redshift::Color toRedshift (Color const &, redshift::SpectrumKind kind);
}

#endif // COLOR_TO_REDSHIFT_HH_20101014
