
#include "StaticArgument.h"

namespace quatsch { namespace extern_template {

StaticArgument::StaticArgument (std::string const &name, StaticType type)
        : name_(name), type_(type)
{
}

std::string StaticArgument::name() const
{
        return name_;
}

StaticType StaticArgument::type() const
{
        return type_;
}


} }
