
#include "Program.h"

namespace quatsch { namespace compiler { namespace phase2 {

Program::Program(TreePtr main, std::list<DefunPtr> defuns,
                 std::list<ConstantPtr> constants)
        : main_(main), defuns_(defuns), constants_(constants)
{}

TreePtr Program::main() const
{
        return main_;
}

std::list<DefunPtr> Program::defuns() const
{
        return defuns_;
}

std::list<ConstantPtr> Program::constants() const
{
        return constants_;
}

} } }
