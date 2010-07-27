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

#include "pattern.hh"
#include "segment.hh"

Segment const& Pattern::operator[] (unsigned int i) const {
        return symbols[i];
}



Segment & Pattern::operator[] (unsigned int i) {
        return symbols[i];
}



unsigned int Pattern::size() const {
        return symbols.size();
}



void Pattern::push_back(Segment const &sym) {
        symbols.push_back(sym);
}



bool Pattern::empty() const {
        return size() == 0;
}



Pattern::Pattern() {
}



Pattern::Pattern(Pattern const &rhs)
: symbols(rhs.symbols)
{
}



Pattern& Pattern::operator= (Pattern const &rhs) {
        symbols = rhs.symbols;
        return *this;
}



bool operator == (Pattern const &lhs, Pattern const &rhs) {
        if (lhs.size() != rhs.size())
                return false;
        for (unsigned int i=0; i<lhs.size(); ++i) {
                if (lhs[i] != rhs[i])
                        return false;
        }
        return true;
}

