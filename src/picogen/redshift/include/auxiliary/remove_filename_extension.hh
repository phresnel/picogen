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

#ifndef REMOVE_FILENAME_EXTENSION_HH_INCLUDED_20100715
#define REMOVE_FILENAME_EXTENSION_HH_INCLUDED_20100715

#include <cstring>
#include <algorithm>

#include "filename_extension.hh"

namespace picogen { namespace redshift {
inline std::string remove_filename_extension (const std::string &str) {
        using std::string;
        const string ext = filename_extension(str);
        if (ext == "")
                return str;
        return str.substr(0, (str.length()-ext.length())-1); // "ext" + "."
}
} }

#endif // FILENAME_EXTENSION_HH_INCLUDED_20100701
