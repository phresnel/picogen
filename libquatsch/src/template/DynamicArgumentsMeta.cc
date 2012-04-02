#include "DynamicArgumentsMeta.h"

namespace quatsch { namespace extern_template {

DynamicArgumentsMeta::DynamicArgumentsMeta (std::initializer_list<Typename> types)
        : list_(types)
{}

DynamicArgumentsMeta::const_iterator DynamicArgumentsMeta::begin() const
{
        return list_.begin();
}

DynamicArgumentsMeta::const_iterator DynamicArgumentsMeta::end() const
{
        return list_.end();
}

DynamicArgumentsMeta::size_type DynamicArgumentsMeta::size() const
{
        return list_.size();
}


} }
