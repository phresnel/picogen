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

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "optional.hh"
#include "meta.hh"
#include "parsing.hh"

namespace quatsch_preprocessor {
namespace {

// C++ std::isalpha() may depend on locale, hence our own
bool isAlpha(char c) {
        switch (c) {
        case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D':
        case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
                return true;
        }
        return false;
}

bool isAlphaNum (char c) {
        return isAlpha(c) || isdigit(c);
}

template <typename iterator>
void eatWhitespace (iterator &it, iterator end) {
        while (it != end && (*it==' ' || *it=='\n' || *it=='\t'))
                ++it;
}

template <typename iterator>
std::string parseId (iterator &it_, const iterator &end) {
        iterator it = it_;

        std::string ret;

        if (it == end || *it != '$')
                return "";
        ret += *it;
        ++it;
        while (it != end) {
                if (!isAlphaNum(*it))
                        break;
                ret += *it;
                ++it;
        }
        if (ret != "") {
                it_ = it;
        }
        return ret;
}

template <typename iterator>
optional<DomainScalar> parseScalar (iterator &it_, const iterator &end) {
        iterator it = it_;

        if (it == end || !(isdigit (*it) || *it=='-'))
                return false;

        const iterator value_begin (it);
        if (*it == '-') {
                ++it;
        }
        while (it != end && isdigit (*it)) {
                ++it;
        }

        if (it != end && *it == '.') {
                if (it+1 != end && isdigit(*(it+1))) {
                        ++it;
                        while (it != end && isdigit (*it)) {
                                ++it;
                        }
                }
        }
        const iterator value_end (it);

        double val;
        std::stringstream ss;
        ss << std::string (value_begin, value_end);
        ss >> val;

        it_ = it;
        return DomainScalar(val); // TODO: <<
}
template <typename iterator>
optional<DomainScalar> parseScalarValue (iterator &it_, const iterator &end) {
        return parseScalar (it_, end);
}


template <typename iterator>
optional<DomainInterval> parseInterval (iterator &it_, const iterator &end) {
        iterator it = it_;

        if (it == end || *it != '[')
                return false;
        ++it;

        eatWhitespace (it, end);

        const optional<DomainScalar> from = parseScalar (it, end);
        if (!from)
                return false;
        eatWhitespace (it, end);

        if (it == end || it+1 == end ||
            *it != '.' || *(it+1) != '.')
        {
                return false;
        }
        it += 2;

        eatWhitespace (it, end);
        const optional<DomainScalar> to = parseScalar (it, end);
        if (!to)
                return false;
        eatWhitespace (it, end);

        if (it == end || *it != ']')
                return false;
        ++it;


        it_ = it;
        return DomainInterval(*from, *to); // TODO: <<
}
template <typename iterator>
optional<DomainInterval> parseIntervalValue (iterator &it_, const iterator &end) {
        return parseInterval (it_, end);
}


template <typename iterator>
optional<DomainValue> parseDomainValue (iterator &it_, const iterator &end) {
        iterator it = it_;

        if (it == end)
                return false;

        if (optional<DomainScalar> d = parseScalarValue (it, end)) {
                it_ = it;
                return DomainValue(*d);
        }
        if (optional<DomainInterval> d = parseIntervalValue (it, end)) {
                it_ = it;
                return DomainValue(*d);
        }

        return false;
}

template <typename iterator>
optional<Domain> parseDomain (iterator &it_, const iterator &end) {
        iterator it = it_;

        Domain ret;

        if (it == end || *it != '{')
                return false;
        ++it;
        eatWhitespace (it, end);

        bool first = true;
        while (it != end && *it != '}') {

                if (!first) {
                        if (it == end || *it != ',') {
                                return false;
                        }
                        ++it;
                        eatWhitespace (it, end);
                }
                optional<DomainValue> value = parseDomainValue (it, end);
                if (!value)
                        return false;
                ret.push_back (*value);
                eatWhitespace (it, end);

                first = false;
        }
        if (it == end || *it != '}') {
                return false;
        }
        ++it;

        it_ = it;
        return ret;
}


template <typename iterator>
optional<DomainEnumeration> parseEnumeration (iterator &it_, const iterator &end) {
        iterator it = it_;

        DomainEnumeration ret;

        if (it == end || *it != '{')
                return false;
        ++it;
        eatWhitespace (it, end);

        bool first = true;
        while (it != end && *it != '}') {

                if (!first) {
                        if (it == end || *it != ',') {
                                return false;
                        }
                        ++it;
                        eatWhitespace (it, end);
                }
                optional<std::string> value = parseString(it, end);
                if (!value)
                        return false;
                ret.push_back (*value);
                eatWhitespace (it, end);

                first = false;
        }
        if (it == end || *it != '}') {
                return false;
        }
        ++it;

        it_ = it;
        return ret;
}



template <typename iterator>
bool parseString (iterator &it_, const iterator &end,
                   const std::string &literal
) {
        iterator it = it_;

        iterator lit=literal.begin(),
                 lend=literal.end();
        while (lit!=lend && it!=end) {
                if (*it != *lit)
                        return false;
                ++it;
                ++lit;
        }
        if (lit != lend)
                return false;

        it_ = it;
        return true;
}


template <typename iterator>
optional<std::string> parseString (iterator &it_, const iterator &end) {
        iterator it = it_;

        std::string ret;

        if (it == end || !isAlpha(*it))
                return optional<std::string>();
        ret += *it;
        ++it;
        while (it != end) {
                if (!isAlphaNum(*it))
                        break;
                ret += *it;
                ++it;
        }
        if (ret != "") {
                it_ = it;
        }
        return ret;
}

template <typename iterator>
optional<DeclaredType> parseType (iterator &it, const iterator &end) {
        if (parseString (it, end, "integer"))
                return Integer;
        if (parseString (it, end, "real"))
                return Real;
        if (parseString (it, end, "boolean"))
                return Boolean;
        if (parseString (it, end, "filename"))
                return Filename;
        if (parseString (it, end, "enumeration"))
                return EnumerationValue;
        return false;
}

optional<std::string> parsePlaceholder (std::string statement) {
        typedef std::string::const_iterator iterator;

        iterator it = statement.begin();
        const iterator end = statement.end();

        eatWhitespace (it, end);
        const std::string id = parseId (it, end);
        eatWhitespace(it, end);

        // A simple placeholder is the only thing in the statement, so we must
        // be at the end by now.
        if (it != end || id == "")
                return false;
        return id;
}

optional<Declaration> parseDeclaration (std::string statement) {
        typedef std::string::const_iterator iterator;

        Declaration ret;

        iterator it = statement.begin();
        const iterator end = statement.end();

        eatWhitespace (it, end);
        const std::string id = parseId (it, end);
        if (it == end || id == "") {
                return false;
        }
        ret.setId (id);

        eatWhitespace(it, end);
        if (it == end || *it != ':')
                return false;
        ++it;

        eatWhitespace(it, end);
        const optional<DeclaredType> dt = parseType (it, end);
        if (!dt)
                return false;
        ret.setType (*dt);
        eatWhitespace(it, end);

        if (it == end) {
                return ret;
        }

        if (*it == '=') {
                ++it;
                eatWhitespace(it, end);


                optional<Domain> domain;
                if (EnumerationValue == *dt) {
                        const optional<DomainEnumeration> e = parseEnumeration(it, end);
                        if (!e) return false;
                        Domain dom;
                        dom.push_back(*e);
                        domain = dom;
                } else {
                        domain = parseDomain (it, end);
                }
                if (domain && domainMustBeImplicit(ret.type())) {
                        return false;
                }
                if (domain)
                        ret.setDomain (*domain);
        }

        eatWhitespace(it, end);
        if (parseString(it, end, "default")) {
                eatWhitespace(it, end);
                if ('=' != *it)
                        return false;
                ++it;
                eatWhitespace(it, end);

                const optional<DomainValue> val = parseDomainValue(it, end);
                if (!val || val->elementCount()>1)
                        return false;
                ret.setDefaultValue(*val);
        }

        return ret;
}

template <typename iterator>
optional<tuple<iterator, iterator> >
findNextStatement (iterator it, iterator const &end) {
        while (it != end) {
                if (*it == '('
                    && (it+1!=end) && *(it+1) == '(')
                {
                        const iterator stmt_begin = it;
                        bool valid = false;
                        it += 2;

                        std::string statement = "";
                        for (; it != end; ) {
                                if (*it == ')'
                                    && (it+1!=end) && *(it+1) == ')')
                                {
                                        it+=2;
                                        valid = true;
                                        break;
                                }
                                statement += *it;
                                ++it;
                        }

                        if (valid) {
                                const iterator stmt_end = it;
                                return make_tuple (stmt_begin, stmt_end);
                        }
                } else {
                        ++it;
                }
        }
        return false;
}

std::string actualStatement (tuple<std::string::const_iterator,
                                   std::string::const_iterator> stmt)
{
        return std::string(get<0>(stmt)+2,
                           get<1>(stmt)-2);
}

template<typename iterator>
iterator behindStatement (tuple<iterator,iterator> stmt) {
        return get<1>(stmt);
}

template<typename iterator>
iterator beforeStatement (tuple<iterator,iterator> stmt) {
        return get<0>(stmt);
}

} // namespace {
} // namespace quatsch_preprocessor {


