#ifndef INSTANTIATION_H_INCLUDED_20120302
#define INSTANTIATION_H_INCLUDED_20120302

#include "quatsch_function.h"
#include "DynamicArgumentsMeta.h"

namespace quatsch { namespace extern_template {

struct Instantiation
{
        quatsch_function                      function;
        Typename                              return_type;
        extern_template::DynamicArgumentsMeta arguments_meta;
};


} }

#endif // INSTANTIATION_H_INCLUDED_20120302
