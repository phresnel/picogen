#ifndef COMPILER_PHASE3_PROGRAM_H_INCLUDED_20120402
#define COMPILER_PHASE3_PROGRAM_H_INCLUDED_20120402

#include "Template.h"
#include "ErrorState.h"
#include "phase3/Program.h"
#include <string>
#include <list>

namespace quatsch {
        class ProgramType;
}

namespace quatsch { namespace compiler {

phase3::ProgramPtr
compile_phase3_program (std::string const &code,
                        std::list<quatsch::extern_template::TemplatePtr> const & templates,
                        ProgramType const &ptype,
                        quatsch::ErrorState &err
                        );
} }

#endif // COMPILER_PHASE3_PROGRAM_H_INCLUDED_20120402
