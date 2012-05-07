#ifndef PROGRAMTYPE_H_INCLUDED_20120507
#define PROGRAMTYPE_H_INCLUDED_20120507

#include "Typename.h"
#include <list>

namespace quatsch {

        struct ProgramArgumentMeta {
                std::string name;
                Typename    type;
        };

        struct ProgramType {

                Typename                       return_type;
                std::list<ProgramArgumentMeta> arguments;

                ProgramType() = default;
                ProgramType (Typename return_type,
                             std::initializer_list<ProgramArgumentMeta>);
        };

        namespace compiler { namespace phase3 { class Program; } }

        bool coherent (ProgramType const ptype, compiler::phase3::Program const & prog);

}

#endif // PROGRAMTYPE_H_INCLUDED_20120507
