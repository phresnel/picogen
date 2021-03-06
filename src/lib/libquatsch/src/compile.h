#ifndef COMPILE_H_INCLUDED_20120402
#define COMPILE_H_INCLUDED_20120402

#include "ErrorState.h"
#include "Template.h"
#include "ProgramContract.h"
#include <functional>

namespace quatsch { namespace compiler { namespace phase3 {
        class Program;
} } }


namespace quatsch {

typedef std::list<quatsch::extern_template::TemplatePtr> TemplatePtrList;

quatsch::quatsch_function
compile (std::string const &code,
         TemplatePtrList const & templates,
         ProgramContract const &ptype,
         ErrorState &err,
         std::function<quatsch_function (compiler::phase3::Program const &,
                                         ProgramContract const &)> target
        ) ;

}

#endif // COMPILE_H_INCLUDED_20120402
