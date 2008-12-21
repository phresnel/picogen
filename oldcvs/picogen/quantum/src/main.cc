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
#include <picogen/picogen.h>
#include <picogen/compile_info.h>
#include <picogen/errors.h>


#ifdef NDEBUG
#warning "note: assert() macros disabled"
#else
#warning "note: assert() macros enabled"
#endif


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
        << "\n"
        << " mkskymap :      mkskymap is a sky/cloud (w.i.p.) generation tool.\n"
        << "                 type 'picogen mkskymap --help' for further help.\n"
        << "\n"
        << " ssdf :          for parsing the (not too) static scene definition format.\n"
        << "                 type 'picogen ssdf --help' for further help.\n"
    << std::endl;
}



static void warranty() {
    ::std::cout << "The lazy author of this program has not yet written any warranty information. Kick his ass at seb@greenhybrid.net." << ::std::endl;
}



static void conditions() {
    ::std::cout << "The lazy author of this program has not yet written any conditions information. Kick his ass at seb@greenhybrid.net." << ::std::endl;
}



int main (int argc, char *argv[]) {
    extern picogen::error_codes::code_t main_mkheightmap (int argc, char *argv[]);
    extern int main_mkskymap (int argc, char *argv[]);
    extern int main_testscenes (int argc, char *argv[]);
    extern picogen::error_codes::code_t main_ssdf (int argc, char *argv[]);

    using namespace std;

    std::cout << "picogen "
              << XRT_VERSION_MAJOR << "."
              << XRT_VERSION_MINOR << "."
              << XRT_VERSION_PATCH;
#ifdef XRT_DEBUG
    std::cout << " debug-build";
#else
#endif
    std::cout << std::endl;

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
    
    picogen::error_codes::code_t err = picogen::error_codes::generic_okay;

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
    }

    else if (primary == string ("ssdf")) {
        err = main_ssdf (argc, argv);
    }

    else if (primary == string ("mkheightmap")) {
        argc++; // <-- quick and dirty patch
        argv--;
        err = main_mkheightmap (argc, argv);
    }

    else if (primary == string ("mkskymap")) {
        argc++; // <-- quick and dirty patch
        argv--;
        return main_mkskymap (argc, argv);
    }

    /*
    else if (primary == string ("testscene")) {
        return main_testscenes (argc, argv);
    }
    */

    else {
        usage();
    }
    
    if (picogen::error_codes::generic_okay != err) {
        cerr << "error:err" << static_cast <int> (err) << endl;
        return 1;
    }
    return 0;
}
