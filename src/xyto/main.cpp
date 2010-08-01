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

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <boost/optional.hpp>
#include <stdexcept>

#include "xyto_ios.hh"
#include "tokenize.hh"
#include "parameter.hh"

void compile(const char*, const char*);


boost::optional<Parameter> parse_factor (TokenIterator it,
                   TokenIterator end,
                   TokenIterator &behind
) {
        Parameter ret;
        switch (it->type()) {
        case Token::Integer:
                ret.setType(Parameter::Integer);
                ret.setInteger(it->valueAsInteger());
                break;
        case Token::Real:
                ret.setType(Parameter::Real);
                ret.setReal(it->valueAsReal());
                break;
        case Token::Identifier:
                ret.setType(Parameter::Identifier);
                ret.setIdentifier(it->value());
                break;
        default:
                return false;
        };
        behind = ++it;
        return ret;
}


Parameter::Type tokenTypeToParameterType (Token::Type tok) {
        switch (tok) {
        case Token::Plus:  return Parameter::Addition;
        case Token::Minus: return Parameter::Subtraction;
        case Token::Asterisk: return Parameter::Multiplication;
        case Token::Slash: return Parameter::Division;
        case Token::LessThan: return Parameter::LessThan;
        case Token::LessEqual: return Parameter::LessEqual;
        case Token::GreaterThan: return Parameter::GreaterThan;
        case Token::GreaterEqual: return Parameter::GreaterEqual;
        default: throw std::runtime_error("unhandled token-type in "
                                "tokenTypeToParameterType (Token::Type tok)");
        };
}

template <
        typename AcceptedTokens,
        boost::optional<Parameter> descendant (TokenIterator it,
                   TokenIterator end,
                   TokenIterator &behind
        )
>
boost::optional<Parameter> parse_term_tpl (
        TokenIterator it,
        TokenIterator end,
        TokenIterator &behind
) {
        using boost::optional;
        /*
                a * b * c * d
                (* a (* b (* c d)))  // <-- the more intuitive way, but wrong
                                            for any left-associative operation
                (* (* (* a b) c) d)  // <-- right
        */

        if (it == end)
                return false;

        const optional<Parameter> first = descendant(it, end, behind);
        if (!first)
                return false;
        it = behind;

        Parameter ret = *first;
        Parameter prev = *first;

        while (it != end) {
                optional<Parameter> next;

                bool any = false;
                for (unsigned int i=0;
                     i<sizeof(AcceptedTokens::tokens)
                       / sizeof(AcceptedTokens::tokens[0]);
                      ++i
                ) {
                        if (it->type() == AcceptedTokens::tokens[i]) {
                                ret.setType(
                                        tokenTypeToParameterType(it->type()));
                                any = true;
                                break;
                        }
                }
                if (!any) {
                        break;
                }

                behind = ++it;
                next = descendant(it, end, behind);
                if (!next)
                        return false;
                it = behind;

                ret.setLhs(prev);
                ret.setRhs(*next);
                prev = ret;
        }

        return ret;
}


// It will be better with variadic templates.
struct AddTokens { static const Token::Type tokens []; };
const Token::Type AddTokens::tokens [] = { Token::Asterisk, Token::Slash };

struct MulTokens { static const Token::Type tokens []; };
const Token::Type MulTokens::tokens [] = { Token::Plus, Token::Minus };

struct RelTokens { static const Token::Type tokens []; };
const Token::Type RelTokens::tokens [] = { Token::LessThan,
                                           Token::LessEqual,
                                           Token::GreaterThan,
                                           Token::GreaterEqual
                                         };


boost::optional<Parameter> parse_term (TokenIterator it, TokenIterator end,
                                       TokenIterator &behind)
{
        return parse_term_tpl<AddTokens, parse_factor>(it, end, behind);
}

boost::optional<Parameter> parse_expr (TokenIterator it, TokenIterator end,
                                       TokenIterator &behind)
{
        return parse_term_tpl<MulTokens, parse_term>(it, end, behind);
}

boost::optional<Parameter> parse_rel(TokenIterator it, TokenIterator end,
                                     TokenIterator &behind)
{
        return parse_term_tpl<RelTokens, parse_expr>(it, end, behind);
}


bool parse_cond (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        std::cout << "(? ";
        if (!parse_expr(it, end, behind))
                return false;
        it = behind;
        while (it != end &&
               (  it->type() == Token::LessThan
               || it->type() == Token::GreaterThan
               )
        ) {
                behind = ++it;
                if (!parse_expr(it, end, behind))
                        return false;
                it = behind;
        }
        std::cout << ")";
        return true;
}



int main()
{
        /*if (1) {
                // f(x) < y(x)   should yield an error "parameter names may only appear once"
                const char * code =
                        //"foo: A B #up(25) #left(10) --> result;"
                        "foo: foo(x) --> foo(x*5);"
                ;
                compile(code, "foo(5)");
                return 1;
        }*/

        {
                const char *expr = "a*b*c+d >= 0 && 1";

                const TokenVector tokens = tokenize(expr);
                std::cout << expr << ": " << tokens << std::endl;
                std::cout << "------\n";
                TokenIterator behind;
                const boost::optional<Parameter> t = parse_rel(tokens.begin(),
                                                               tokens.end(),
                                                               behind);
                if (t) std::cout << "\n{\n" << *t << "\n}\n";
        }

        /*
        #up
        #UP
        /up
        {up}
        $up
        .up
        up
        */


        return 0;
}
