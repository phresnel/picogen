#include "phase3/Program.h"

namespace quatsch { namespace compiler { namespace phase3 {

Program::Program (TreePtr main, std::list<DefunPtr> defuns) :
        main_(main), defuns_(defuns)
{
}

TreePtr Program::main() const
{
        return main_;
}

std::list<DefunPtr> Program::defuns() const
{
        return defuns_;
}

} } }
