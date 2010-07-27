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

#include "symbol.hh"
#include "parameterlist.hh"

std::string Symbol::name() const {
        return name_;
}


void Symbol::setName(std::string const &name) {
        name_ = name;
}



ParameterList Symbol::parameterList() const {
        return parameterList_;
}



ParameterList &Symbol::parameterList() {
        return parameterList_;
}



void Symbol::setParameterList(ParameterList const &rhs) {
        parameterList_ = rhs;
}



Symbol::Symbol() {
}



Symbol::Symbol (Symbol const &rhs)
: name_(rhs.name_)
, parameterList_(rhs.parameterList_)
{
}



Symbol& Symbol::operator= (Symbol const &rhs) {
        name_ = rhs.name_;
        parameterList_ = rhs.parameterList_;
        return *this;
}


bool operator == (Symbol const &lhs, Symbol const &rhs) {
        return lhs.name() == rhs.name()
            && lhs.parameterList().size() == rhs.parameterList().size();
}



bool operator != (Symbol const &lhs, Symbol const &rhs) {
        return !(lhs == rhs);
}
