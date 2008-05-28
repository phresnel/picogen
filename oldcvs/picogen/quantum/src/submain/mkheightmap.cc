/***************************************************************************
 *            mkheightmap.cc
 *
 *  Copyright  2008  Sebastian Mach
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
#include <picogen.h>

std::string inlispEditor() {
    using namespace std;
    char s[1024];
    fgets( s, sizeof(s), stdin );
    return std::string( s );
}

int main_mkheightmap( int argc, char *argv[] ) {
    using namespace std;
    cout << "Type in some formula in inlisp-syntax: " << endl;

    const string code = inlispEditor();
    cout << "you have written:\n   " << code << "\n";

    using namespace picogen::misc::functional;
    try {
        Function_R2_R1 fun( code );
    } catch ( const functional_general_exeption &e ) {
        cerr << "there was some error in your function:\n   " << e.getMessage() << endl;
    }

    return 0;
}
