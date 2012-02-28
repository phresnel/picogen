#ifndef STATICARGUMENT_H_INCLUDED_20120228
#define STATICARGUMENT_H_INCLUDED_20120228

#include <string>
#include "StaticType.h"

namespace quatsch { namespace extern_template {

class StaticArgument
{
public:
        StaticArgument (std::string const &name, StaticType type);

        std::string name() const ;
        StaticType  type() const ;

private:
        std::string name_;
        StaticType type_;
};

} }

#endif // STATICARGUMENT_H_INCLUDED_20120228
