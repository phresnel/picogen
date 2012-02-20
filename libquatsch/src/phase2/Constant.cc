#include "Constant.h"
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase2 {

Constant::Constant (Type type, std::string const &name, TreePtr expr)
        : type_(type), name_(name), expression_(expr)
{
}

ConstantPtr Constant::Floating (std::string const &name, TreePtr expr)
{
        ConstantPtr ret{new Constant (Type::Floating, name, expr)};
        return ret;
}

ConstantPtr Constant::Integer (std::string const &name, TreePtr expr)
{
        ConstantPtr ret{new Constant (Type::Integer, name, expr)};
        return ret;
}

Constant::Type Constant::type() const
{
        return type_;
}

TreePtr Constant::expression() const
{
        return expression_;
}

/*
int Constant::integer() const
{
        assert_type (Type::Integer, "Constant::integer() called for non-integer");
        return integer_;
}
float Constant::floating() const
{
        assert_type (Type::Floating, "COnstant::floating() called for non-float");
        return floating_;
}*/

std::string Constant::name() const
{
        return name_;
}

void Constant::assert_type (Type t, const char *message) const
{
        if (this->type_ != t)
                throw std::runtime_error (message);
}

} } }
