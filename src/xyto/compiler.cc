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

#include <map>
#include <set>
#include <iostream>

#include <boost/optional.hpp>

#include "production.hh"
#include "tokenize.hh"
#include "token.hh"
#include "xyto_ios.hh"

inline bool hasPrecedenceOver (Production const &lhs, Production const &rhs) {
        const int numContextsLhs =
                !lhs.header().leftContext().empty()
                + !lhs.header().rightContext().empty();
        const int numContextsRhs =
                !rhs.header().leftContext().empty()
                + !rhs.header().rightContext().empty();

        // Always prefer contexts over no context, as well as
        // two contexts over one context.
        if (numContextsLhs > numContextsRhs)
                return true;
        else if (numContextsLhs < numContextsRhs)
                return false;

        // Always prefer larger main-pattern.
        const int patternSizeLhs = lhs.header().pattern().size();
        const int patternSizeRhs = rhs.header().pattern().size();
        if (patternSizeLhs > patternSizeRhs)
                return true;
        else if (patternSizeLhs < patternSizeRhs)
                return false;

        // Prefer larger context.
        const int contextSizeLhs = lhs.header().leftContext().size()
                                 + lhs.header().rightContext().size();
        const int contextSizeRhs = rhs.header().leftContext().size()
                                 + rhs.header().rightContext().size();
        if (contextSizeLhs > contextSizeRhs)
                return true;
        else if (contextSizeLhs < contextSizeRhs)
                return false;

        return false;
}



inline bool ambiguous (Production const &lhs, Production const &rhs) {
        return lhs.header().pattern() == rhs.header().pattern()
            && !hasPrecedenceOver(lhs, rhs)
            && !hasPrecedenceOver(rhs, lhs)
            ;
}



boost::optional<Parameter> parse_parameter (
        TokenIterator it, TokenIterator end, TokenIterator &behind,
        bool isHeader
) {
        if (it == end)
                return boost::optional<Parameter>();
        if (it->type() == Token::Identifier) {
                Parameter p;
                p.setType (Parameter::Identifier);
                p.setIdentifier (it->value());
                behind = ++it;
                return p;
        }
        if (isHeader) {
                std::cerr
                  << "error: only a single identifier (e.g. 'id') "
                  << "is allowed as a formal parameter in the "
                  << "header of a production, line "
                  << it->from().row() << ", column "
                  << it->from().column() << std::endl;
                return boost::optional<Parameter>();
        }
        if (it->type() == Token::Real) {
                Parameter p;
                p.setType (Parameter::Real);
                p.setReal (it->valueAsReal());
                behind = ++it;
                return p;
        }
        if (it->type() == Token::Integer) {
                Parameter p;
                p.setType (Parameter::Integer);
                p.setInteger (it->valueAsInteger());
                behind = ++it;
                return p;
        }
        return boost::optional<Parameter>();
}



boost::optional<ParameterList> parse_parameter_list (
        TokenIterator it, TokenIterator end, TokenIterator &behind,
        bool isHeader
) {
        ParameterList ret;
        if (it == end || it->type() != Token::LeftParen) {
                behind = it;
                return ret;
        }
        ++it;
        if (it == end) {
                std::cerr << "error: unclosed parameter list in line "
                        << (it-1)->to().next().row() << ", column "
                        << (it-1)->to().next().column() << std::endl;
                return boost::optional<ParameterList>();
        }

        if (it->type() == Token::RightParen) {
                ++it;
        } else while (1) {
                TokenIterator behindParam;
                boost::optional<Parameter> p = parse_parameter(
                                          it, end, behindParam,
                                          isHeader);
                if (p) {
                        ret.push_back(*p);
                        it = behindParam;
                } else {
                        std::cerr <<
                           "error: expected parameter in line "
                           << it->from().next().row() << ", column "
                           << it->from().next().column() << std::endl;
                        return boost::optional<ParameterList>();
                }

                if (it == end) {
                        TokenIterator i = it - 1;
                        std::cerr <<
                           "error: unclosed parameter list in line "
                           << i->from().next().row() << ", column "
                           << i->from().next().column() << std::endl;
                        return boost::optional<ParameterList>();
                } else if (it->type() == Token::RightParen) {
                        ++it;
                        break;
                } else if (it->type() == Token::Comma) {
                        ++it;
                }
        }

        behind = it;
        return ret;
}



boost::optional<Symbol> parse_symbol (
        TokenIterator it, TokenIterator end, TokenIterator &behind,
        bool isHeader=false
) {
        if (it == end || it->type() != Token::Identifier)
                return boost::optional<Symbol>();
        Symbol sym;
        sym.setName (it->value());
        ++it;

        TokenIterator behindParams;
        boost::optional<ParameterList> params =
                parse_parameter_list(it, end, behindParams, isHeader);
        if (!params)
                return boost::optional<Symbol>();
        sym.setParameterList (*params);
        it = behindParams;
        behind = it;
        return sym;
}



