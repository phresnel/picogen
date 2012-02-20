#ifndef PROGRAM_H_INCLUDED_20120206
#define PROGRAM_H_INCLUDED_20120206

#include "Tree.h"
#include <list>
#include <memory>

namespace quatsch { namespace compiler { namespace phase3 {

class Program;
typedef std::shared_ptr<Program> ProgramPtr;

class Program {
public:
        Program (TreePtr main, std::list<DefunPtr> defuns);
        TreePtr main() const;
        std::list<DefunPtr> defuns() const;
private:
        TreePtr main_;
        std::list<DefunPtr> defuns_;
};



} } }

#endif // PROGRAM_H_INCLUDED_20120206
