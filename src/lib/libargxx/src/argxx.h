#ifndef ARGXX_HH_INCLUDED_20120529
#define ARGXX_HH_INCLUDED_20120529

#include <string>
#include <iostream>

namespace argxx {
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
        inline std::ostream& operator<< (std::ostream &os, names const& n)
        {
                return os << "\"--" << n.long_opt << "\" (or alias \"-" << n.short_opt << "\")";
        }
}

#include "detail.h"

namespace argxx {

        using detail::Arguments;

        template <typename T>
        T mandatory (detail::State &state, names const &n)
        {
                state.non_flag_parsed = true;
                auto &args = state.arguments;

                auto it = find (args, n);
                if (it == args.end())
                        throw std::runtime_error("Mandatory argument '--" + n.long_opt +
                                                 "' (or '-" + n.short_opt + "') missing.");

                const auto ret = detail::convert_argument_value<T> (n, *it);
                args.erase (it);
                return ret;
        }

        template <typename T>
        T optional_with_default (detail::State &state, names const &n, T const &default_value)
        {
                state.non_flag_parsed = true;
                auto &args = state.arguments;

                auto it = find (args, n);
                if (it == args.end())
                        return default_value;

                const auto ret = detail::convert_argument_value<T> (n, *it);
                args.erase (it);
                return ret;
        }

        template <typename T>
        detail::optional<T> optional (detail::State &state, names const &n)
        {
                state.non_flag_parsed = true;
                auto &args = state.arguments;
                auto it = find (args, n);
                if (it == args.end())
                        return {n};

                const auto ret = detail::optional<T>{n, detail::convert_argument_value<T> (n, *it)};
                args.erase (it);
                return ret;
        }

        bool flag (detail::State &state, names const &n)
        {
                if (state.non_flag_parsed)
                        throw std::logic_error ("program logic error: tried to parse a flag after a non-flag "
                                                "has been parsed already; "
                                                "in order to prevent ambiguities when handling "
                                                "flag combinations (e.g. 'foo -xvf x'), this is "
                                                "forbidden. Fix this error before shipping: Query all flags first.");

                auto &args = state.arguments;

                auto it = find (args, n);
                if (it == end(args))
                        return false;

                // If there's a value, the end-user possibly combined multiple flags
                // at once (e.g. "tar -xzvf foobar.tar.gz"). In that case, we 'shift'
                // the argument to the left, like "tar -zvf foobar.tar.gz".
                if (it->has_value) {
                        *it = shift (*it);
                } else {
                        args.erase (it);
                }

                return true;
        }


        detail::State
        parse (int argc, char *argv[])
        {
                Arguments ret;
                if (argc) {
                        ret.push_back (detail::Argument::ProgramName (*argv));
                        --argc;
                        ++argv;
                }
                for (auto s : detail::catenate (argc, argv))
                        ret.push_back (detail::argument(s));
                return detail::State{ret};
        }


        void assert_no_unparsed_present (detail::State const &state)
        {
                if (state.arguments.empty())
                        return;
                if (state.arguments.size() == 1 &&
                    state.arguments.front().is_program_name)
                        return;

                std::stringstream ss;
                for (detail::Argument arg : state.arguments) {
                        if (arg.is_program_name) continue;
                        ss << "\n * name:<";
                        if (arg.has_name) ss << arg.name;
                        ss << ">, value:<";
                        if (arg.has_value) ss << arg.value;
                        ss << ">";
                }
                throw std::runtime_error ("unrecognized command line arguments:" + ss.str());
        }
}

#endif // ARGXX_HH_INCLUDED_20120529
