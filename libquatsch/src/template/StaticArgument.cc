#include "StaticArgument.h"
#include <stdexcept>

namespace quatsch { namespace extern_template {

StaticParameter::StaticParameter (std::string const &name,  StaticType type)
        : name_(name), type_(type)
{
}

StaticParameter StaticParameter::String (std::string const &name,
                                         std::string const &val)
{
        StaticParameter ret {name, StaticType::String};
        ret.string_ = val;
        return ret;
}
StaticParameter StaticParameter::Integer (std::string const &name,
                                          int val)
{
        StaticParameter ret {name, StaticType::Integer};
        ret.integer_ = val;
        return ret;
}
StaticParameter StaticParameter::Float (std::string const &name,
                                        float val)
{
        StaticParameter ret {name, StaticType::Float};
        ret.float_ = val;
        return ret;
}
std::string StaticParameter::name() const
{
        return name_;
}
StaticType StaticParameter::type() const
{
        return type_;
}

std::string StaticParameter::string() const
{
        if (type_ != StaticType::String)
                throw std::runtime_error ("StaticParameter::string() "
                                          "called for non-string");
        return string_;
}
int StaticParameter::integer() const
{
        if (type_ != StaticType::Integer)
                throw std::runtime_error ("StaticParameter::integer() "
                                          "called for non-integer");
        return integer_;
}
float StaticParameter::floating() const
{
        if (type_ != StaticType::Float)
                throw std::runtime_error ("StaticParameter::floating() "
                                          "called for non-float");
        return float_;
}

} }
