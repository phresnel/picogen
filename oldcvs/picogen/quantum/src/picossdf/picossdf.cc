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

#include <boost/intrusive_ptr.hpp>

#include <picogen/picogen.h>
#include <picogen/misc/picossdf.h>
//#include <picogen/experimental/PicoPico.h>
#include <picogen/misc/scripting/nano-c.h>


using namespace std;
using ::picogen::real;
using ::picogen::geometrics::Vector3d;
using ::picogen::geometrics::Ray;

using ::picogen::graphics::material::abstract::IBRDF;
using ::picogen::graphics::objects::abstract::IScene;
using ::picogen::graphics::material::abstract::IShader;
using ::picogen::graphics::color::Color;

using ::picogen::misc::functional::Function_R6_R1_Refcounted;
using ::picogen::misc::functional::Function_R6_R1;
using ::boost::intrusive_ptr;


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
    using picogen::real;
    using picogen::geometrics::Vector3d;
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

// split a string, but don't split between any pair of braces
template <char splitter, char begin_brace, char end_brace> void splitString (std::vector<string> &dest, const std::string &source) {
    // TODO: catch error on bias<0
    // TODO: catch error on bias>0 at end of string
    using std::string;
    int bias = 0;
    string tmp = "";
    for (string::const_iterator it=source.begin(); bias>=0 && it!=source.end(); ++it) {
        switch (*it) {
            case begin_brace:
                ++bias;
                goto foo; // i am not going to discuss this
            case end_brace:
                --bias;
                goto foo; // no really
            case splitter:
                if (0 == bias) {
                    dest.push_back (tmp);
                    tmp = "";
                    break;
                }
                goto foo; // no.
            foo:
            default:
                tmp += *it;
        };
    }
    // final token
    if (0 >= bias) {
        dest.push_back (tmp);
    }
}

// split a string, but don't split between any pair of braces
template <char splitter> void splitString (std::vector<string> &dest, const std::string &source) {
    // TODO: catch error on bias<0
    // TODO: catch error on bias>0 at end of string
    using std::string;
    int bias = 0;
    string tmp = "";
    for (string::const_iterator it=source.begin(); bias>=0 && it!=source.end(); ++it) {
        switch (*it) {
            case '(':
            case '{':
            case '[':
            case '<':
                ++bias;
                goto foo; // i am not going to discuss this
            case ')':
            case '}':
            case ']':
            case '>':
                --bias;
                goto foo; // no really
            case splitter:
                if (0 == bias) {
                    dest.push_back (tmp);
                    tmp = "";
                    break;
                }
                goto foo; // no.
            foo:
            default:
                tmp += *it;
        };
    }
    // final token
    if (0 >= bias) {
        dest.push_back (tmp);
    }
}


IShader *parse_shader (const std::string &code);
namespace {
    namespace shader_parser {
        // TODO: urgent: release stuff if error occurs
        IShader *parse_hs_shader (const std::string &code);
        IShader *parse_const_rgb_shader (const std::string &params);
        void parse_shader_vector (std::vector<IShader*> &ishaders, const std::string &list);

        IShader *parse_const_rgb_shader (const std::string &params) {
            using ::picogen::graphics::color::Color;
            unsigned int numScalars;
            Vector3d rgb = scanVector3d (params, numScalars);
            if (numScalars >= 3) {
                //errreason = string ("too many values for parameter 'const-rgb'");
                return 0;
            }
            if (numScalars < 2) {
                //errreason = string ("not enough values for parameter 'const-rgb'");
                return 0;
            }
            return new picogen::graphics::shaders::ConstantShader (Color (rgb [0], rgb [1], rgb [2]));
        }

