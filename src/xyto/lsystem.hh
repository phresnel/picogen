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

#ifndef LSYSTEM_HH_INCLUDED_20100802
#define LSYSTEM_HH_INCLUDED_20100802

#include <vector>
#include "pattern.hh"
#include "production.hh"

namespace kallisto { namespace random { namespace marsaglia {
        class UNI;
} } }

class LSystem {
public:
        Pattern axiom () const;
        void setAxiom (const Pattern &axiom);

        Production production(unsigned int index) const;
        void setProductions (std::vector<Production> const &);

        Pattern run (kallisto::random::marsaglia::UNI &rng,
                     unsigned int count) const;
        Pattern run (unsigned int count) const;
private:

        std::vector<Production> productions_;
        Pattern axiom_;
};

#endif // LSYSTEM_HH_INCLUDED_20100802
