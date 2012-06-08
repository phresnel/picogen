#include "quatsch.h"
#include "phase5/callable/to_callable.h"
#include "ppm.h"
#include "argxx.h"
#include <fstream>

int main (int argc, char *argv[])
{
        using namespace quatsch;

        try {
                auto args = argxx::parse (argc, argv);
                const auto code_opt     = argxx::optional<std::string> (args, {'c', "code"});
                const auto filename_opt = argxx::optional<std::string> (args, {'f', "filename"});
                if ((!code_opt && !filename_opt) || (code_opt && filename_opt)) {
                        throw std::runtime_error("error: either pass --code=<code>, "
                                                 "or --filename=<quatsch-file>");
                }
                const auto width  = argxx::optional_with_default<unsigned> (args, {'w', "width"}, 256);
                const auto height = argxx::optional_with_default<unsigned> (args, {'h', "height"}, width);
                argxx::assert_no_unparsed_present (args);

                // TODO: extract function
                std::string code;
                if (filename_opt) {
                        std::ifstream ifs (*filename_opt);
                        if (!ifs.good())
                                throw std::runtime_error("error: could not open file \""
                                                         + *filename_opt + "\"");

                        std::string line;
                        while (getline (ifs, line))
                                code += line;
                } else {
                        code = *code_opt;
                }
                /*
                std::string     code = "(defun foo (x y) (* x y))\n"
                                       "(* x z)";
                */
                TemplatePtrList templates;
                ErrorState      errors;
                ProgramContract ptype (Typename::Float,
                                  {{"x",Typename::Float},
                                   {"z",Typename::Float}});

                quatsch::quatsch_function fun = quatsch::compile (
                                code, templates, ptype, errors,
                                compiler::phase5::to_callable);

                if (errors) {
                        print_errors (errors, std::cerr);
                        return EXIT_FAILURE;
                }

                image::write_ppm (std::cout, width, height, [=](int x, int y) {
                        // Transform [0..image-size) -> [-0.5..0.5)
                        const double u_ = (x / static_cast<double> (width))-0.5,
                                     v_ = ((height-1-y) / static_cast<double> (height))-0.5;

                        // Call height-function.
                        const auto   h_ = fun ({DynamicVariant::Floating(u_),
                                                DynamicVariant::Floating(v_)});

                        // Done!
                        const double h = h_.floating();
                        return std::make_tuple(h, h, h);
                });

        } catch (std::exception &e) {
                std::cerr << "error: " << e.what() << '\n';
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
