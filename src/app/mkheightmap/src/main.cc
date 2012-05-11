#include "quatsch.h"
#include "phase5/callable/to_callable.h"

int main ()
{
        using namespace quatsch;

        try {
                std::string code = "(defun foo (x y) (* x y))\n"
                                   "(foo 1 2)";
                TemplatePtrList templates;
                ErrorState errors;
                ProgramContract ptype (Typename::Float,
                                  {{"x",Typename::Float},
                                   {"z",Typename::Float}});

                quatsch::quatsch_function fun = quatsch::compile (
                                code, templates, ptype, errors, compiler::phase5::to_callable);

                if (errors) {
                        print_errors (errors, std::cerr);
                        return EXIT_FAILURE;
                }

                DynamicArguments args;
                args.push_back (DynamicVariant::Integer (2));
                args.push_back (DynamicVariant::Floating (3));
                //$ TODO: expected error
                const DynamicVariant ret = fun (args);

                std::cout << ret.integer() << std::endl; //$ TODO: should be no error

        } catch (std::exception &e) {
                std::cerr << "error: " << e.what() << '\n';
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
