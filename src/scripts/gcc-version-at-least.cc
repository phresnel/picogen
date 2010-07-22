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

// Small helper to either print the compilers version number.
// Can also be called like "foo 4 5", and it would return FAIL
// if the current compilers version is smaller.
#include <iostream>
#include <sstream>
int main (int argc, char *argv[]) {
        if (argc <= 1) {
                std::cout << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
                return 0;
        } else if (argc == 3) {
                int major, minor;
                {std::stringstream ss;
                 ss << argv[1];
                 ss >> major;}
                {std::stringstream ss;
                 ss << argv[2];
                 ss >> minor;}

                if (major < __GNUC__ || (major==__GNUC__ && minor<=__GNUC_MINOR__))
                        return 0;
                return 1;
        }
        return 1;
}