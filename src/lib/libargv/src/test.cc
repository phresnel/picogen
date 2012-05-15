
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <stdexcept>

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
                if (str.size() < 2)
                        return Argument::NameOnly(str.substr(1));
                return Argument::NameValue(str.substr(1,1), str.substr(2));
        }
        Argument positional_argument (std::string str)
        {
                return Argument::Positional(str);
        }
}
Argument argument (std::string const &curr)
{
        using namespace detail;
        if (starts_with (curr, "--")) return long_argument (curr);
        if (starts_with (curr, "-"))  return short_argument (curr);
        return positional_argument (curr);
}

std::vector<std::string> catenate (int argc, char *argv[])
{
        std::list<std::string> raw_args (argv, argv+argc);
        std::vector<std::string> args;
        for (auto it = raw_args.begin(), end = raw_args.end(); it!=end; ++it) {
                if (!args.empty() &&
                   (*it == "="
                 || args.back().back() == '='))
                {
                        args.back() += *it;
                }
                else {
                        args.push_back(*it);
                }
        }
        return args;
}

int main (int argc, char *argv[]) {

        auto args = catenate (argc, argv);
        for (auto arg : args) {
                const Argument iarg = argument (arg);
                std::cout << "{" << arg << "}: "
                          << '"' << iarg.name << "\" <- \""
                          << iarg.value << "\" ("
                          << iarg.has_value << ")"
                          << '\n';
        }
}
