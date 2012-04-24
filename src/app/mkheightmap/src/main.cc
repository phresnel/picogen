#include "quatsch.h"
#include "phase5/callable/to_callable.h"

int main ()
{
        using namespace quatsch;

        try {
                std::string code = "(defun foo:int (x y) (* x y))\n"
                                   "(foo x y)";
                TemplatePtrList templates;
                ErrorState errors;

                quatsch::quatsch_function fun = quatsch::compile (
                                code, templates, errors, compiler::phase5::to_callable);
                if (errors) {
                        print_errors (errors, std::cerr);
                        return EXIT_FAILURE;
                }

                DynamicArguments args;
                args.push_back (DynamicVariant::Floating (2));
                args.push_back (DynamicVariant::Floating (3));
                args.push_back (DynamicVariant::Floating (4));
                const DynamicVariant ret = fun (args);

                std::cout << ret.floating() << std::endl;

        } catch (std::exception &e) {
                std::cerr << "error: " << e.what() << '\n';
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