        IShader *parse_hs_shader (const std::string &code) {
            vector <string> params;
            splitString <','> (params, code);
            //cout << "code-str:" << code << ";" << endl;
            if (2 != params.size())
                return 0;
            unsigned int tmp = params [0].find_first_not_of (" \n\t");
            if (tmp >= params [0].size() || '{' != params [0][tmp])
                return 0;
            params [0][tmp] = ' '; // replace '{' with ' '

            // having sub-shaders now, so create the shading-object
            ::boost::intrusive_ptr<Function_R6_R1_Refcounted> fun (new Function_R6_R1_Refcounted (params [1]));
            std::vector <IShader*> ishaders;
            parse_shader_vector (ishaders, params [0]);
            return new picogen::graphics::shaders::HeightSlangShader (fun, ishaders);
        }

        void parse_shader_vector (std::vector<IShader*> &ishaders, const std::string &list) {
            using namespace std;
            vector <string> shaders;
            splitString <','> (shaders, list);
            for (vector <string>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
                //cout << "£ " << *it << endl;
                ishaders.push_back (parse_shader (*it));
            }
        }        
    }
}
IShader *parse_shader (const std::string &code) {
    using std::string;
    using namespace shader_parser;
    string type = "";
    string::const_iterator it;
    for (it = code.begin(); (*it == ' ' || *it == '\n' || *it == '\t') && it != code.end(); ++it) {
    }
    for (; *it != '(' && *it != ' ' && *it != '\n' && *it != '\t' && it != code.end(); ++it) {
        type += *it;
    }

    if (it == code.end()) {
        return 0;
    }

    int bias = 0;
    string shader_params = "";
    for (++it ; (*it != ')' || bias>0) && it != code.end(); ++it) {
        if ('(' == *it) {
            ++bias;
        } else if (')' == *it) {
            --bias;
        }
        shader_params += *it;
    }

    //cout << "code>>" << code << endl;
    if ("hs" == type) {
        //cout << "!hs-shader found, " << shader_params << endl;
        return parse_hs_shader (shader_params);
    } else if ("const-rgb"==type) {
        //cout << "!const-rgb-shader found, " << shader_params  << endl;
        return parse_const_rgb_shader (shader_params);
    } else {
        return 0;
    }
    return 0;
}

