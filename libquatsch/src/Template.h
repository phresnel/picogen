#ifndef TEMPLATE_H_INCLUDED_20120228
#define TEMPLATE_H_INCLUDED_20120228

#include <initializer_list>
#include <vector>
#include <list>

#include "template/StaticArgument.h"
#include "template/StaticParameter.h"

namespace quatsch { namespace extern_template {

        class Template
        {
        public:
                virtual ~Template() ;

                bool static_argument_exists (std::string const &name) const;
                StaticArgument static_argument (std::string const &name) const;

                void instantiate (std::list<StaticParameter> parameters) const;

                template <typename ...Args>
                void instantiate (Args... args) const
                { return instantiate (std::list<StaticParameter>{args...}); }

        protected:
                Template (std::initializer_list<StaticArgument>) ;

                /* boils down to this with delegating constructors
                template <typename ...Args>
                Template (Args ...args) : Template ({args...}) {}
                */

        private:
                std::vector<StaticArgument> static_args_;
        };


        class Test : public Template
        {
        public:
                Test() : Template({StaticArgument("foo", StaticType::String),
                                   StaticArgument("bar", StaticType::Float) })
                {}
        };
} }

#endif // TEMPLATE_H_INCLUDED_20120228
