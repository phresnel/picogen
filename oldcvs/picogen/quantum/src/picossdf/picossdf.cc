/*
 *  picossdf.cc
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

#define PICOSSDF_CC_STANDALONE 0

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <sstream>

#include <picogen/picogen.h>
#include <picogen/picossdf/picossdf.h>

using namespace std;



PicoSSDF::PicoSSDF (const string &filename, SSDFBackend *backend) : filename (filename), backend (backend) {
    switch (parse()) {
        case OKAY:
            break;
        case FILE_NOT_FOUND:
            throw exception_file_not_found (filename);
        case SYNTAX_ERROR:
            throw exception_syntax_error (
                filename, errcurrentline, errreason, linenumber
            );
        default:
            throw exception_unknown (filename);
    }
}



PicoSSDF::parse_err PicoSSDF::push_block (BLOCK_TYPE type) {
    if (blockStack.size() >0) {
        // check if the new block is allowed within the current block
        if (!blockStack.back()->isBlockAllowed (type)) {
            errreason =
                string ("Block of type '") +blockTypeAsString (type) +
                string ("' forbidden within block of type '") +
                blockTypeAsString (blockStack.back()->type) +string ("'")
                ;
            return SYNTAX_ERROR;
        }
    } else {
        globalBlockCount++;
        if (globalBlockCount > 1) {
            errreason = string ("Only one block allowed at global scope.");
            return SYNTAX_ERROR;
        }
    }
    switch (type) {
        case BLOCK_GLOBAL: {
            blockStack.push_back (new GlobalBlock());
            backend->beginGlobalBlock();
        }
        break;
        case BLOCK_LIST: {
            blockStack.push_back (new ListBlock());
            backend->beginListBlock ();
        }
        break;
        case BLOCK_TRI_BIH: {
            blockStack.push_back (new TriBIHBlock());
            backend->beginTriBIHBlock ();
        }
        break;
    };

    if (0) {
        // pretty print log
        for (unsigned int u=0; u<blockStack.size(); ++u) printf ("  ");
        printf ("new block\n");
    }

    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::pop_block() {
    if (blockStack.size() <= 0) {
        errreason = string ("invalid closing bracket '}'");
        return SYNTAX_ERROR;
    }

    Block *block = blockStack.back();
    switch (block->type) {
        case BLOCK_GLOBAL: {
            backend->endGlobalBlock();
        }
        break;
        case BLOCK_LIST: {
            backend->endListBlock();
        }
        break;
        case BLOCK_TRI_BIH: {
            backend->endTriBIHBlock();
        }
        break;
    }
    delete block;

    // pretty print log
    if (0) {
        for (unsigned int u=0; u<blockStack.size(); ++u) printf ("  ");
        printf ("compiling block\n");
    }

    blockStack.pop_back();
    return OKAY;
}



static const picogen::common::Vector3d scanVector3d (const std::string &str, unsigned int &index) {
    using namespace std;
    using picogen::misc::prim::real;
    using picogen::misc::geometrics::Vector3d;
    index = 0;
    string tmp;
    real ret [3];
    for (string::const_iterator it=str.begin(); it != str.end(); it++) {
        if (*it != ',') {
            tmp += *it;
        } else {
            if (index <= 2) {
                stringstream ss;
                ss << tmp;
                ss >> ret [index];
            }
            tmp = "";
            ++index;
        }
    }
    if (index <= 2) {
        stringstream ss;
        ss << tmp;
        ss >> ret [index];
    }
    return Vector3d (ret [0], ret [1], ret [2]);
}



PicoSSDF::parse_err PicoSSDF::read_terminal (TERMINAL_TYPE type, char *&line) {
    using namespace ::std;
    using ::picogen::misc::prim::real;
    using ::picogen::misc::geometrics::Vector3d;
    using ::picogen::graphics::image::color::Color;

    // Step 1: Read Parameters.
    map<string,string> parameters;
    string name;
    string value;
    bool scanName = true;
    ++line; // Eat '('.
    while (true) {
        if (*line == ':') {
            if (!scanName) {
                errreason = string ("Expected either ')' or ';'.");
                return SYNTAX_ERROR;
            }
            scanName = false;
            ++line;
            continue;
        } else if (*line == ';' || *line == ')') {
            scanName = true;
            if (parameters.end() != parameters.find (name)) {
                errreason = string ("Parameter '") + name + string ("' has been set multiple times.");
                return SYNTAX_ERROR;
            }
            parameters [name] = value;
            name = value = "";
            if (*line == ')') {
                break;
            }
            ++line;
            continue;
        } else if (*line == '\0') {
            errreason = string ("Missing ')'.");
            return SYNTAX_ERROR;
        } else if (*line == ' ' || *line == '\t') { // always (ALWAYS) eat up whitespace
            line++;
            continue;
        }

        switch (scanName) {
            case true:  name  += *line; break;
            case false: value += *line; break;
        }
        ++line;
    }



    // Step 2: Validate and Convert Parameters to specific Terminal-Type.
    switch (type) {
        // Step 2: sphere (radius:<float>; center:<float>,<float>,<float>; color-rgb:<float>,<float>,<float>)
        case TERMINAL_SPHERE: {
            real radius = 1.0;
            Vector3d center (0.0, 0.0, 0.0);
            Color color (1.0, 1.0, 1.0);
            while( !parameters.empty() ) {
                if ((*parameters.begin()).first == string ("radius")) {
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> radius;
                } else if ((*parameters.begin()).first == string ("center")) {
                    unsigned int numScalars;
                    center = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'center'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'center'");
                        return SYNTAX_ERROR;
                    }
                } else if ((*parameters.begin()).first == string ("color-rgb")) {
                    unsigned int numScalars;
                    Vector3d col_vec = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'color-rgb'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'color-rgb'");
                        return SYNTAX_ERROR;
                    }
                    color.from_rgb (col_vec [0], col_vec [1], col_vec [2] );
                } else {
                    errreason = string ("unknown parameter to sphere: '") + string ((*parameters.begin()).first) + string ("'");
                    return SYNTAX_ERROR;
                }
                parameters.erase (parameters.begin());
            }
            backend->addSphereTerminal (radius, center, color);
            break;
        }
    }

    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::read_state (STATE_TYPE stateType, char *&line) {
    using namespace ::std;
    using ::picogen::misc::prim::real;
    using ::picogen::misc::geometrics::Vector3d;
    using ::picogen::graphics::image::color::Color;

    string type;
    ++line; // Eat '='
    while (*line != '\0' && (*line == ' ' || *line == '\t')) // Eat whitespace.
        ++line;

    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '(') {
        type += *line;
        ++line;
    }


    // TODO: Below, the 'Read Parameters', is redundant. Better write a function "read-parameters" and stuff
    // Read Parameters.
    map<string,string> parameters;
    string name;
    string value;
    bool scanName = true;
    if (*line == '(') { // Parameters are optional.
        ++line; // Eat '('.
        while (true) {
            if (*line == ':') {
                if (!scanName) {
                    errreason = string ("Expected either ')' or ';'.");
                    return SYNTAX_ERROR;
                }
                scanName = false;
                ++line;
                continue;
            } else if (*line == ';' || *line == ')') {
                scanName = true;
                if (parameters.end() != parameters.find (name)) {
                    errreason = string ("Parameter '") + name + string ("' has been set multiple times.");
                    return SYNTAX_ERROR;
                }
                parameters [name] = value;
                name = value = "";
                if (*line == ')') {
                    break;
                }
                ++line;
                continue;
            } else if (*line == '\0') {
                errreason = string ("Missing ')'.");
                return SYNTAX_ERROR;
            } else if (*line == ' ' || *line == '\t') { // always (ALWAYS) eat up whitespace
                line++;
                continue;
            }

            switch (scanName) {
                case true:  name  += *line; break;
                case false: value += *line; break;
            }
            ++line;
        }
    }

    // Set State.
    switch (stateType) {
        case STATE_MATERIAL:
            if ("lambertian" == type || "specular" == type) {
                real reflectance = 1.0;
                while( !parameters.empty() ) {
                    if ((*parameters.begin()).first == string ("reflectance")) {
                        stringstream ss;
                        ss << (*parameters.begin()).second;
                        ss >> reflectance;
                        cout << "<<" << reflectance << ">>" << endl;
                    } else {
                        errreason = string ("unknown parameter to ") + type + string ("-material: '") + string ((*parameters.begin()).first) + string ("'");
                        return SYNTAX_ERROR;
                    }
                    parameters.erase (parameters.begin ());
                }
                if ("lambertian" == type) {
                    backend->setBRDFToLambertian (reflectance);
                } else {
                    backend->setBRDFToSpecular (reflectance);
                }
            } else {
                errreason = string ("material '") + type + string ("' unknown");
                return SYNTAX_ERROR;
            }
            break;
    };

    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::interpretLine (char *line) {
    //return SYNTAX_ERROR;
    // skip whitespace
    while (isblank (*line)) ++line;

    // is this a comment?
    if (line[0] == '/' && line[1] == '/')
        return OKAY;

    // scan name of block
    char block_name[1024] = { '\0' };
    char *tmp = block_name;
    while (isidchar (*line) && tmp != &block_name[sizeof (block_name) ]) {
        *tmp = *line;
        ++tmp;
        ++line;
    }
    *tmp = '\0';
    // get length of block-name and see if it is anonymous
    const unsigned int block_name_len = strlen (block_name);
    const bool anonymous = ! (block_name_len > 0);

    //if( !anonymous ) printf( "\n<%s>\n", block_name );
    while (isblank (*line)) ++line;

    if ('{' == *line) {
        // get block typer out of block_name
        BLOCK_TYPE type;
        if (anonymous) {
            errreason = string ("anonymous blocks are not allowed");
            return SYNTAX_ERROR;
        } else if (!strcmp ("list", block_name)) {
            type = BLOCK_LIST;
        } else if (!strcmp ("tri-bih", block_name)) {
            type = BLOCK_TRI_BIH;
        } else {
            errreason = string ("unknown block type '") + string (block_name) + string ("'");
            return SYNTAX_ERROR;
        }
        // push
        parse_err err = push_block (type);
        if (OKAY != err) {
            return err;
        }
        line++;
    } else if ('}' == *line) {
        if (!anonymous) {
            errreason = string ("'}' is only allowed standalone");
            return SYNTAX_ERROR;
        }
        // pop
        parse_err err = pop_block();
        if (OKAY != err)
            return err;
        line++;
    } else if ('(' == *line) {
        TERMINAL_TYPE type;
        if (!strcmp ("sphere", block_name)) {
            type = TERMINAL_SPHERE;
        } else {
            errreason = string ("unknown terminal type '") + string (block_name) + string ("'");
            return SYNTAX_ERROR;
        }
        if (!blockStack.back()->isTerminalAllowed (type)) {
            errreason = string ("terminal type '") + terminalTypeAsString (type) + string ("' not allowed within block of type '")
                + blockTypeAsString (blockStack.back()->type) + string ("'");
            return SYNTAX_ERROR;
        }
        parse_err err = read_terminal (type, line);
        if (OKAY != err)
            return err;
        line++;
    } else if ('=' == *line) {
        STATE_TYPE type;
        if (!strcmp ("brdf", block_name)) {
            type = STATE_MATERIAL;
        } else {
            errreason = string ("unknown state type '") + string (block_name) + string ("'");
            return SYNTAX_ERROR;
        }
        parse_err err = read_state (type, line);
        if (OKAY != err)
            return err;
        line++;
    } else {
        // a) this is intended to be a damn fast text file format
        // b) i allow blank lines
        // c) but i do not allow other format freeness
        // ca) after a block declarator there must be [blank]*'{'
        //     i do not allow putting the '{' into the next line
        //     this is an error, and the lone '{' is then a SCOPE_BLOCK!
        if (!anonymous) {
            errreason = string ("block name must be followed by optional spaces and '{'");
            return SYNTAX_ERROR;
        }
    }
    while (isblank (*line)) ++line;
    if (*line != '\0') {
        errreason = string ("trailing, non whitespace character(s) forbidden ('") +string (line) +string ("')");
        return SYNTAX_ERROR;
    }
    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::parse() {

    // open file, check if okay
    FILE *fin = fopen (filename.c_str(), "r");
    if (0 == fin) {
        cerr << "file \"" << filename << "\" not found. quitting parser." << endl;
        return FILE_NOT_FOUND;
    }

    parse_err retcode = OKAY;
    errcurrentline = string ("");
    errreason = string ("");
    // parse
    linenumber = 0;
    globalBlockCount = 0; // The number of blocks in the implicit global-block
    push_block (BLOCK_GLOBAL);
    while (1) {
        // get next line
        char curr[1024];
        if (NULL == fgets (curr, sizeof (curr), fin)) {
            break;
        }
        ++linenumber;
        // remove trailing '\n'
        {
            unsigned int length = strlen (curr);
            if (curr[length-1] == '\n')
                curr[length-1] = '\0';
        }

        // interpret current line
        parse_err err = interpretLine (curr);
        if (err != OKAY) {
            retcode = err;
            errcurrentline = string (curr);
            break;
        }
        if (feof (fin)) {
            break;
        }
    }
    pop_block();

    // clean up
    fclose (fin);
    return retcode;
}



#if PICOSSDF_CC_STANDALONE // to be used when testing standalone
void print_usage() {
    cout << "usage:" << endl;
    cout << "  {this program} filename\n" << endl;
}
int main (int argc, char *argv[]) {
    --argc;
    ++argv;
    if (argc == 0) {
        print_usage();
        return 0;
    }
    try {
        PicoSSDF (string (argv[0]));
    } catch (PicoSSDF::exception_file_not_found e) {
    } catch (PicoSSDF::exception_unknown e) {
    } catch (PicoSSDF::exception_syntax_error e) {
        cerr << e.file << ":" << e.line << ":" << e.reason << "\nhere: \"" << e.curr << "\"" << endl;
    }
}
#endif // PICOSSDF_CC_STANDALONE

