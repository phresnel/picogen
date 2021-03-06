#ifndef DETAIL_H_INCLUDED_20120529
#define DETAIL_H_INCLUDED_20120529

#include <list>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <limits>

namespace argxx {
        struct State;
        State parse (int argc, char *argv[]);
}

namespace argxx { namespace detail {

        inline bool starts_with (std::string const& str, std::string const &seq) {
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

                bool is_program_name;

                static Argument ProgramName (std::string value) {
                        Argument ret = Positional (value);
                        ret.is_program_name = true;
                        return ret;
                }

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
                Argument () : has_name(false), has_value(false), is_program_name(false) {}
        };

        typedef std::list<Argument> Arguments;

} }

namespace argxx {

        // Put State into namespace argxx (instead of detail) so ADL
        // triggers properly when the user types
        //    const auto foo = xxx<> (state, ...);
        // .
        struct State {
                detail::Arguments arguments;
                bool non_flag_parsed;

        private:
                friend State parse (int argc, char *argv[]);

                State () : non_flag_parsed(false) {}
                State (detail::Arguments const &args) : arguments (args), non_flag_parsed(false) {}
        };

        inline detail::Arguments::iterator begin (State &state) { return state.arguments.begin(); }
        inline detail::Arguments::iterator end   (State &state) { return state.arguments.end(); }
        inline detail::Arguments::const_iterator begin (State const &state) { return state.arguments.begin(); }
        inline detail::Arguments::const_iterator end   (State const &state) { return state.arguments.end(); }
}

namespace argxx { namespace detail {

        inline
        Argument shift (Argument const &arg)
        {
                if (arg.value.size() <= 1) {
                        return Argument::NameOnly (arg.value);
                }
                return Argument::NameValue (arg.value.substr(0, 1),
                                            arg.value.substr(1));
        }

        inline
        Argument long_argument (std::string str)
        {
                str = str.substr(2);
                const auto e = str.find_first_of('=');
                if (e == std::string::npos)
                        return Argument::NameOnly (str);
                return Argument::NameValue(str.substr(0, e), str.substr(1+e));
        }
        inline
        Argument short_argument (std::string str)
        {
                if (str.size() <= 2)
                        return Argument::NameOnly(str.substr(1));
                return Argument::NameValue(str.substr(1,1), str.substr(2));
        }
        inline
        Argument positional_argument (std::string str)
        {
                return Argument::Positional(str);
        }

        inline
        Argument argument (std::string const &curr)
        {
                if (starts_with (curr, "--")) return long_argument (curr);
                if (starts_with (curr, "-"))  return short_argument (curr);
                return positional_argument (curr);
        }

        inline
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



        inline
        Arguments::iterator find (Arguments &args, names const &n)
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

        template <typename T>
        struct convert_helper {
                static bool convert (std::string const &str, T &ret) {
                        std::stringstream ss;
                        ss.str (str);
                        ss >> ret;

                        // The following is needed because streams accept negative signs
                        // even for unsigned types.
                        constexpr bool forbid_minus = std::numeric_limits<T>::is_integer &&
                                                      !std::numeric_limits<T>::is_signed;
                        if (forbid_minus) {
                                if (std::string::npos != str.find_first_of('-'))
                                        return false;
                        }

                        // fail() is set if nothing can be extracted (e.g.: int <- abc)
                        // but we may also have partial extraction (e.g. int <- 2a), which
                        // is covered by eof().
                        if (ss.fail() || !ss.eof()) {
                                return false;
                        }
                        return true;
                }
        };

        template <>
        struct convert_helper<std::string> {
                static bool convert (std::string const &str, std::string &ret) {
                        ret = str;
                        return true;
                }
        };

        template <typename T>
        inline
        bool convert (std::string const &str, T &ret)
        {
                return convert_helper<T>::convert (str, ret);
        }




        template <typename T> struct type_helper;

        template <> struct type_helper<int> {
                static const char* format_example() { return "1, 0, -99, +20"; }
                static const char* type_name () { return "integer"; }
        };
        template <> struct type_helper<unsigned int> {
                static const char* format_example() { return "10, +500, 99"; }
                static const char* type_name () { return "positive integer"; }
        };
        template <> struct type_helper<float> {
                static const char* format_example() { return "0.5, -2.3, 1e10"; }
                static const char* type_name () { return "real"; }
        };
        template <> struct type_helper<bool> {
                static const char* format_example() { return "1, 0"; }
                static const char* type_name() { return "boolean"; }
        };
        template <> struct type_helper<std::string> {
                static const char* format_example() { return "\"foobar\""; }
                static const char* type_name() { return "string"; }
        };

        template <typename T> const char* format_example()
        {
                return type_helper<T>::format_example();
        }

        template <typename T> const char* type_name()
        {
                return type_helper<T>::type_name();
        }


        template <typename T> struct limit_diagnostic
        {
                friend std::ostream& operator<< (std::ostream& os, limit_diagnostic)
                {
                        if (std::numeric_limits<T>::is_signed) {
                                os << std::fixed
                                   << "Must be between " << -std::numeric_limits<T>::max()
                                   << " and " << std::numeric_limits<T>::max();
                        } else {
                                os << "Must be between 0 and " << std::numeric_limits<T>::max();
                        }
                        return os << ".";
                }
        };
        template <> struct limit_diagnostic<std::string>
        {
                friend std::ostream& operator<< (std::ostream &os, limit_diagnostic) {
                        return os;
                }
        };
        template <> struct limit_diagnostic<bool>
        {
                friend std::ostream& operator<< (std::ostream &os, limit_diagnostic) {
                        return os << "Must be 0 or 1.";
                }
        };

        template <typename T>
        inline
        T convert_argument_value (names const &n, Argument const &arg) {

                if (!arg.has_value)
                        throw std::runtime_error("Missing value for argument '--" + n.long_opt +
                                                 "' (or '-" + n.short_opt + "'). "
                                                 "Write  '--" + n.long_opt + "=<value>' "
                                                 "or '-" + n.short_opt + "<value>'");


                T ret;
                if (!detail::convert (arg.value, ret)) {
                        std::stringstream ss;
                        ss << "Format error: Passed value '" << arg.value << "' for option "
                           << n << ", but " << type_name<T>() << " is expected"
                           << ". Examples for valid formats: " << detail::format_example<T>()
                           << ". ";
                        ss << limit_diagnostic<T>();
                        throw std::runtime_error (ss.str());
                }
                return ret;
        }


        // lightweight wrapper. does nothing fancy nor performant
        // (it is NOT a replacement for boost::optional!)
        template <typename T>
        class optional {
        public:
                optional (names const &n) : arg_names_(n), has_value_(false) {}
                optional (names const &n, T const &v) : arg_names_(n), value_(v) {}

                explicit operator bool () const {
                        return has_value_;
                }

                T operator* () const {
                        if (!*this) {
                                std::ostringstream ss;
                                ss << "Value for optional " << arg_names_
                                   << " dereferenced, but it wasn't given a value";
                                throw std::runtime_error(ss.str());
                        }
                        return value_;
                }
        private:
                names arg_names_;
                bool has_value_;
                T    value_;
        };

} }

#endif // DETAIL_H_INCLUDED_20120529
