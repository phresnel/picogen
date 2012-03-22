
#include "to_callable.h"

namespace quatsch { namespace compiler { namespace phase5 {

extern_function to_callable (phase3::Program const &prog)
{
        using extern_template::DynamicArguments;
        using extern_template::DynamicVariant;

        return [] (DynamicArguments const &da) {
                return DynamicVariant::Floating (0.33);
        };
}

} } }
