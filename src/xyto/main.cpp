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

void compile(const char*, const char*);

int main()
{
        // f(x) < y(x)   should yield an error "parameter names may only appear once"
        const char * code =
                //"foo: A B #up(25) #left(10) --> result;"
                "foo0: A --> [0.0002] a \n"
                "            [0.0001] b \n"
                "            [0] c \n"
                ";"
        ;
        compile(code, "A");

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
