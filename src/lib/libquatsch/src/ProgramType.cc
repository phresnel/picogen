#include "ProgramType.h"
#include "phase3/Program.h"
#include <sstream>

namespace quatsch {

ProgramType::ProgramType (Typename return_type,
                          std::initializer_list<ProgramArgumentMeta> arguments)
        : return_type (return_type)
        , arguments (arguments)
{
}



bool coherent (ProgramType const &ptype, compiler::phase3::Program const &prog, std::string &err)
{
        bool is_awesome = true;
        if (prog.main()->expression_type() != ptype.return_type) {
                std::stringstream ss;
                ss << "return type of program is "
                   << to_string (prog.main()->expression_type())
                   << ", but should be "
                   << to_string (ptype.return_type) << '\n';
                err += ss.str();
                is_awesome = false;
        }
        return is_awesome;
}

std::string to_string(ProgramType const &ptype) {

        std::string ret;
        auto pit  = ptype.arguments.begin(),
             pend = ptype.arguments.end();

        ret += "(";
        if (pit != pend) {
                ret += pit->name + ":" + to_string(pit->type);
                for (++pit; pit!=pend; ++pit)
                        ret += ", " + pit->name + ":" + to_string(pit->type);
        }
        ret += ")";
        ret += " -> " + to_string(ptype.return_type);
        return ret;
}

}
