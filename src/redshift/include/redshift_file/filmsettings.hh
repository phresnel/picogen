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

#ifndef FILMSETTINGS_HH_20101013
#define FILMSETTINGS_HH_20101013

namespace redshift_file {
        struct FilmSettings {
                double colorscale;
                bool convertToSrgb;

                FilmSettings ()
                : colorscale(1)
                , convertToSrgb(false)
                {}

                FilmSettings (FilmSettings const &rhs)
                : colorscale(rhs.colorscale)
                , convertToSrgb(rhs.convertToSrgb)
                {}

                FilmSettings &operator = (FilmSettings const &rhs) {
                        colorscale = rhs.colorscale;
                        convertToSrgb = rhs.convertToSrgb;
                        return *this;
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("color-scale", colorscale);
                        arch & pack ("convert-to-srgb", convertToSrgb);
                }
        };
}

#endif // FILMSETTINGS_HH_20101013
