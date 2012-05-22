
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sstream>

bool starts_with (std::string const& str, std::string const &seq) {
        auto a = str.begin(), b = str.end(),
             x = seq.begin(), y = seq.end();
        for (; a != b && x != y; ++a, ++x) {
                if (*a != *x) return false;
        }
        if (a != b && x != y) return false;

        return true;
}

struct ArgumentMeta {
        bool flag;
};


struct Argument {
        bool has_name;
        std::string name;

        bool has_value;
        std::string value;

        static Argument Positional (std::string value) {
                Argument ret;
                ret.has_value = true;
                ret.value     = value;
                return ret;
        }

        static Argument NameValue (std::string name, std::string value) {
                Argument ret;
                ret.has_name  = true;
                ret.name      = name;
                ret.has_value = true;
                ret.value     = value;
                return ret;
        }

        static Argument NameOnly (std::string name) {
                Argument ret;
                ret.has_name = true;
                ret.name     = name;
                return ret;
        }

private:
        Argument () : has_name(false), has_value(false) {}
};

typedef std::vector<Argument> Arguments;


namespace detail {
        Argument long_argument (std::string str)
        {
                str = str.substr(2);
                const auto e = str.find_first_of('=');
                if (e == std::string::npos)
                        return Argument::NameOnly (str);
                return Argument::NameValue(str.substr(0, e), str.substr(1+e));
        }
        Argument short_argument (std::string str)
        {
                if (str.size() <= 2)
                        return Argument::NameOnly(str.substr(1));
                return Argument::NameValue(str.substr(1,1), str.substr(2));
        }
        Argument positional_argument (std::string str)
        {
                return Argument::Positional(str);
        }

        Argument argument (std::string const &curr)
        {
                if (starts_with (curr, "--")) return long_argument (curr);
                if (starts_with (curr, "-"))  return short_argument (curr);
                return positional_argument (curr);
        }

        std::vector<std::string> catenate (int argc, char *argv[])
        {
                std::list<std::string> raw_args (argv, argv+argc);
                std::vector<std::string> args;
                for (auto it = raw_args.begin(), end = raw_args.end(); it!=end; ++it) {
                        if (!args.empty()
                           && (*it == "=" || args.back().back() == '=')
                        ) {
                                args.back() += *it;
                        }
                        else {
                                args.push_back(*it);
                        }
                }
                return args;
        }
}

Arguments
parse (int argc, char *argv[])
{
        Arguments ret;
        for (auto s : detail::catenate (argc, argv))
                ret.push_back (detail::argument(s));
        return ret;
}

class names {
public:
        names (std::string const &short_opt,
               std::string const &long_opt)
                : short_opt (short_opt)
                , long_opt (long_opt)
        {
        }

        std::string short_opt, long_opt;
};

Arguments::const_iterator find (Arguments const &args, names const &n)
{
        auto it = std::find_if (args.begin(), args.end(), [&](Argument const &arg)
        {
                return arg.has_name && arg.name == n.short_opt;
        });
        if (it != args.end()) return it;

        return std::find_if (args.begin(), args.end(), [&](Argument const &arg)
        {
                return arg.has_name && arg.name == n.long_opt;
        });
}

namespace detail {
        template <typename T>
        bool convert (std::string const &str, T &ret)
        {
                std::stringstream ss;
                ss.str (str);
                ss >> ret;

                // fail() is set if nothing can be extracted (e.g.: int <- abc)
                // but we may also have partial extraction (e.g. int <- 2a), which
                // is covered by eof().
                if (ss.fail() || !ss.eof()) {
                        return false;
                }
                return true;
        }


        template <typename T> struct format_helper {
                //static_assert ("Missing format_helper-specialisation");
        };
        template <> struct format_helper<int> {
                static const char* format_example() { return "[+-]?[0-9]+"; }
        };
        template <> struct format_helper<unsigned int> {
                static const char* format_example() { return "[0-9]+"; }
        };
        template <> struct format_helper<float> {
                static const char* format_example() { return "[0-9]+.[0-9]+"; }
        };

        template <typename T>
        const char* format_example() {
                return format_helper<T>::format_example();
        }
}

template <typename T>
T mandatory (Arguments &args, names const &n)
{
        auto it = find (args, n);
        if (it == args.end())
                throw std::runtime_error("Mandatory argument '--" + n.long_opt +
                                         "' (or '-" + n.short_opt + "') missing.");

        if (!it->has_value)
                throw std::runtime_error("Missing value for argument '--" + n.long_opt +
                                         "' (or '-" + n.short_opt + "'). "
                                         "Write  '--" + n.long_opt + "=<value>' "
                                         "or '-" + n.short_opt + "<value>'");

        T ret;
        if (!detail::convert (it->value, ret)) {
                std::stringstream ss;
                ss << "Format error: '"
                   << it->value << "', passed for option "
                   << "'--" << n.long_opt << "' (or "
                   << "'-"  << n.short_opt << "') "
                   << "has bad format. It must be in format "
                   << detail::format_example<T>();
                throw std::runtime_error (ss.str());
        }
        return ret;
}


int main (int argc, char *argv[]) {

        auto parsed = parse (argc, argv);
        auto const x = mandatory<float>(parsed, names("x", "Coeff"));

        /*
        int y = optional(parsed,
                         default_value(11),
                         names("y", "yankee"));
                         */
}
