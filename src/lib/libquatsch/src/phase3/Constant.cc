#include "Constant.h"
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase3 {


ConstantPtr Constant::Floating (std::string const &name, float value)
{
        ConstantPtr ret {new Constant (Typename::Float, name)};
        ret->floating_ = value;
        return ret;
}

ConstantPtr Constant::Integer  (std::string const &name, int value)
{
        ConstantPtr ret {new Constant (Typename::Integer, name)};
        ret->integer_ = value;
        return ret;
}

Typename Constant::type() const
{
        return type_;
}

std::string Constant::name() const
{
        return name_;
}

float Constant::floating () const
{
        assert_type (Typename::Float, "phase3::Constant::floating() "
                                              "called for non-float");
        return floating_;
}

int Constant::integer () const
{
        assert_type (Typename::Integer, "phase3::Constant::integer() "
                                                "called for non-integer");
        return integer_;
}

Constant::Constant (Typename t, std::string const &name)
        : type_(t), name_(name)
{
}

void Constant::assert_type (Typename t, std::string const &message) const
{
        if (this->type() != t)
                throw std::runtime_error (message);
}


} } }

