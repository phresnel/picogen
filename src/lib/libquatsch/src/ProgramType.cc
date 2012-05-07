#include "ProgramType.h"
#include "phase3/Program.h"

namespace quatsch {

ProgramType::ProgramType (Typename return_type,
             std::initializer_list<ProgramArgumentMeta>)
        : return_type (return_type)
        , arguments (arguments)
{
}


bool coherent (ProgramType const ptype, compiler::phase3::Program const & prog)
{
        return false;
}

}
