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

#include "meta.hh"
#include "parsing.hh"
#include "ios.hh"

#include <vector>
#include <tuple>

#include <iostream>

int main () {
        std::string code = 
        //"((frob = {"Disabled"=0 10 [15..20],0 }))\n"
        "(($frob:real = {0.0, 10, [15.5  ..  20 ] , 7}))\n"
        "(+ foobar (*(($frob))0.5))";
        
        using quatsch_preprocessor::Declaration;
        std::vector<Declaration> 
                decls = quatsch_preprocessor::findDeclarations (code);
        std::map<std::string,std::string> reps;
        
        for (std::vector<Declaration>::const_iterator 
                it=decls.begin(), end=decls.end(); it!=end; ++it)
        {
                reps[it->id()] = "Hello World!";
        }
        
        std::cout << "--------------------------------------\n";
        std::cout << "original code:\n";
        std::cout << code << '\n';
        std::cout << "--------------------------------------\n";
        std::cout << "preprocessed code:\n";
        std::cout << quatsch_preprocessor::replace(code, reps) << '\n';
        std::cout << "--------------------------------------\n";
}
