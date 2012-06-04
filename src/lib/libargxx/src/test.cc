#include "argxx.h"

#if 0
enum nflag_value {
        not_present,
        positive,
        negative
};
nflag_value nflag (Arguments &args, names const &n)
{
        if (flag (args, n)) return nflag_value::positive;
        const names nn {"no-"+n.short_opt, "no-"+n.long_opt};
        if (flag (args, nn)) return nflag_value::negative;
        return nflag_value::not_present;
}
#endif

int main (int argc, char *argv[]) {

        // TODO: make flag combination optional. Run with the current code
        //       ./test -ghfbar to see.

        try {
                auto parsed = argxx::parse (argc, argv);


                for (auto const & x : parsed) std::cout << x.name << ":" << x.value << std::endl;

                bool x = flag (parsed, argxx::names ("x", "extract"));
                bool v = flag (parsed, argxx::names ("v", "verbose"));
                std::string f = argxx::mandatory<std::string> (parsed,
                                                        argxx::names ("f", "filename"));
                bool z = flag (parsed, argxx::names ("z", "zortech"));


                for (auto const & x : parsed) std::cout << x.name << ":" << x.value << std::endl;


                /*auto const f = nflag(parsed, names("f", "foobar"));
                if (f == nflag_value::not_present) {
                        std::cout << "not presen\n";
                } else if (f == nflag_value::positive) {
                        std::cout << "foobar positive\n";
                } else if (f == nflag_value::negative) {
                        std::cout << "foobar negative\n";
                }

                auto const x = optional<unsigned int>(parsed, names("x", "Coeff"));

                if (x) {
                        std::cout << "{" << *x << "}" << std::endl;
                } else {
                        std::cout << "!{}" << std::endl;
                }*/
        } catch (std::exception &e) {
                std::cerr << "caught exception: " << e.what() << '\n';
        }
        /*
        int y = optional(parsed,
                         default_value(11),
                         names("y", "yankee"));
                         */
}
