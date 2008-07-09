/*
 *  nano-c.cc
 *  (C) 2008 Sebastian Mach
 *  seb@greenhybrid.net
 *
 *
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

#include <picogen/picogen.h>
#include <picogen/experimental/PicoPico.h>
#include <picogen/misc/scripting/nano-c.h>
#include <picogen/misc/scripting/AST.h>

using namespace picogen;
using namespace std;

namespace picogen {

    NanoC::NanoC (const std::string &code) : code(code), it(code.begin()) {
        const ExprAST *ast = 0;
        PicoPico p(code, PicoPico::return_ast, &ast);
        if (0 != ast) {
            /*cout << "\nprogram's AST:{{\n";
            ast->print (1);
            cout << "\n}}" << endl;*/
            delete ast;
        }
    }



    bool NanoC::next (std::string &out) {
        /*using namespace std;
        for (; it!=code.end(); ++it) {
            cout << *it << "|";
        }*/
        return false;
    }

}; // namespace picogen