Pattern parse_pattern (TokenIterator it, TokenIterator end,
                       TokenIterator &behind, bool isHeader=false) {
        Pattern ret;
        while (boost::optional<Symbol> sym = parse_symbol(it, end, behind, isHeader)) {
                it = behind;
                ret.push_back(*sym);
        }
        behind = it;
        return ret;
}



bool contains_unknowns (Pattern const &pat) {
        for (unsigned int i=0; i<pat.size(); ++i) {
                Symbol const & sym = pat[i];
                ParameterList const &pm = sym.parameterList();

                for (unsigned int p=0; p<pm.size(); ++p) {
                        if (pm[p].type() != Parameter::Integer
                         && pm[p].type() != Parameter::Real
                        ) {
                                return true;
                        }
                }
        }
        return false;
}



boost::optional<Pattern> parse_axiom (TokenIterator it, TokenIterator end) {
        TokenIterator trash;
        Pattern ret = parse_pattern (it, end, trash);
        if (contains_unknowns(ret)) {
                std::cerr << "error: axioms may not contain any unknowns"
                          << std::endl;
                return boost::optional<Pattern>();
        }
        return ret;
}



bool parse_header_patterns (
        TokenIterator it, TokenIterator end,
        TokenIterator &behind,
        Pattern &leftContext, Pattern &mainPattern, Pattern &rightContext
) {
        const TokenIterator beforePatterns = it;
        TokenIterator behindPattern;
        const Pattern pat = parse_pattern(it, end, behindPattern, true);
        TokenIterator prev = it;
        it = behindPattern;

        if (it != end && it->type() == Token::LessThan) {
                leftContext = pat;
                ++it;
                mainPattern = parse_pattern(it, end, behindPattern, true);
                prev = it; it = behindPattern;

                if (it != end && it->type() == Token::GreaterThan) {
                        ++it;
                        rightContext = parse_pattern(it, end,
                                                     behindPattern, true);
                        prev = it; it = behindPattern;
                }

        } else if (it != end && it->type() == Token::GreaterThan) {
                mainPattern = pat;
                ++it;
                rightContext = parse_pattern(it, end, behindPattern, true);
                prev = it; it = behindPattern;
        } else if (!pat.empty()) {
                mainPattern = pat;
        }

        if (mainPattern.empty()) {
                TokenIterator i = beforePatterns - 1;
                std::cerr <<
                   "error: no (valid) pattern found in line "
                   << i->from().next().row() << ", column "
                   << i->from().next().column() << std::endl;
                return boost::optional<Production>();
        }
        behind = it;

        return true;
}



boost::optional<ProductionHeader> parse_production_header (
        TokenIterator it, TokenIterator end, TokenIterator &behind
) {
        TokenIterator prev = it;

        if (Token::Identifier != it->type()) {
                std::cerr << "error: expected identifier at line "
                        << prev->from().row() << ", column "
                        << prev->from().column() << std::endl;
                return boost::optional<ProductionHeader>();
        }
        const std::string name = it->value();
        prev = it;
        ++it;
        // Checkpoint: We have an identifier.

        if (it==end || it->type() != Token::Colon) {
                std::cerr << "error: expected ':' at line "
                   << prev->to().next().row() << ", column "
                   << prev->to().next().column() << std::endl;
                return boost::optional<ProductionHeader>();
        }
        prev = it;
        ++it;
        // Checkpoint: The identifier is followed by a colon.

        TokenIterator behindMatchPatterns;
        Pattern leftContext, mainPattern, rightContext;

        if (!parse_header_patterns(it, end, behindMatchPatterns,
                                   leftContext, mainPattern, rightContext)
        ) {
                return boost::optional<ProductionHeader>();
        }
        it = behindMatchPatterns;

        if (it == end || it->type() != Token::TransformTo) {
                TokenIterator i = it - 1;
                std::cerr <<
                   "error: expected body (starting with '-->') for "
                   " production in line "
                   << i->from().next().row() << ", column "
                   << i->from().next().column() << ", but found "
                   << "'" << i->value() << "'" << std::endl;
                return boost::optional<ProductionHeader>();
        }
        // Checkpoint: We just passed "-->"

        ++it;
        behind = it;

        ProductionHeader ret;
        ret.setName (name);
        ret.setLeftContext(leftContext);
        ret.setRightContext(rightContext);
        ret.setPattern(mainPattern);
        return ret;
}



