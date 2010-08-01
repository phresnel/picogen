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

#include "xyto_ios.hh"
#include "tokenize.hh"

void compile(const char*, const char*);


bool parse_factor (TokenIterator it,
                   TokenIterator end,
                   TokenIterator &behind
) {
        switch (it->type()) {
        case Token::Integer:
                std::cout << "0 ";
                break;
        case Token::Real:
                std::cout << "0.0 ";
                break;
        case Token::Identifier:
                std::cout << "id ";
                break;
        default:
                return false;
        };
        behind = ++it;
        return true;
}

bool parse_term (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        std::cout << "(*/ ";
        if (!parse_factor(it, end, behind))
                return false;
        it = behind;
        while (it != end &&
               (it->type() == Token::Asterisk || it->type() == Token::Slash)
        ) {
                behind = ++it;
                if (!parse_factor(it, end, behind))
                        return false;
                it = behind;
        }
        std::cout << ")";
        return true;
}

bool parse_expr (TokenIterator it, TokenIterator end, TokenIterator &behind) {
        std::cout << "(+- ";
        if (!parse_term(it, end, behind))
                return false;
        it = behind;
        while (it != end &&
               (it->type() == Token::Plus || it->type() == Token::Minus)
        ) {
                behind = ++it;
                if (!parse_term(it, end, behind))
                        return false;
                it = behind;
        }
        std::cout << ")";
        return true;
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


/*template <typename T> struct soft_value {
public:

        soft_value& operator= (soft_value rhs) {
                swap (rhs);
        }
        void swap (soft_value &rhs) {
                swap (ptr, rhs.ptr);
        }

private:
        boost::scoped_ptr<T> ptr;
};
template <typename T>
void swap (soft_value<T> &lhs, soft_value<T> &rhs) {
        lhs.swap (rhs);
}*/

int main()
{
        if (0) {
                // f(x) < y(x)   should yield an error "parameter names may only appear once"
                const char * code =
                        //"foo: A B #up(25) #left(10) --> result;"
                        "foo: foo(x) --> foo(x*2);"
                ;
                compile(code, "foo(5)");
        }

        {
                const char *expr = "x*2 + 5*1 < 0";
                TokenVector tokens = tokenize(expr);
                std::cout << expr << ": " << tokens << std::endl;
                std::cout << "------\n";
                TokenIterator behind;
                parse_expr(tokens.begin(), tokens.end(), behind);
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
