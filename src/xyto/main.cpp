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
#include <boost/optional.hpp>

#include "xyto_ios.hh"
#include "tokenize.hh"
#include "parse_expr.hh"

void compile(const char*, const char*);


int main()
{
        if (1) {
                const char * code =
                        //"foo: A B #up(25) #left(10) --> result;"
                        "foo: foo(x,y) --> foo(2.5*x,x*y);"
                ;
                compile(code, "foo(5,2)");
                return 1;
        }

        if (0) {
                const char *expr = "1<x";

                const TokenVector tokens = tokenize(expr);
                std::cout << expr << ": " << tokens << std::endl;
                std::cout << "------\n";
                TokenIterator behind;
                const boost::optional<Parameter> t = parse_logical(tokens.begin(),
                                                               tokens.end(),
                                                               behind);
                if (t) std::cout << "\n{\n" << *t << "\n}\n";
                else std::cout << "<invalid>\n";
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
