
#include "DynamicVariant.h"
#include <stdexcept>

namespace quatsch {

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

Typename DynamicVariant::type() const
{
        return type_;
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

}
