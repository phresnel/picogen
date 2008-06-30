/***************************************************************************
 *            main.cc
 *
 *  Copyright  2007  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <iostream>


static void disclaimer() {
    using namespace std;
    cout
        << "---\n"
        << "\"picogen\"  Copyright (C) 2007,2008  Sebastian Mach (*1983)\n"
        << "This program comes with ABSOLUTELY NO WARRANTY." /* for details type `show w'.*/"\n"
        << "This is free software, and you are welcome to redistribute it\n"
        << "under certain conditions. " /*type `show c' for details.*/ "\n"
        << "---\n"
        << endl
        ;
}



static void usage() {
    std::cout
        << "Invocation: picogen [instruction [options for instruction]]\n"
        << "In the version of picogen you have installed, the following instructions are possible:\n"
        << "\n"
        << " mkheightmap :   mkheightmap is a heightmap generation tool.\n"
        << "                 type 'picogen mkheightmap --help' for further help.\n"
    << std::endl;
}



static void warranty() {
}



static void conditions() {
}



int main (int argc, char *argv[]) {
    extern int main_seb (int argc, char *argv[]);
    extern int main_mkheightmap (int argc, char *argv[]);
    extern int main_testscenes (int argc, char *argv[]);
    extern int main_ssdf (int argc, char *argv[]);

    using namespace std;
    disclaimer();

    argc--;
    argv++;

    if (argc == 0) {   // check if any argument is given, we need at least one
        usage();
        return -1;
    }

    const std::string primary = argv[0];
    argc--;
    argv++;

    if (primary == string ("show")) {
        if (argc == 0) {   // check if any argument is given, we need at least one remaining
            usage();
            return -1;
        }
        const std::string secondary = argv[0];
        if (secondary == string ("w")) {
            warranty();
            return 0;
        } else if (secondary == string ("c")) {
            conditions();
            return 0;
        }
    } else if (primary == string ("mkheightmap")) {
        return main_mkheightmap (argc, argv);
    }


    else if (primary == string ("testscene")) {
        return main_testscenes (argc, argv);
    }

    #ifndef PICOGEN_RELEASE
    else if (primary == string ("ssdf")) {
        return main_ssdf (argc, argv);
    }

    else if (primary == string ("seb")) {
        return main_seb (argc, argv);
    }
    #endif

    else {
        usage();
    }

    return 0;
}
