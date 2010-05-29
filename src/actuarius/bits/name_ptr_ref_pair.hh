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

#ifndef NAME_PTR_REF_PAIR_HH_INCLUDED_20100330
#define NAME_PTR_REF_PAIR_HH_INCLUDED_20100330

namespace actuarius {

//-----------------------------------------------------------------------------
// nrp - name ptr enum reference-to-container pair
//-----------------------------------------------------------------------------
template <typename T, typename ADVICE_TYPE>
struct nprp {
        const char *name;
        ADVICE_TYPE T::*ptr;

        T &value;

        nprp (const char *name, ADVICE_TYPE T::* ptr,
                T &value)
        : name(name), ptr(ptr), value (value) {}

private:
        nprp operator = (nprp const &) ;
};

template <typename T, typename ADVICE_TYPE>
nprp<T,ADVICE_TYPE> make_nprp (
        const char *name,
        ADVICE_TYPE T::* ptr,
        T &value
) {
        return nprp<T,ADVICE_TYPE> (
                name, ptr, value
        );
}

}

#endif // NAME_PTR_REF_PAIR_HH_INCLUDED_20100330
