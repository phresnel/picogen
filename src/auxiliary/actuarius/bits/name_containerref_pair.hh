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

#ifndef NAME_CONTAINERREF_PAIR_HH_INCLUDED_20090906
#define NAME_CONTAINERREF_PAIR_HH_INCLUDED_20090906

namespace actuarius {

//-----------------------------------------------------------------------------
// nrp - name reference-container pair
//-----------------------------------------------------------------------------
template <typename T> struct ncrp {
        const char *name;
        T &value;
        
        ncrp (const char *name, T &value) : name(name), value (value) {}
                
private:
        ncrp operator = (ncrp const &) ;
};

template <typename T> ncrp<T> make_ncrp (const char *name, T &value) {
        return ncrp<T> (name, value);
}

}

#endif // NAME_CONTAINERREF_PAIR_HH_INCLUDED_20090906
