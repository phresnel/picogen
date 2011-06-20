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

#ifndef FILENAME_EXTENSION_HH_INCLUDED_20100701
#define FILENAME_EXTENSION_HH_INCLUDED_20100701

#include <cstring>
#include <algorithm>

inline std::string filename_extension (const std::string &str) {
        using std::string;
        string ret;
        for (string::const_reverse_iterator it=str.rbegin(); it!=str.rend(); ++it) {
                if ('.' == *it) {
                        reverse (ret.begin(), ret.end());
                        return ret;
                }
                ret += *it;
        }
        return "";
}

#endif // FILENAME_EXTENSION_HH_INCLUDED_20100701
