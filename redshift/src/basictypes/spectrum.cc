//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
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

#include "../include/redshift.hh"


#include <iostream>
void foo () {

        redshift::Spectrum<4> aa;
        aa += aa;
        aa -= aa;
        aa *= aa;
        aa /= aa;
        aa = aa + aa;
        //aa = aa * aa;
        aa = aa / aa;
        aa = aa - aa;

        aa+=1.f;
        aa-=1.f;
        aa*=1.f;
        aa/=1.f;
        aa = aa + 1.f;
        aa = aa * 1.f;
        aa = aa - 1.f;
        aa = aa / 1.f;
        aa = 2.5f * aa;

        aa += 1.3f * aa;
}
