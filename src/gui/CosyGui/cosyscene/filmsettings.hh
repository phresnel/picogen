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

#ifndef FILMSETTINGS_HH_20101216
#define FILMSETTINGS_HH_20101216

#include "stash.hh"
#include "redshift/include/smart_ptr.hh"
#include <actuarius/bits/enum.hh>

namespace cosyscene {

class FilmSettings : public StashableMixin<FilmSettings>
{
public:
        FilmSettings();

        double brightnessFactor() const;
        void setBrightnessFactor(double);

        template<typename Arch> void serialize (Arch &arch);
        bool data_equals(FilmSettings const &rhs) const;
private:

        double brightnessFactor_;

        void reset();
};

} // namespace cosyscene

#endif // FILMSETTINGS_HH_20101216
