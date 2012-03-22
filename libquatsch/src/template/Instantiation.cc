
#include "Instantiation.h"
#include <stdexcept>

namespace quatsch { namespace extern_template {

DynamicVariant::DynamicVariant (Typename type)
        : type_(type)
{
}

DynamicVariant DynamicVariant::Floating (float value)
{
        DynamicVariant ret {Typename::Float};
        ret.floating_ = value;
        return ret;
}

DynamicVariant DynamicVariant::Integer (int value)
{
        DynamicVariant ret {Typename::Integer};
        ret.integer_ = value;
        return ret;
}

float DynamicVariant::floating() const
{
        if (type_ != Typename::Float)
                throw std::runtime_error("called DynamicVariant::floating() for "
                                         "non-float");
        return floating_;
}

int DynamicVariant::integer() const
{
        if (type_ != Typename::Integer)
                throw std::runtime_error("called DynamicVariant::integer() for "
                                         "non-integer");
        return integer_;
}


/*
Instantiation::Instantiation (
        Typename return_type,
        DynamicArgumentsMeta const &dam,
        std::function<DynamicVariant(DynamicArguments)> const &fun)
 : return_type_(return_type), arg_desc_(dam), fun_(fun)
{}

Typename Instantiation::return_type() const
{
        return return_type_;
}

DynamicArgumentsMeta Instantiation::arguments_meta() const
{
        return arg_desc_;
}

DynamicVariant Instantiation::operator() (DynamicArguments const& in) const
{
        return fun_(in);
}
*/

} }
