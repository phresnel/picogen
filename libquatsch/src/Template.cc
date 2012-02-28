#include "Template.h"
#include <stdexcept>

namespace quatsch { namespace extern_template {

Template::Template (std::initializer_list<StaticArgument> args)
        : static_args_(args)
{
}


Template::~Template()
{
}


bool Template::static_argument_exists (std::string const &name) const
{
        for (auto const &s : static_args_)
             if (s.name() == name) return true;
        return false;
}


StaticArgument Template::static_argument (std::string const &name) const
{
        for (auto const &s : static_args_)
                if (s.name() == name) return s;
        throw std::runtime_error ("quatsch::extern_template::Template::"
                                  "static_argument(\"" + name + "\") "
                                  "called, but '" + name + "' does not "
                                  "exist.");
}


void Template::instantiate (std::list<StaticParameter> parameters) const
{
        for (auto p : parameters) {
                if (!static_argument_exists (p.name()))
                        throw std::runtime_error( "parameter '"
                         + p.name() + "' given to instantiate(), "
                         "but it doesn't exist");
                StaticArgument sa = static_argument(p.name());
                if (sa.type() != p.type())
                        throw std::runtime_error("parameter '" +
                         p.name() + "' passed with incompatible"
                         " type.");
        }
}

} }
