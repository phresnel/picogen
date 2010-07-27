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

namespace {

Pattern parse_pattern (TokenIterator it, TokenIterator end,
                       TokenIterator &behind, bool isHeader=false);



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



boost::optional<Segment> parse_segment (
        TokenIterator it, TokenIterator end, TokenIterator &behind,
        bool isHeader=false
) {
        if (it == end)
                return boost::optional<Segment>();

        if (it->type() == Token::Identifier) {
                Segment sym;
                sym.setType (Segment::Letter);
                sym.setName (it->value());
                ++it;

                TokenIterator behindParams;
                boost::optional<ParameterList> params =
                        parse_parameter_list(it, end, behindParams, isHeader);
                if (!params)
                        return boost::optional<Segment>();
                sym.setParameterList (*params);
                it = behindParams;
                behind = it;
                return sym;
        } else if (it->type() == Token::LeftBracket) {
                const TokenIterator start = it;

                ++it;
                Segment sym;
                sym.setType(Segment::Branch);
                sym.setName("[");
                TokenIterator behindBranch;
                sym.setBranch(parse_pattern(it, end, behindBranch, isHeader));
                it = behindBranch;
                if (it == end || it->type() != Token::RightBracket) {
                        std::cerr << "missing ']' after branch started at "
                           << "line " << start->to().row() << ", column "
                           << start->to().column() << std::endl;
                        return boost::optional<Segment>();
                }

                behind = ++it;
                return sym;
        } else {
                return boost::optional<Segment>();
        }
}



Pattern parse_pattern (TokenIterator it, TokenIterator end,
                       TokenIterator &behind, bool isHeader) {
        Pattern ret;
        while (boost::optional<Segment> sym = parse_segment(it, end,
                                                            behind, isHeader)
        ) {
                it = behind;
                ret.push_back(*sym);
        }
        behind = it;
        return ret;
}



bool contains_unknowns (Pattern const &pat) {
        for (unsigned int i=0; i<pat.size(); ++i) {
                Segment const & sym = pat[i];

                switch (sym.type()) {
                case Segment::Letter: {
                        ParameterList const &pm = sym.parameterList();

                        for (unsigned int p=0; p<pm.size(); ++p) {
                                if (pm[p].type() != Parameter::Integer
                                 && pm[p].type() != Parameter::Real
                                ) {
                                        return true;
                                }
                        }
                        break;
                }
                case Segment::Branch:
                        return contains_unknowns(sym.branch());
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



void compile_symbol_table (
        const Pattern& pattern,
        std::map<std::string, int> &symtab
) {
        for (unsigned int p=0; p<pattern.size(); ++p) {
                switch (pattern[p].type()) {
                case Segment::Letter: {
                        ParameterList params = pattern[p].parameterList();
                        for (unsigned int a=0; a<params.size(); ++a) {
                                Parameter &param = params[a];
                                std::string const &id = param
                                                        .identifier();
                                if (!symtab.count(id)) {
                                        const int id_ = symtab.size();
                                        symtab[id] = id_;
                                }
                        }
                } break;
                case Segment::Branch:
                        compile_symbol_table(pattern[p].branch(), symtab);
                        break;
                }
        }
}



boost::optional<Pattern> apply_symbol_table (
        Pattern pat,
        std::map<std::string, int> const & symtab
) {
        for (unsigned int p=0; p<pat.size(); ++p) {
                switch (pat[p].type()) {
                case Segment::Letter: {
                        ParameterList params = pat[p].parameterList();
                        for (unsigned int a=0; a<params.size(); ++a) {
                                Parameter &param = params[a];
                                if (param.type() != Parameter::Identifier) {
                                        continue;
                                } else {
                                }
                                std::string const &id = param.identifier();
                                if (!symtab.count(id)) {
                                        std::cout << "error: symbol '" << id
                                                << "' unknown." << std::endl;
                                        return boost::optional<Pattern>();
                                }
                                param.toParameterIndex(
                                        symtab.find(id)->second);
                        }
                        pat[p].setParameterList(params);
                } break;
                case Segment::Branch: {
                        boost::optional<Pattern> sub = apply_symbol_table(
                                                             pat[p].branch(),
                                                             symtab);
                        if (!sub)
                                return boost::optional<Pattern>();
                        pat[p].setBranch (*sub);
                        break;
                } break;
                }
        }
        return pat;
}



boost::optional<Production> parse_production (TokenIterator it,
                                              TokenIterator end,
                                              TokenIterator &behind
) {
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
                // At this point it is ensured that all parameters are
                // identifiers. parse_headers() did the check for us.

                std::map<std::string, int> symtab;
                compile_symbol_table(header->leftContext(), symtab);
                compile_symbol_table(header->pattern(), symtab);
                compile_symbol_table(header->rightContext(), symtab);
                for (std::map<std::string, int>::const_iterator it = symtab.begin();
                        it != symtab.end(); ++it) {
                                std::cout << it->first << "::" << it->second << std::endl;
                        }

                const boost::optional<Pattern>
                     newL = apply_symbol_table(header->leftContext(), symtab),
                     newC = apply_symbol_table(header->pattern(), symtab),
                     newR = apply_symbol_table(header->rightContext(), symtab);
                if ((!newL && header->leftContext().size())
                  ||(!newC && header->pattern().size())
                  ||(!newR && header->rightContext().size())
                ) {
                        std::cout << "internal error: after apply_symbol_table"
                                "(), one of newL, newR, or newC is null"
                                << std::endl;
                        return boost::optional<Production>();
                }

                ProductionHeader ph(*header);
                ph.setLeftContext (*newL);
                ph.setPattern(*newC);
                ph.setRightContext (*newR);
                ret.setHeader (ph);

                const boost::optional<Pattern> newp = apply_symbol_table(
                                                            body->pattern(),
                                                            symtab);
                if (!newp)
                        return boost::optional<Production>();
                body->setPattern (*newp);
                ret.setBody (*body);
        }
        //---------------------------------------------------------



        behind = it;
        return ret;
}


void generate_warnings (Pattern const &pat,
                        std::map<std::string, Segment> &first_appearance
) {
        for (unsigned int i=0; i<pat.size(); ++i) {
                switch (pat[i].type()) {
                case Segment::Letter:
                        if (!first_appearance.count(pat[i].name())) {
                                first_appearance[pat[i].name()] = pat[i];
                        } else if (pat[i] != first_appearance[pat[i].name()]) {
                                Segment const & sym =
                                  first_appearance[pat[i].name()];
                                std::cerr
                                << "warning: letter '"
                                << pat[i].name() << "' used with"
                                << " differing parameter-counts, "
                                << "'" << pat[i] << "' won't match "
                                << "'" << sym << "'" << std::endl;
                        }
                        break;
                case Segment::Branch:
                        generate_warnings(pat[i].branch(), first_appearance);
                        break;
                }
        }
}


void generate_warnings (std::vector<Production> const &prods) {
        std::map<std::string, Segment> first_appearance;
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
                        generate_warnings(pat, first_appearance);
                }
        }
}

} // namespace {


void compile (const char *code, const char *axiom_) {
        const TokenVector tokens = tokenize (code);

        std::vector<Production> prods;

        std::set<std::string> names;
        TokenIterator it = tokens.begin();
        const TokenIterator &end = tokens.end();
        //char c = 'a';
        while (it != end) {
                TokenIterator behind;
                if (boost::optional<Production> op = parse_production (it, end,
                                                                       behind)
                ) {
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
        generate_warnings(prods);
        std::cout << prods;

        std::cout << "\n--------------\n";
        TokenIterator behind;
        const TokenVector axiom = tokenize(axiom_);
        boost::optional<Pattern> ax = parse_axiom(axiom.begin(), axiom.end());

        if (!ax) {
                std::cout << "axiom '" << axiom_ << "' is not valid."
                          << std::endl;
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
