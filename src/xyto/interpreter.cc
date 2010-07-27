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

#include <boost/optional.hpp>
#include "pattern.hh"
#include "production.hh"
#include <iostream>
#include "xyto_ios.hh"

namespace {

unsigned int match (Pattern const &pattern,
                 Pattern const &axiom,
                 int axiomIndex
) {
        if (axiomIndex<0)
                return 0;
        for (unsigned int i=0; i<pattern.size(); ++i) {
                if (i+axiomIndex >= axiom.size()) return 0;
                if (pattern[i] != axiom[i+axiomIndex])
                        return 0;
        }
        return pattern.size();
}

unsigned int match (Production const &production,
                 Pattern const &axiom,
                 int axiomIndex
) {
        const ProductionHeader &header = production.header();
        const int mainLen = match (header.pattern(), axiom, axiomIndex);
        if (0 == mainLen)
                return 0;

        if (!header.leftContext().empty()) {
                Pattern const & ct = header.leftContext();
                if (!match (ct, axiom, axiomIndex-ct.size()))
                        return 0;
        }
        if (!header.rightContext().empty()) {
                Pattern const & ct = header.rightContext();
                if (!match (ct, axiom, axiomIndex+mainLen))
                        return 0;
        }
        return mainLen;
}


void fillStack (
        Pattern const &pattern,
        Pattern const &axiom, unsigned int axiomIndex, int axiomOffset,
        std::vector<Parameter> &stack
) {
        // Step 0) Reap values from axiom.
        /*
            axiom:       x(1,2)
            production:  x(#0,#1) -> x(#1,#0)
        */
        for (unsigned int i=0; i<pattern.size(); ++i) {
                Segment const &sym = pattern[i];
                Segment const &xsym = axiom[(i+axiomIndex)+axiomOffset];

                if (sym.type() != xsym.type()) {
                        std::cerr << "internal error: divergence of "
                                "segment-types within function "
                                "interpreter.cc:fillStack(.) ["
                                << "'"<<sym<<"' / '"<<xsym<<"'"
                                << "]"
                                << std::endl;
                        return;
                }

                switch (sym.type()) {
                case Segment::Letter: {
                        ParameterList const &paramList = sym.parameterList();
                        ParameterList const &xparamList = xsym.parameterList();
                        for (unsigned int p=0; p<paramList.size(); ++p) {
                                Parameter const &param = paramList[p];
                                Parameter const &xparam = xparamList[p];

                                int const paramIndex = param.parameterIndex();
                                if (paramIndex >= (int)stack.size()) {
                                        stack.resize (paramIndex+1);
                                }

                                stack[paramIndex] = xparam;
                        }
                } break;
                case Segment::Branch: {
                        fillStack(sym.branch(),
                                  xsym.branch(), 0, 0,
                                  stack);
                } break;
                }
        }
}

Segment applyStack (Segment const &symbol, std::vector<Parameter> const &stack);
Pattern applyStack (Pattern const &pattern, std::vector<Parameter> const &stack);

Pattern applyStack (Pattern const &pattern, std::vector<Parameter> const &stack) {
        Pattern ret;
        for (unsigned int i=0; i<pattern.size(); ++i) {
                ret.push_back(applyStack(pattern[i], stack));
        }
        return ret;
}




Segment applyStack (Segment const &symbol, std::vector<Parameter> const &stack) {

        Segment ret = symbol;

        switch (symbol.type()) {
        case Segment::Letter: {
                ParameterList const &params = symbol.parameterList();
                ParameterList &rparams = ret.parameterList();
                for (unsigned int p=0; p<params.size(); ++p) {
                        Parameter const &param = params[p];
                        Parameter &rparam = rparams[p];
                        if (param.type() == Parameter::ParameterIndex) {
                                rparam = stack[param.parameterIndex()];
                        } else {
                                rparam = param;
                        }
                }
        } break;
        case Segment::Branch: {
                ret.setBranch (applyStack (symbol.branch(), stack));
        } break;
        }

        return ret;
}

} // namespace {



boost::optional<Pattern> apply(std::vector<Production> const &prods, Pattern const &axiom) {
        bool axiomWasTweaked = false;
        Pattern ret;
        for (unsigned int A=0; A<axiom.size(); ) {
                bool any = false;
                for (unsigned int P=0; P<prods.size(); ++P) {
                        const int len = match(prods[P], axiom, A);
                        const bool doesMatch = len > 0;
                        if (doesMatch) {
                                any = true;

                                const Pattern &lcPattern = prods[P].header().leftContext();
                                const Pattern &rcPattern = prods[P].header().rightContext();
                                const Pattern &mPattern  = prods[P].header().pattern();
                                const Pattern &body = prods[P].body().pattern();

                                std::vector<Parameter> stack(16);

                                // Fill stack with values from axiom.
                                // E.g., axiom = "a(x) < b(y) c(z) > d(a)"

                                fillStack (lcPattern, axiom, A, -lcPattern.size(), stack);
                                fillStack (mPattern,  axiom, A, 0, stack);
                                fillStack (rcPattern, axiom, A, mPattern.size(), stack);

                                ret = applyStack(body, stack);

                                A += len;
                                break;
                        }
                }
                if (any) {
                        // NOTE: this is actually wrond. in many cases, doesMatch will be true,
                        //       but the resultant axiom wasn't really tweaked
                        axiomWasTweaked = true;
                } else {
                        ret.push_back (axiom[A]);
                        ++A;
                }
        }

        if (axiomWasTweaked)
                return ret;
        return boost::optional<Pattern>();
}


