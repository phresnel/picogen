
#include "StaticArgumentMeta.h"

namespace quatsch { namespace extern_template {

StaticArgumentMeta::StaticArgumentMeta (std::string const &name, StaticType type)
        : name_(name), type_(type), required_(false)
{
}

StaticArgumentMeta::StaticArgumentMeta (std::string const &name, StaticType type,
                                required_t)
        : name_(name), type_(type), required_(true)
{
}

std::string StaticArgumentMeta::name() const
{
        return name_;
}

StaticType StaticArgumentMeta::type() const
{
        return type_;
}

bool StaticArgumentMeta::required() const
{
        return required_;
}


} }