PicoSSDF::parse_err PicoSSDF::read_terminal (TERMINAL_TYPE type, const char *&line) {
    // TODO: Refactor the code in this function with some calls like 'parseVector', 'parseReal' and so forth.
    using namespace ::std;
    using namespace shader_parser;
    using ::picogen::real;
    using ::picogen::geometrics::Vector3d;
    using ::picogen::graphics::color::Color;
    using ::picogen::misc::functional::Function_R2_R1_Refcounted;
    using ::picogen::misc::functional::Function_R6_R1_Refcounted;
    using ::picogen::misc::functional::Function_R2_R1;

    // Step 1: Read Parameters.
    map<string,string> parameters;
    string name;
    string value;
    bool scanName = true;
    int braceBias = 0;
    ++line; // Eat '('.
    while (true) {

        // Check for end of string (it would give better performance if we would check that last).
        if (*line == '\0') {
            errreason = string ("Missing ')'.");
            return INCOMPLETE_TERMINAL;
        }

        // Check for braces, it could be that we are parsing height-slang.
        if (0 < braceBias || (!scanName && '(' == *line)) {
            if ('(' == *line) {
                ++braceBias;
            } else if (')' == *line) {
                --braceBias;
            }
        } else {
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
            } else if (*line == ' ' || *line == '\t') { // always (ALWAYS) eat up whitespace
                line++;
                continue;
            }
        }

        switch (scanName) {
            case true:  name  += *line; break;
            case false: value += *line; break;
        }
        ++line;
    }

    //if (0 < brace_bias



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
        } break;

        case TERMINAL_HEIGHTSLANG_HEIGHTFIELD: {
            int resolution = 128;
            Vector3d center (0,0,0), size (1,1,1);
            ::std::string hs ("0.5"), /*mi ("$"),*/ shader ("const-rgb(1,1,1)");
            /*::std::vector < ::picogen::graphics::material::abstract::IBRDF*> brdfs;
            ::std::vector < ::picogen::graphics::material::abstract::IShader*> shaders;*/

            while( !parameters.empty() ) {
                if ((*parameters.begin()).first == string ("resolution")) {
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> resolution;
                } else if ((*parameters.begin()).first == string ("center")) {
                    unsigned int numScalars;
                    const Vector3d tmp = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'center'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'center'");
                        return SYNTAX_ERROR;
                    }
                    center = tmp;
                } else if ((*parameters.begin()).first == string ("size")) {
                    unsigned int numScalars;
                    const Vector3d tmp = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'size'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'size'");
                        return SYNTAX_ERROR;
                    }
                    size = tmp;
                } else if ((*parameters.begin()).first == string ("code")) {
                    hs = (*parameters.begin()).second;
                } /*else if ((*parameters.begin()).first == string ("material-interpolator")) {
                    mi = (*parameters.begin()).second;
                }*/ /*else if ((*parameters.begin()).first == string ("color")) {
                    //!!
                    vector <string> colorss;
                    //vector <Color> colors;
                    splitString <',','(',')'> (colorss, (*parameters.begin()).second);
                    for (unsigned int i=0; i<colorss.size(); ++i) {
                        string type = "";
                        string::const_iterator it;
                        for (it=colorss [i].begin(); *it != '(' && it!=colorss [i].end(); ++it) {
                            type += *it;
                        }
                        ++it;

                        string params = "";
                        for (; *it != ')' && it!=colorss [i].end(); ++it) {
                            params += *it;
                        }

                        //cout << "type:'" << type << "' = '" << params << "'" << endl;
                        if ("color-rgb"==type) {
                            unsigned int numScalars;
                            Vector3d col_vec = scanVector3d (params, numScalars);
                            if (numScalars >= 3) {
                                errreason = string ("too many values for parameter 'color-rgb'");
                                return SYNTAX_ERROR;
                            }
                            if (numScalars < 2) {
                                errreason = string ("not enough values for parameter 'color-rgb'");
                                return SYNTAX_ERROR;
                            }
                            Color color;
                            color.from_rgb (col_vec [0], col_vec [1], col_vec [2]);
                            //colors.push_back (color);
                            shaders.push_back (new base_shaders::ConstantShader (color));
                        }
                    }
                }*/ else if ((*parameters.begin()).first == string ("material")) {
                    //cout << "[" << (*parameters.begin()).second << "]" << endl;
                    shader = (*parameters.begin()).second;

                } else {
                    errreason = string ("unknown parameter to hs-heightfield: '") + string ((*parameters.begin()).first) + string ("'");
                    return SYNTAX_ERROR;
                }
                parameters.erase (parameters.begin());
            }

            try {
                ::boost::intrusive_ptr<Function_R2_R1_Refcounted> fun (new Function_R2_R1_Refcounted (hs));
                //::boost::intrusive_ptr<Function_R6_R1_Refcounted> mat_fun (new Function_R6_R1_Refcounted ("$"==mi?hs:mi)); // <-- if $ is given, than it's the same as hs
                IShader *ishader = parse_shader (shader);
                backend->addHeightfield (fun, ishader, /*mat_fun, brdfs, shaders,*/ resolution, center, size);
            } catch (::picogen::misc::functional::functional_general_exeption &e) {
                errreason = string ("Exception caught while generating heightfield from height-slang code: "
                    + e.getMessage());
                return SYNTAX_ERROR;
            }

        } break;

        case TERMINAL_IMPLICIT_HEIGHTSLANG_HEIGHTFIELD: {
            Vector3d center (0,0,0), size (1,1,1);
            ::std::string hs ("0.5");

            while( !parameters.empty() ) {
                if ((*parameters.begin()).first == string ("center")) {
                    unsigned int numScalars;
                    const Vector3d tmp = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    center = tmp;
                } else if ((*parameters.begin()).first == string ("size")) {
                    unsigned int numScalars;
                    const Vector3d tmp = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    size = tmp;
                } else if ((*parameters.begin()).first == string ("code")) {
                    hs = (*parameters.begin()).second;
                } else {
                    errreason = string ("unknown parameter to hs-implicit-heightfield: '") + string ((*parameters.begin()).first) + string ("'");
                    return SYNTAX_ERROR;
                }
                parameters.erase (parameters.begin());
            }

            try {
                Function_R2_R1 *fun = new Function_R2_R1 (hs);
                backend->addImplicitHeightfield (&fun, center, size);
            } catch (::picogen::misc::functional::functional_general_exeption &e) {
                errreason = string ("Exception caught while generating heightfield from height-slang code: "
                    + e.getMessage());
                return SYNTAX_ERROR;
            }

        } break;

        case TERMINAL_PREETHAM: {
            while( !parameters.empty() ) {
                if ((*parameters.begin()).first == string ("turbidity")) {
                    real tmp;
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> tmp;
                    backend->preethamSetTurbidity (tmp);
                } else if ((*parameters.begin()).first == string ("sun-solid-angle-factor")) {
                    real tmp;
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> tmp;
                    backend->preethamSetSunSolidAngleFactor (tmp);
                } else if ((*parameters.begin()).first == string ("direction")) {
                    unsigned int numScalars;
                    const Vector3d direction = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'direction'");
                        return SYNTAX_ERROR;
                    }
                    backend->preethamSetSunDirection (direction);
                } else if ((*parameters.begin()).first == string ("fog-exponent")) {
                    real tmp;
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> tmp;
                    backend->preethamSetFogExp (tmp);
                } else if ((*parameters.begin()).first == string ("fog-max-distance")) {
                    real tmp;
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> tmp;
                    backend->preethamSetFogMaxDist (tmp);
                } else if ((*parameters.begin()).first == string ("color-filter-rgb")) {
                    unsigned int numScalars;
                    Vector3d col_vec = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'color-filter-rgb'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'color-filter-rgb'");
                        return SYNTAX_ERROR;
                    }
                    Color color;
                    color.from_rgb (col_vec [0], col_vec [1], col_vec [2]);
                    backend->preethamSetColorFilter (color);
                } else if ((*parameters.begin()).first == string ("sun-color-rgb")) {
                    unsigned int numScalars;
                    Vector3d col_vec = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'sun-color-rgb'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'sun-color-rgb'");
                        return SYNTAX_ERROR;
                    }
                    Color color;
                    color.from_rgb (col_vec [0], col_vec [1], col_vec [2] );
                    backend->preethamSetSunColor (color);
                } else if ((*parameters.begin()).first == string ("sun-falloff-parameters")) {
                    unsigned int numScalars;
                    Vector3d col_vec = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'sun-falloff-parameters'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'sun-falloff-parameters'");
                        return SYNTAX_ERROR;
                    }                    
                    backend->preethamSetSunFalloffParameters (true, col_vec [0], col_vec [1], col_vec [2]);
                } else {
                    errreason = string ("unknown parameter to sunsky: '") + string ((*parameters.begin()).first) + string ("'");
                    return SYNTAX_ERROR;
                }
                parameters.erase (parameters.begin());
            }
        } break;

        case TERMINAL_SET_CAMERA_YAW_PITCH_ROLL: {
            Vector3d position (0.0,0.0,0.0);
            real yaw = 0.0;
            real pitch = 0.0;
            real roll = 0.0;
            while( !parameters.empty() ) {
                if ((*parameters.begin()).first == string ("position")) {
                    unsigned int numScalars;
                    Vector3d vec = scanVector3d ((*parameters.begin()).second, numScalars);
                    if (numScalars >= 3) {
                        errreason = string ("too many values for parameter 'position'");
                        return SYNTAX_ERROR;
                    }
                    if (numScalars < 2) {
                        errreason = string ("not enough values for parameter 'position'");
                        return SYNTAX_ERROR;
                    }
                    position = vec;
                } else if ((*parameters.begin()).first == string ("yaw")) {
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> yaw;
                } else if ((*parameters.begin()).first == string ("pitch")) {
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> pitch;
                } else if ((*parameters.begin()).first == string ("roll")) {
                    stringstream ss;
                    ss << (*parameters.begin()).second;
                    ss >> roll;
                } else {
                    errreason = string ("unknown parameter to camera-yaw-pitch-roll: '") + string ((*parameters.begin()).first) + string ("'");
                    return SYNTAX_ERROR;
                }
                parameters.erase (parameters.begin());
            }
            backend->cameraSetPositionYawPitchRoll (position, yaw, pitch, roll);
        } break;
    }

    //backend->preethamEnableFogHack (preetham_fog_exp, preetham_fog_max_distance);

    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::read_state (STATE_TYPE stateType, const char *&line) {
    using namespace ::std;
    using ::picogen::real;
    using ::picogen::geometrics::Vector3d;
    using ::picogen::graphics::color::Color;

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
    int brace_bias = 0;
    if (*line == '(') { // Parameters are optional.
        ++line; // Eat '('.
        while (true) {

            if (0<brace_bias) {
                // klutch to allow for nested bracing-groups (e.g. to allow height-slang-parameters)
                if ('(' == *line) {
                    ++brace_bias;
                } else if (')' == *line) {
                    --brace_bias;
                }

                if (*line == '\0') {
                    errreason = string ("Missing ')'.");
                    return INCOMPLETE_TERMINAL;;
                }
            } else {
                // klutch to allow for nested bracing-groups (e.g. to allow height-slang-parameters)
                if ('(' == *line) {
                    ++brace_bias;
                    cout << "[" << *line << ":" << brace_bias << "]" << flush;
                } else if (')' == *line) {
                    --brace_bias;
                    cout << "[" << *line << ":" << brace_bias << "]" << flush;
                }

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
                    return INCOMPLETE_TERMINAL;;
                } else if (*line == ' ' || *line == '\t') { // always (ALWAYS) eat up whitespace
                    line++;
                    continue;
                }
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
        case STATE_MATERIAL:{
            if ("lambertian" == type || "specular" == type || "specular_distorted-height"==type  ) {
                real reflectance = 1.0;
                string code = "1"; // <-- for distorting brdfs
                while( !parameters.empty() ) {
                    if ((*parameters.begin()).first == string ("reflectance")) {
                        stringstream ss;
                        ss << (*parameters.begin()).second;
                        ss >> reflectance;
                    } else if ("specular_distorted-height"==type && (*parameters.begin()).first == string ("code")) {
                        code = (*parameters.begin()).second;
                    } else {
                        errreason = string ("unknown parameter to ") + type + string ("-material: '") + string ((*parameters.begin()).first) + string ("'");
                        return SYNTAX_ERROR;
                    }
                    parameters.erase (parameters.begin ());
                }
                if ("lambertian" == type) {
                    backend->setBRDFToLambertian (reflectance);
                } else if ("specular" == type) {
                    backend->setBRDFToSpecular (reflectance);
                } else if ("specular_distorted-height" == type) {
                    cout << "{{{" << code << "}}}" << endl;
                    backend->setBRDFToSpecular_DistortedHeight (reflectance, code);
                }
            } else {
                errreason = string ("material '") + type + string ("' unknown");
                return SYNTAX_ERROR;
            }
        } break;
    };

    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::interpretLine (const char *line) {
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
        } else if (!strcmp ("sunsky", block_name)) {
            type = TERMINAL_PREETHAM;
        } else if (!strcmp ("camera-yaw-pitch-roll", block_name)) {
            type = TERMINAL_SET_CAMERA_YAW_PITCH_ROLL;
        } else if (!strcmp ("hs-heightfield", block_name)) {
            type = TERMINAL_HEIGHTSLANG_HEIGHTFIELD;
        } else if (!strcmp ("hs-implicit-heightfield", block_name)) {
            type = TERMINAL_IMPLICIT_HEIGHTSLANG_HEIGHTFIELD;
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
        // UPDATE: below comment deprecated
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
        errreason = string ("trailing, non whitespace character(s) forbidden ('") +string (line-20) +string ("')");
        return SYNTAX_ERROR;
    }
    return OKAY;
}



PicoSSDF::parse_err PicoSSDF::interpretCode (const std::string &code, string::const_iterator &it) {
    /*using namespace std;
    for (; it!=code.end(); ++it) {
    }*/
    std::string curr;
    picogen::NanoC nanoC (code);
    while (nanoC.next (curr)) {
        parse_err err = interpretLine (curr.c_str());
        if (err != OKAY) {
            errcurrentline = string (curr);
            return err;
        }
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
    backend->initialize();
    push_block (BLOCK_GLOBAL);
    while (1) {
        // get next line
        /*char curr[1024];
        if (0 == fgets (curr, sizeof (curr), fin)) {
            break;
        }*/
        int num_open_braces = 0;
        string curr;
        while (!feof (fin)) {
            char tmp = fgetc (fin);
            num_open_braces += '('==tmp?1:')'==tmp?-1:0;
            if ('\0' != tmp && '\n' != tmp && EOF != tmp) {
                curr += tmp;
            } else {
                break;
            }
        }
        ++linenumber;
        // remove trailing '\n'
        /*{
            unsigned int length = strlen (curr);
            if (curr[length-1] == '\n')
                curr[length-1] = '\0';
        }*/
        /*string curr = "";
        int bias = -1;
        bool isBlock = false;
        while (0 != bias && !feof (fin) && (!isBlock || )) {
            char tmp = *fgetc (fin);
            curr += tmp;
            if (-1==bias && '{' == tmp) {
                isBlock
            }
        }*/

        // check if we have inline-code starting with an {{
        const char *c = curr.c_str();
        while (*c != '\0' && (*c == ' ' || *c == '\t')) {
            ++c;
        }
        if (c[0] != '\0' && c[0] == '{' && c[1] == '{' ) {
            // --> Found "{{"
            string code ("");
            while (!feof (fin)) {
                char c1 = fgetc (fin);
                char c2 = fgetc (fin); // Peek forward.
                if (c1 == '}' && c2 == '}') {
                    break;
                } else {
                    fseek (fin, -1, SEEK_CUR);
                    code += c1;
                }
            }
            string::const_iterator it = code.begin();
            interpretCode (code, it);
        } else {
            // interpret current line
            const parse_err err = interpretLine (curr.c_str());
            if (num_open_braces>0 && INCOMPLETE_TERMINAL == err) {
                // could be that the parameter list of a terminal has multiple lines, so scan until
                // braces are balanced instead of parsing to line-end, and then retry
                while (num_open_braces>0 && !feof (fin)) {
                    char tmp = fgetc (fin);
                    num_open_braces += '('==tmp?1:')'==tmp?-1:0;
                    if ('\0' != tmp && EOF != tmp) {
                        // replace newline characters with a space-char, so the rest of our ssdf-work can remain line-based
                        curr += '\n' != tmp ? tmp : ' ';
                    }
                }
                //cout << "{{{{{" << curr.c_str() << "}}}}}" << endl;
                const parse_err err = interpretLine (curr.c_str()); // <-- retry
                if (OKAY != err) {
                    retcode = err;
                    errcurrentline = string (curr);
                    break;
                }
            } else if (OKAY != err) {
                retcode = err;
                errcurrentline = string (curr);
                break;
            }
        }
        if (feof (fin)) {
            break;
        }
    }
    pop_block();
    backend->finish();

    // clean up
    fclose (fin);
//exit(0);
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

