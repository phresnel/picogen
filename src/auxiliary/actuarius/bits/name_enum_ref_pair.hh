//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef NAME_ENUM_REF_PAIR_HH_INCLUDED_20090829
#define NAME_ENUM_REF_PAIR_HH_INCLUDED_20090829

namespace actuarius {
        
//-----------------------------------------------------------------------------
// nerp - name enum reference pair 
//-----------------------------------------------------------------------------
template <typename T> struct nerp {
        const char *name;
        Enum<T> enumDesc;
        T &value;
        
        nerp (const char *name, Enum<T> enumDesc, T &value)
        : name(name), enumDesc (enumDesc), value (value) {}
                
private:
        nerp operator = (nerp const &) ;
};

template <typename T> nerp<T> make_nerp (
        const char *name, Enum<T> enumDesc, T &value
) {
        return nerp<T> (name, enumDesc, value);
}

}

#endif // NAME_ENUM_REF_PAIR_HH_INCLUDED_20090829
