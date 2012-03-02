#ifndef TEMPLATE_H_INCLUDED_20120228
#define TEMPLATE_H_INCLUDED_20120228

#include <initializer_list>
#include <vector>
#include <list>

#include "template/ArgumentDeclaration.h"
#include "template/StaticParameter.h"
#include "template/Instantiation.h"

namespace quatsch { namespace extern_template {

        class Template
        {
        public:
                virtual ~Template() ;

                bool static_argument_exists (std::string const &name) const;
                ArgumentDeclaration static_argument (std::string const &name) const;

                Instantiation instantiate (std::list<StaticParameter> parameters) const;

                template <typename ...Args>
                Instantiation instantiate (Args... args) const
                { return instantiate (std::list<StaticParameter>{args...}); }

        protected:
                Template (std::initializer_list<ArgumentDeclaration>) ;

                /* boils down to this with delegating constructors
                template <typename ...Args>
                Template (Args ...args) : Template ({args...}) {}
                */

        private:
                std::vector<ArgumentDeclaration> static_args_;
        };


        class Test : public Template
        {
        public:
                Test() : Template({ArgumentDeclaration("foo", StaticType::String),
                                   ArgumentDeclaration("bar", StaticType::Float),
                                   ArgumentDeclaration("frob", StaticType::Float, required)})
                {}
        };
} }

#endif // TEMPLATE_H_INCLUDED_20120228
