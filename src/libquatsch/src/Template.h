#ifndef TEMPLATE_H_INCLUDED_20120228
#define TEMPLATE_H_INCLUDED_20120228

#include <initializer_list>
#include <vector>
#include <list>
#include <functional>
#include <memory>

#include "template/StaticArgumentMeta.h"
#include "template/StaticArgument.h"
#include "template/Instantiation.h"
#include "DynamicVariant.h"

namespace quatsch { namespace extern_template {

        class Template
        {
        public:
                virtual ~Template() ;

                std::string name() const;

                bool static_argument_exists (std::string const &name) const;
                StaticArgumentMeta static_argument (std::string const &name) const;

                Instantiation
                instantiate (std::list<StaticParameter> const &parameters) const;

                template <typename ...Args>
                Instantiation instantiate (Args... args) const
                { return instantiate (std::list<StaticParameter>{args...}); }

        protected:
                Template (std::string const &name,
                          std::initializer_list<StaticArgumentMeta>) ;

                /*boils down to this with delegating constructors
                template <typename ...Args>
                Template (Args ...args) : Template ({args...}) {}
                */
                template <typename ...Args>
                Template (std::string const &name, Args ...args)
                        : name_(name), static_args_({args...})
                {}

        private:

                // Precondition: the list of parameters is type- and
                //               requiredness-validated.
                // Postcondition: A valid Instantiation object.
                virtual
                  Instantiation instantiate_ (std::list<StaticParameter> const &)
                  const = 0;
        private:
                std::string name_;
                std::vector<StaticArgumentMeta> static_args_;
        };

        typedef std::shared_ptr<Template> TemplatePtr;

} }

#endif // TEMPLATE_H_INCLUDED_20120228
