#ifndef TO_C99_HH_INCLUDED_20120206
#define TO_C99_HH_INCLUDED_20120206

#include <string>
#include "phase3/Program.h"

namespace quatsch {
        class ProgramContract;
}

namespace quatsch { namespace compiler { namespace phase5 {
        std::string to_C99 (phase3::Program const &, ProgramContract const &);
} } }

#endif // TO_C99_HH_INCLUDED_20120206