namespace quatsch_preprocessor {

Declarations findDeclarations (std::string const &code) {
        typedef std::string::const_iterator iterator;

        Declarations decls;

        iterator it = code.begin();
        const iterator end = code.end();
        while (optional<tuple<iterator,iterator> >
                stmt_ = findNextStatement (it, end))
        {
                const std::string stmt (actualStatement(*stmt_));
                it = behindStatement(*stmt_);

                if (const optional<Declaration> decl = parseDeclaration (stmt)) {
                        decls.push_back (*decl);
                }
        }

        return decls;
}


std::string replace(
        std::string const &code,
        std::map<std::string,std::string> const &rep
) {
        typedef std::string::const_iterator iterator;

        std::string ret;
        iterator it = code.begin();
        const iterator end = code.end();

        while (optional<tuple<iterator,iterator> >
                stmt_ = findNextStatement (it, end))
        {
                const iterator before = beforeStatement(*stmt_),
                               behind = behindStatement(*stmt_);
                const std::string stmt (actualStatement(*stmt_));

                // Add part before preprocessor-statement.
                ret += std::string (it, before);

                // Add replaced preprocessor-statement.
                if (const optional<std::string>
                        placeholder = parsePlaceholder(stmt))
                {
                        if (rep.count(*placeholder) != 0)
                                ret += rep.find(*placeholder)->second;
                }


                it = behind;
        }
        ret += std::string (it, end);
        return ret;
}

}
