#include "ProgramType.h"

namespace quatsch {

ProgramType::ProgramType (Typename return_type,
             std::initializer_list<ProgramArgumentMeta>)
        : return_type (return_type)
        , arguments (arguments)
{
}

}
