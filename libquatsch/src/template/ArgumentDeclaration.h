#ifndef STATICARGUMENT_H_INCLUDED_20120228
#define STATICARGUMENT_H_INCLUDED_20120228

#include <string>
#include "StaticType.h"
#include "required.h"

namespace quatsch { namespace extern_template {

class ArgumentDeclaration
{
public:
        ArgumentDeclaration (std::string const &name, StaticType type);
        ArgumentDeclaration (std::string const &name, StaticType type, required_t);

        std::string name()     const ;
        StaticType  type()     const ;
        bool        required() const ;

private:
        std::string name_;
        StaticType type_;
        bool required_;
};

} }

#endif // STATICARGUMENT_H_INCLUDED_20120228
