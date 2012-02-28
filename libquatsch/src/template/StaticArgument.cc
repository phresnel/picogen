
#include "StaticArgument.h"

namespace quatsch { namespace extern_template {

StaticArgument::StaticArgument (std::string const &name, StaticType type)
        : name_(name), type_(type), required_(false)
{
}

StaticArgument::StaticArgument (std::string const &name, StaticType type,
                                required_t)
        : name_(name), type_(type), required_(true)
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

bool StaticArgument::required() const
{
        return required_;
}


} }
