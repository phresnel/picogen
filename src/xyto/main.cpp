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
#include "lsystem.hh"
#include <boost/optional.hpp>

boost::optional<LSystem> compile(const char*);


int main()
{
        if (1) {
                const char * code =
                        //"foo: A B #up(25) #left(10) --> result;"
                        "theta=12.5;\n"
                        "axiom: f;\n"
                        "f: f(theta) --> f(theta);"
                        /*"f0:   foo  --> foo bar;  \n"
                        "f1:   bar  --> frob [;  \n"
                        "f2:   frob --> [1] foo [left foo] \n"
                        "               [1] foo [right foo] \n"
                        ";  \n"*/
                        //"f0: foo --> [foo];"
                ;
                boost::optional<LSystem> lsys = compile(code);

                if (lsys) {
                        std::cout << "------\n";
                        std::cout << *lsys;
                        std::cout << "------\n";
                        for (int i=0; i<5; ++i) {
                                std::cout << "[" << i << "] " << lsys->run(i) << std::endl;
                        }
                }
                return 0;
        }

        if (0) {
                const char *expr = "-x";

                const TokenVector tokens = tokenize(expr);
                std::cout << expr << ": " << tokens << std::endl;
                std::cout << "------\n";
                TokenIterator behind;
                const boost::optional<Parameter> t = parse_expr(tokens.begin(),
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
