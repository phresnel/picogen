#ifndef PROGRAM_H_INCLUDED_20120125
#define PROGRAM_H_INCLUDED_20120125

#include "Tree.h"
#include "Defun.h"
#include "Constant.h"
#include <memory>

namespace quatsch { namespace compiler { namespace phase2 {

class Program;
typedef std::shared_ptr<Program> ProgramPtr;

class Program {
public:
        Program (TreePtr main, std::list<DefunPtr>, std::list<ConstantPtr>);

        TreePtr main() const;

        std::list<DefunPtr>    defuns   () const;
        std::list<ConstantPtr> constants() const;

private:
        TreePtr main_;
        std::list<DefunPtr> defuns_;
        std::list<ConstantPtr> constants_;
};

} } }

#endif // PROGRAM_H_INCLUDED_20120125