boost::optional<ProductionBody> parse_production_body (
        // foo --> bar  x:
        TokenIterator it, TokenIterator end, TokenIterator &behind
) {
        TokenIterator behindPattern;
        const Pattern pat = parse_pattern(it, end, behindPattern);
        it = behindPattern;
        if (it == end || it->type() != Token::Semicolon) {
                TokenIterator prev = it - 1;
                std::cerr << "error: expected ';' after production in line "
                        << prev->to().row() << ", column "
                        << prev->to().column() << std::endl;
                return boost::optional<ProductionBody>();
        }
        ++it;
        behind = it;

        ProductionBody ret;
        ret.setPattern (pat);
        return ret;
}



boost::optional<Production> parse_production (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        if (it==end) {
                std::cerr << "found nothing" << std::endl;
                return boost::optional<Production>();
        }


        boost::optional<ProductionHeader> header;
        boost::optional<ProductionBody> body;

        //---------------------------------------------------------
        // Header
        TokenIterator behindHeader;
        header = parse_production_header(it, end, behindHeader);
        if (!header) {
                return boost::optional<Production>();
        }
        it = behindHeader;

        if (it == end) {
                TokenIterator i = it - 1;
                std::cerr <<
                   "error: expected body behind '-->' in line "
                   << i->to().next().row() << ", column "
                   << i->to().next().column() << std::endl;
                return boost::optional<Production>();
        }
        //---------------------------------------------------------



        //---------------------------------------------------------
        // Body
        TokenIterator behindBody;
        body = parse_production_body(it, end, behindBody);
        if (!body) {
                return boost::optional<Production>();
        }
        it = behindBody;
        //---------------------------------------------------------



        Production ret;


        //---------------------------------------------------------
        // Compile symbol table.
        {
                int index = 0;
                std::map<std::string, int> symtab;
                Pattern headerPatterns[3] = {
                        header->leftContext(),
                        header->pattern(),
                        header->rightContext()
                };
                // At this point it is ensured that all parameters are
                // identifiers. parse_headers() did the check for us.
                for (unsigned int i=0; i<3; ++i) {
                        Pattern &pat = headerPatterns[i];
                        for (unsigned int p=0; p<pat.size(); ++p) {
                                ParameterList params = pat[p].parameterList();
                                for (unsigned int a=0; a<params.size(); ++a) {
                                        Parameter &param = params[a];
                                        std::string const &id = param.identifier();
                                        if (!symtab.count(id)) {
                                                symtab[id] = index++;
                                        }
                                        param.toParameterIndex(symtab[id]);
                                }
                                pat[p].setParameterList(params);
                        }
                }
                ProductionHeader ph(*header);
                ph.setLeftContext (headerPatterns[0]);
                ph.setPattern(headerPatterns[1]);
                ph.setRightContext (headerPatterns[2]);
                ret.setHeader (ph);

                // Apply symbol table to body.
                Pattern bodyPattern = body->pattern();
                for (unsigned int p=0; p<bodyPattern.size(); ++p) {
                        ParameterList params = bodyPattern[p].parameterList();
                        for (unsigned int a=0; a<params.size(); ++a) {
                                Parameter &param = params[a];
                                if (param.type() != Parameter::Identifier) {
                                        continue;
                                } else {
                                }
                                std::string const &id = param.identifier();
                                if (!symtab.count(id)) {
                                        std::cout << "error:" << std::endl;
                                        return boost::optional<Production>();
                                }
                                param.toParameterIndex(symtab[id]);
                        }
                        bodyPattern[p].setParameterList(params);
                }
                ProductionBody pb = *body;
                pb.setPattern(bodyPattern);
                ret.setBody (pb);
        }
        //---------------------------------------------------------



        behind = it;
        return ret;
}



