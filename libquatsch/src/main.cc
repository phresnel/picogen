#include <iostream>
#include <deque>
#include <boost/optional.hpp>

#include "phase1/tokenize.h"
#include "phase2/parse.h"
#include "phase3/resolve_and_verify.h"
#include "phase5/C99/to_C99.h"
#include "phase5/callable/to_callable.h"
#include "Template.h"

#include "ErrorState.h"

namespace quatsch { namespace extern_template {

class Test : public Template
{
public:
        Test() : Template("Test",
                          StaticArgumentMeta("foo", StaticType::String),
                          StaticArgumentMeta("bar", StaticType::Float),
                          StaticArgumentMeta("frob", StaticType::Float, required))
        {}

private:
        Instantiation instantiate_ (std::list<StaticParameter> const &) const;
};

Instantiation
Test::instantiate_ (std::list<StaticParameter> const &params) const
{
        return { [](DynamicArguments const &args){
                        return DynamicVariant::Floating(args[0].floating()*
                                                        args[1].floating());

                 },
                 Typename::Float,
                 {Typename::Float, Typename::Float}
               };
}

} }


quatsch::compiler::phase3::ProgramPtr
compile_phase3_program (std::string const &code,
                        std::list<quatsch::extern_template::TemplatePtr> const & templates
                        )
{
        using namespace quatsch::extern_template;
        using namespace quatsch::compiler;

        phase1::Toque toks = phase1::tokenize (code.begin(), code.end());
        phase2::ProgramPtr P2 = phase2::parse(toks);
        if (!P2) {
                std::cerr << "parse error" << std::endl;
                return 0;
        }

        quatsch::ErrorState err;
        phase3::ProgramPtr P3 = phase3::resolve_and_verify (*P2, templates, err);
        if (!P3) {
                std::cerr << "verficiation/resolution error" << std::endl;
                return 0;
        }
        return P3;
}


std::function<quatsch::extern_template::DynamicVariant(
                quatsch::extern_template::DynamicArguments const&
              )>
compile_callable (std::string const &code,
                  std::list<quatsch::extern_template::TemplatePtr> const & templates)
{
        using namespace quatsch::compiler;
        phase3::ProgramPtr P3 = compile_phase3_program (code, templates);
        return phase5::to_callable (*P3);
}


int main () {
        using namespace quatsch;
        namespace qe = quatsch::extern_template;

        std::list<qe::TemplatePtr> templates;
        templates.emplace_back (new qe::Test());
        auto fun = compile_callable (
                "\n"
                "(let pi:float 3.14159)\n"
                "(let two:float 3.14159)\n"
                "(defun main:int (x:int y) (main 2i 3))\n"
                //"([Test frob{3} foo{1 2 3}] 2 2)"
                "(defun T (x:int) (/ y y))\n"
                "([Test frob{1.5}] x y)",
                templates
        );

        qe::DynamicArguments args ({qe::DynamicVariant::Floating(0.5),
                                    qe::DynamicVariant::Floating(0.5)});
        std::cout << fun (args).floating() << std::endl;
}


