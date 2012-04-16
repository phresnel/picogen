#include "compile_phase3_program.h"

#include "phase1/tokenize.h"
#include "phase2/parse.h"
#include "phase3/resolve_and_verify.h"

namespace quatsch { namespace compiler {

quatsch::compiler::phase3::ProgramPtr
compile_phase3_program (std::string const &code,
                        std::list<quatsch::extern_template::TemplatePtr> const & templates,
                        quatsch::ErrorState &err
                        )
{
        using namespace quatsch::extern_template;
        using namespace quatsch::compiler;

        phase1::Toque toks = phase1::tokenize (code.begin(), code.end(), err);
        if (err.has_errors()) {
                return 0;
        }
        if (toks.empty()) {
                err.post_error ("cannot compile an empty program\n",
                                code.begin(), code.end());
                return 0;
        }
        phase2::ProgramPtr P2 = phase2::parse(toks);
        if (!P2) {
                std::cerr << "parse error" << std::endl;
                return 0;
        }

        phase3::ProgramPtr P3 = phase3::resolve_and_verify (*P2, templates, err);
        if (!P3) {
                std::cerr << "verficiation/resolution error" << std::endl;
                return 0;
        }
        return P3;
}

} }
