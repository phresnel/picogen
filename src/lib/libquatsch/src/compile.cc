#include "compile.h"
#include "compile_phase3_program.h"
#include "phase3/Program.h"
#include "DynamicVariant.h"
#include <stdexcept>

namespace quatsch {

quatsch::quatsch_function
compile (std::string const &code,
         TemplatePtrList const &templates,
         quatsch::ErrorState &err,
         std::function<quatsch_function (compiler::phase3::Program const &)> target)
{
        using namespace quatsch::compiler;
        phase3::ProgramPtr P3 = compile_phase3_program (code, templates, err);

        if (err.has_errors()) {
                return [] (quatsch::DynamicArguments const&)
                       -> quatsch::DynamicVariant
                        { throw std::runtime_error ("invalid quatsch program"); };
        }
        return target (*P3);
}

}
