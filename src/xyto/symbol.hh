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

#ifndef SYMBOL_HH_INCLUDED_20100726
#define SYMBOL_HH_INCLUDED_20100726

#include <string>
#include "parameterlist.hh"

class ParameterList;

class Symbol {
public:
        Symbol();
        Symbol (Symbol const &rhs);
        Symbol& operator= (Symbol const &rhs);

        std::string name() const;
        void setName(std::string const &name);

        ParameterList parameterList() const;
        ParameterList &parameterList();
        void setParameterList(ParameterList const &rhs);
private:
        std::string name_;
        ParameterList parameterList_;
};

bool operator == (Symbol const &lhs, Symbol const &rhs);
bool operator != (Symbol const &lhs, Symbol const &rhs);

#endif // SYMBOL_HH_INCLUDED_20100726
