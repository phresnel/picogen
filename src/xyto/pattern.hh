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

#ifndef PATTERN_HH_INCLUDED_20100726
#define PATTERN_HH_INCLUDED_20100726

#include <vector>
#include "symbol.hh"

class Symbol;
class Pattern {
public:
        Pattern();
        Pattern(Pattern const &rhs);
        Pattern& operator= (Pattern const &rhs);

        Symbol const& operator[] (unsigned int i) const;
        Symbol & operator[] (unsigned int i);

        unsigned int size() const;
        void push_back(Symbol const &sym);
        bool empty() const;
private:
        std::vector<Symbol> symbols;
};

bool operator == (Pattern const &lhs, Pattern const &rhs);

#endif // PATTERN_HH_INCLUDED_20100726
