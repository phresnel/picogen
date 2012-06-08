#include "quatsch.h"
#include "phase5/callable/to_callable.h"
#include "ppm.h"
#include "argxx.h"
#include <fstream>

struct ProgramOptions {
        int width, height;
        std::string code;
};

ProgramOptions parse_options (int argc, char *argv[])
{
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

        return {width, height, code};
}

int main (int argc, char *argv[])
{
        using namespace quatsch;

        try {
                const ProgramOptions opts = parse_options (argc, argv);

                const TemplatePtrList templates;
                const ProgramContract ptype (Typename::Float, {{"x", Typename::Float},
                                                               {"z", Typename::Float}});

                ErrorState errors;
                quatsch::quatsch_function fun = quatsch::compile (
                                opts.code, templates, ptype, errors,
                                compiler::phase5::to_callable);

                if (errors) {
                        print_errors (errors, std::cerr);
                        return EXIT_FAILURE;
                }

                const auto iwidth  = 1. / static_cast<double> (opts.width),
                           iheight = 1. / static_cast<double> (opts.height);

                image::write_ppm (std::cout, opts.width, opts.height, [=](int x, int y) {
                        // Transform [0..image-size) -> [-0.5..0.5)
                        const double u_ = -0.5 + iwidth * x,
                                     v_ = -0.5 + iheight * (opts.height-1-y);

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
