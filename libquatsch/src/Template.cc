#include "Template.h"
#include <stdexcept>
#include <set>

namespace quatsch { namespace extern_template {

Template::Template (std::initializer_list<ArgumentDeclaration> args)
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


ArgumentDeclaration Template::static_argument (std::string const &name) const
{
        for (auto const &s : static_args_)
                if (s.name() == name) return s;
        throw std::runtime_error ("quatsch::extern_template::Template::"
                                  "static_argument(\"" + name + "\") "
                                  "called, but '" + name + "' does not "
                                  "exist.");
}


Instantiation Template::instantiate (std::list<StaticParameter> parameters) const
{
        std::set<std::string> defined_names;
        for (auto p : parameters) {
                const bool defined = defined_names.find(p.name())
                                     != defined_names.end();
                if (defined)
                        throw std::runtime_error ("parameter '" + p.name() +
                                                  "' already defined");

                if (!static_argument_exists (p.name()))
                        throw std::runtime_error( "parameter '"
                         + p.name() + "' given to instantiate(), "
                         "but it doesn't exist");
                ArgumentDeclaration sa = static_argument(p.name());
                if (sa.type() != p.type())
                        throw std::runtime_error("parameter '" +
                         p.name() + "' passed with incompatible"
                         " type.");
                defined_names.insert (p.name());
        }

        for (auto a : static_args_) {
                if (a.required()) {
                        const bool defined = defined_names.find(a.name())
                                             != defined_names.end();
                        if (!defined)
                                throw std::runtime_error("required parameter '"
                                                      + a.name() + "' not set");
                }
        }

        return Instantiation();
}

} }