void compile (const char *code, const char *axiom_) {
        const TokenVector tokens = tokenize (code);

        std::vector<Production> prods;

        std::set<std::string> names;
        TokenIterator it = tokens.begin();
        const TokenIterator &end = tokens.end();
        //char c = 'a';
        while (it != end) {
                TokenIterator behind;
                if (boost::optional<Production> op = parse_production (it, end, behind)) {
                        if (names.count(op->header().name())) {
                                std::cerr << "error: multiple productions are "
                                 << "named '" << op->header().name() << "'.\n";
                                return;
                        }
                        names.insert(op->header().name());
                        prods.push_back(*op);
                } else {
                        return;
                }
                it = behind;
        }

        std::stable_sort (prods.begin(), prods.end(), hasPrecedenceOver);
        for (unsigned int i=0; i<prods.size(); ++i) {
                std::cout << prods[i] << '\n';
        }

        std::map<std::string, Symbol> first_appearance;
        for (unsigned int i=0; i<prods.size(); ++i) {

                // Check if there are ambiguous rules which could lead
                // to a situation like:
                //  axiom: a   b   a
                //      A: a < b
                //      B:     b < a
                if (i<prods.size()-1
                   && ambiguous (prods[i], prods[i+1])
                ) {
                        std::cerr << "warning: productions '"
                          << prods[i].header().name()
                          << "' and '"
                          << prods[i+1].header().name()
                          << "' might be ambiguous (neither is more "
                          << "specialized); in such case, '"
                          << prods[i].header().name()
                          << "' will be preferred (because it was "
                          << "declared first).\n";
                        ;
                }

                // ABoP seems to allow for overloading, i.e. "b" != "b(x)".
                // I guess that can cause confusion, thus we'll try to warn
                // about overloads.
                const Pattern pats[4] = {
                        prods[i].header().leftContext(),
                        prods[i].header().pattern(),
                        prods[i].header().rightContext(),
                        prods[i].body().pattern()
                };
                for (unsigned int p=0; p<4; ++p) {
                        const Pattern &pat = pats[p];
                        for (unsigned int i=0; i<pat.size(); ++i) {
                                if (!first_appearance.count(pat[i].name())) {
                                        first_appearance[pat[i].name()] = pat[i];
                                } else if (pat[i] != first_appearance[pat[i].name()]) {
                                        Symbol const & sym =
                                               first_appearance[pat[i].name()];
                                        std::cerr
                                          << "warning: letter '"
                                          << pat[i].name() << "' used with"
                                          << " differing parameter-counts, "
                                          << "'" << pat[i] << "' won't match "
                                          << "'" << sym << "'" << std::endl;
                                }
                        }
                }
        }


        const TokenVector axiom = tokenize(axiom_);
        /*for (unsigned int i=0; i<axiom.size(); ++i) {
                std::cout << axiom[i];
        }*/
        std::cout << "\n--------------\n";
        TokenIterator behind;
        boost::optional<Pattern> ax = parse_axiom(axiom.begin(), axiom.end());

        if (!ax) {
                std::cout << "axiom '" << axiom_ << "' is not valid." << std::endl;
        } else {
                std::cout << "axiom: " << *ax << '\n';

                Pattern pat = *ax;
                for (int step=0; step<6; ++step) {
                        boost::optional<Pattern> apply(std::vector<Production> const &, Pattern const &);

                        boost::optional<Pattern> next = apply (prods, pat);
                        if (next) {
                                pat = *next;
                                std::cout << "step " << step << ": " << pat << '\n';
                        } else {
                                std::cout << "no match in step " << step << '\n';
                                break;
                        }
                }
        }
}







unsigned int matchLength (Pattern const &pattern,
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

unsigned int matchLength (Production const &production,
                 Pattern const &axiom,
                 int axiomIndex
) {
        const ProductionHeader &header = production.header();
        const int mainLen = matchLength (header.pattern(), axiom, axiomIndex);
        if (0 == mainLen)
                return 0;

        if (!header.leftContext().empty()) {
                Pattern const & ct = header.leftContext();
                if (!matchLength (ct, axiom, axiomIndex-ct.size()))
                        return 0;
        }
        if (!header.rightContext().empty()) {
                Pattern const & ct = header.rightContext();
                if (!matchLength (ct, axiom, axiomIndex+mainLen))
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
                Symbol const &sym = pattern[i];
                Symbol const &xsym = axiom[(i+axiomIndex)+axiomOffset];
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
        }
}


Symbol applyStack (Symbol const &symbol, std::vector<Parameter> const &stack) {
        Symbol ret = symbol;
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
        return ret;
}
/*Pattern applyStack (Pattern const &pattern, std::vector<Parameter> const &stack) {
        Pattern ret;
        for (unsigned int s=0; s<pattern.size(); ++s) {
                ret.push_back (applyStack (pattern[s], stack));
        }
        return ret;
}*/

boost::optional<Pattern> apply(std::vector<Production> const &prods, Pattern const &axiom) {
        std::vector<Parameter> stack(16);
        bool axiomWasTweaked = false;
        Pattern ret;
        for (unsigned int A=0; A<axiom.size(); ) {
                bool any = false;
                for (unsigned int P=0; P<prods.size(); ++P) {
                        const int len = matchLength(prods[P], axiom, A);
                        const bool doesMatch = len > 0;
                        if (doesMatch) {
                                any = true;

                                const Pattern &lcPattern = prods[P].header().leftContext();
                                const Pattern &rcPattern = prods[P].header().rightContext();
                                const Pattern &mPattern  = prods[P].header().pattern();
                                const Pattern &body = prods[P].body().pattern();

                                stack.clear();

                                // Fill stack with values from axiom.
                                // E.g., axiom = "a(x) < b(y) c(z) > d(a)"
                                fillStack (lcPattern, axiom, A, -lcPattern.size(), stack);
                                fillStack (mPattern,  axiom, A, 0, stack);
                                fillStack (rcPattern, axiom, A, mPattern.size(), stack);

                                for (unsigned int i=0; i<body.size(); ++i) {
                                        ret.push_back(applyStack(body[i], stack));
                                }

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

