#include "Argument.h"

namespace quatsch { namespace compiler { namespace phase2 {

Argument::Argument (std::string const &name,
                    code_iterator begin, code_iterator end) :
        name_(name), type_(Typename::Float),
        code_begin_(begin), code_end_(end)
{
}

Argument::Argument (std::string const &name, Typename type,
                    code_iterator begin, code_iterator end) :
        name_(name), type_(type),
        code_begin_(begin), code_end_(end)
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

code_iterator Argument::code_begin() const
{
        return code_begin_;
}

code_iterator Argument::code_end() const
{
        return code_end_;
}

} } }
