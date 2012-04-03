#include <iostream>
#include <deque>
#include <stdexcept>
#include <boost/optional.hpp>

#include "phase5/callable/to_callable.h"
#include "Template.h"

#include "compile.h"

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



int main () {
        using namespace quatsch;
        namespace qe = quatsch::extern_template;


        quatsch::ErrorState err;

        quatsch::TemplatePtrList templates;
        templates.emplace_back (new qe::Test());
        auto fun = quatsch::compile (
                "(let pi:float 3.14159)\n"
                "(let two:float 3.14159)\n"
                "(defun main:int (x:int y) (main 2i 3))\n"
                //"([Test frob{3} foo{1 2 3}] 2 2)"
                "(defun T (x:int) (/ y y))\n"
                "([Test frob{1.5}] x y)",
                templates,
                err,
                compiler::phase5::to_callable
        );
        if (err) {
                print_errors (err, std::cerr);
                return EXIT_FAILURE;
        }

        DynamicArguments args ({DynamicVariant::Floating(0.5),
                                DynamicVariant::Floating(0.5)});
        std::cout << fun (args).floating() << std::endl;

        return EXIT_SUCCESS;
}


