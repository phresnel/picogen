
#include "Instantiation.h"

namespace quatsch { namespace extern_template {

DynamicArgument::DynamicArgument (Typename type)
        : type_(type)
{
}

DynamicArgument DynamicArgument::Floating (float value)
{
        DynamicArgument ret {Typename::Float};
        ret.floating_ = value;
        return ret;
}

DynamicArgument DynamicArgument::Integer (int value)
{
        DynamicArgument ret {Typename::Integer};
        ret.integer_ = value;
        return ret;
}

float DynamicArgument::floating() const
{
        return floating_;
}

int DynamicArgument::integer() const
{
        return integer_;
}


Typename Instantiation::return_type() const
{
        return return_type_;
}

DynamicArgumentsMeta Instantiation::arguments_meta() const
{
        return arg_desc_;
}


} }
