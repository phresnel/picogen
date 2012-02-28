#ifndef STATICPARAMETER_H_INCLUDED_20120228
#define STATICPARAMETER_H_INCLUDED_20120228

#include <string>
#include "StaticType.h"

namespace quatsch { namespace extern_template {

class StaticParameter
{
public:
        static StaticParameter String  (std::string const &name, std::string const& val);
        static StaticParameter Integer (std::string const &name, int val);
        static StaticParameter Float   (std::string const &name, float val);

        std::string name() const;
        StaticType  type() const;

        std::string string  () const;
        int         integer () const;
        float       floating() const;

private:
        StaticParameter (std::string const &name,  StaticType type);

private:
        std::string name_;
        StaticType type_;
        std::string string_;
        int integer_;
        float float_;
};

} }

#endif // STATICPARAMETER_H_INCLUDED_20120228
