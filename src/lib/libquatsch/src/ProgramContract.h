#ifndef ProgramContract_H_INCLUDED_20120507
#define ProgramContract_H_INCLUDED_20120507

#include "Typename.h"
#include <list>
#include <string>

namespace quatsch {

        struct ProgramContractArgument {
                std::string name;
                Typename    type;
        };

        struct ProgramContract {

                Typename                       return_type;
                std::list<ProgramContractArgument> arguments;

                ProgramContract() = default;
                ProgramContract (Typename return_type,
                             std::initializer_list<ProgramContractArgument>);
        };

        namespace compiler { namespace phase3 { class Program; } }

        bool coherent (ProgramContract const &, compiler::phase3::Program const &, std::string &err);

        std::string to_string(ProgramContract const &);
}

#endif // ProgramContract_H_INCLUDED_20120507
