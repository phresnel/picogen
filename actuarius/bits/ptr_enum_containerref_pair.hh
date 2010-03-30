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

#ifndef PTR_ENUM_CONTAINERREF_PAIR_HH_INCLUDED_20100330
#define PTR_ENUM_CONTAINERREF_PAIR_HH_INCLUDED_20100330

namespace actuarius {

//-----------------------------------------------------------------------------
// nrp - name ptr enum reference-to-container pair
//-----------------------------------------------------------------------------
template <typename CONT, typename ADVICE_TYPE>
struct pecrp {
        ADVICE_TYPE CONT::value_type:: *ptr;

        Enum<ADVICE_TYPE> enumDesc;
        CONT &value;

        pecrp (ADVICE_TYPE CONT::value_type::* ptr,
                Enum<ADVICE_TYPE> enumDesc, CONT &value)
        : ptr(ptr), enumDesc(enumDesc), value (value) {}

private:
        pecrp operator = (pecrp const &) ;
};

template <typename CONT, typename ADVICE_TYPE>
pecrp<CONT,ADVICE_TYPE> make_pecrp (
        ADVICE_TYPE CONT::value_type::* ptr,
        Enum<ADVICE_TYPE> enumDesc,
        CONT &value
) {
        return pecrp<CONT,ADVICE_TYPE> (
                ptr, enumDesc, value
        );
}

}

#endif // PTR_ENUM_CONTAINERREF_PAIR_HH_INCLUDED_20100330
