#include "quatsch.h"
#include "phase5/callable/to_callable.h"
#include "ppm.h"

int main ()
{
        using namespace quatsch;

        try {
                std::string     code = "(defun foo (x y) (* x y))\n"
                                       "(* x z)";
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

                const int width = 240, height = 240;

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
