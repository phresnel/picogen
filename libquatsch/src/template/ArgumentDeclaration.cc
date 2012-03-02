
#include "ArgumentDeclaration.h"

namespace quatsch { namespace extern_template {

ArgumentDeclaration::ArgumentDeclaration (std::string const &name, StaticType type)
        : name_(name), type_(type), required_(false)
{
}

ArgumentDeclaration::ArgumentDeclaration (std::string const &name, StaticType type,
                                required_t)
        : name_(name), type_(type), required_(true)
{
}

std::string ArgumentDeclaration::name() const
{
        return name_;
}

StaticType ArgumentDeclaration::type() const
{
        return type_;
}

bool ArgumentDeclaration::required() const
{
        return required_;
}


} }
