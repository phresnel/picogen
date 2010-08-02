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
#include "production.hh"
#include "lsystem.hh"
#include "kiss.hh"
#include "xyto_ios.hh"

Pattern LSystem::axiom () const {
        return axiom_;
}


void LSystem::setAxiom (const Pattern &axiom) {
        axiom_ = axiom;
}


Production LSystem::production(unsigned int index) const {
        return productions_[index];
}


void LSystem::setProductions (std::vector<Production> const &rhs) {
        productions_ = rhs;
}


Pattern LSystem::run (
        unsigned int count
) const {
        kallisto::random::marsaglia::UNI rng (1,2,3,4);
        rng.skip(1024);
        return run (rng, count);
}


Pattern LSystem::run (
        kallisto::random::marsaglia::UNI &rng,
        unsigned int count
) const {
        boost::optional<Pattern> apply(
                std::vector<Production> const &,
                Pattern const &,
                kallisto::random::marsaglia::UNI &rng
        );

        Pattern pat = axiom_;
        for (unsigned int step=0; step<count; ++step) {

                boost::optional<Pattern> next = apply (productions_,
                                                       pat,
                                                       rng);
                //std::cout << "step " << step+1 << ": ";
                if (next) {
                        pat = *next;
                        //std::cout << pat << '\n';
                } else {
                        //std::cout << "<no match>\n";
                        break;
                }
        }
        return pat;
}
