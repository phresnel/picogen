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

#ifndef QUATSCHPP_IOS_HH_20101118_INCLUDED
#define QUATSCHPP_IOS_HH_20101118_INCLUDED

#include "meta.hh"

namespace quatsch_preprocessor {

std::ostream& operator<< (std::ostream& o, DomainScalar const& v);
std::ostream& operator<< (std::ostream& o, DomainInterval const& v);
std::ostream& operator<< (std::ostream& o, DomainValue const& v);
std::ostream& operator<< (std::ostream& o, Domain const& v);
std::ostream& operator<< (std::ostream& o, DeclaredType const& v);
std::ostream& operator<< (std::ostream& o, Declaration const& v);

}
#endif // QUATSCHPP_IOS_HH_20101118_INCLUDED
