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

#ifndef NAME_REF_PAIR_HH_INCLUDED_20090829
#define NAME_REF_PAIR_HH_INCLUDED_20090829

namespace actuarius {

//-----------------------------------------------------------------------------
// nrp - name reference pair
//-----------------------------------------------------------------------------
template <typename T> struct nrp {
        const char *name;
        T &value;
        
        nrp (const char *name, T &value) : name(name), value (value) {}
                
private:
        nrp operator = (nrp const &) ;
};

template <typename T> nrp<T> make_nrp (const char *name, T &value) {
        return nrp<T> (name, value);
}

}

#endif // NAME_REF_PAIR_HH_INCLUDED_20090829
