
#include "code_iterator.h"
#include <ostream>

namespace quatsch { namespace compiler {

std::ostream& operator<< (std::ostream& os, code_iterator const &ci)
{
        return os << ci.line() << ":" << ci.column();
}

} }
