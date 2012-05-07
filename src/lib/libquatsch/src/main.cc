#include <iostream>
#include "quatsch.h"
#include "ProgramType.h"
#include "phase5/callable/to_callable.h"

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

std::ostream& operator<< (std::ostream& os, quatsch::Typename t) {
        using quatsch::Typename;
        switch (t) {
        case Typename::Float:    os << "floating"; break;
        case Typename::Integer:  os << "integer"; break;
        }
        return os;
}

int main () {
        using namespace quatsch;
        namespace qe = quatsch::extern_template;


        quatsch::ErrorState err;

        quatsch::TemplatePtrList templates;
        templates.emplace_back (new qe::Test());

        quatsch::ProgramType ptype (Typename::Float,
                                    {{"x", Typename::Float}, {"y", Typename::Float}});

        auto fun = quatsch::compile (
                /*
                "(let pi:float 3.14159)\n"
                "(let two:float 3.14159)\n"
                "(defun main:int (x:int y) (main 2i 3))\n"
                //"([Test frob{3} foo{1 2 3}] 2 2)"
                "(defun T (x:int) (/ y y))\n"
                "([Test frob{1.5}] x y)"*/
                "(defun f:int (x:int) (* x x))\n"
                "y",
                templates,
                ptype,
                err,
                compiler::phase5::to_callable
        );
        if (err) {
                print_errors (err, std::cerr);
                return EXIT_FAILURE;
        }

        DynamicArguments args ({DynamicVariant::Floating(0.5),
                                DynamicVariant::Floating(0.5)});
        std::cout << fun (args).type() << std::endl;

        return EXIT_SUCCESS;
}


