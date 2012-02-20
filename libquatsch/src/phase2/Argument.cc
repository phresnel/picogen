#include "Argument.h"

namespace quatsch { namespace compiler { namespace phase2 {

Argument::Argument (std::string const &name) :
        name_(name), type_(Typename::Float)
{
}

Argument::Argument (std::string const &name, Typename type) :
        name_(name), type_(type)
{
}

void Argument::reset_type (Typename type)
{
        type_ = type;
}

Typename Argument::type() const
{
        return type_;
}

std::string Argument::name () const
{
        return name_;
}

} } }
