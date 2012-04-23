#ifndef QUATSCH_FUNCTION_H_INCLUDED_20120402
#define QUATSCH_FUNCTION_H_INCLUDED_20120402

#include "DynamicVariant.h"
#include <functional>

namespace quatsch
{
        typedef std::function<DynamicVariant(DynamicArguments const& )>
                quatsch_function;
}

#endif // QUATSCH_FUNCTION_H_INCLUDED_20120402
