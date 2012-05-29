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
        T mandatory (Arguments &args, names const &n)
        {
                auto it = find (args, n);
                if (it == args.end())
                        throw std::runtime_error("Mandatory argument '--" + n.long_opt +
                                                 "' (or '-" + n.short_opt + "') missing.");

                return detail::convert_argument_value<T> (n, *it);
        }

        template <typename T>
        T optional_with_default (Arguments &args, names const &n, T const &default_value)
        {
                auto it = find (args, n);
                if (it == args.end())
                        return default_value;
                return detail::convert_argument_value<T> (n, *it);
        }

        template <typename T>
        detail::optional<T> optional (Arguments &args, names const &n)
        {
                auto it = find (args, n);
                if (it == args.end())
                        return {n};
                return {n, detail::convert_argument_value<T> (n, *it)};
        }

        bool flag (Arguments &args, names const &n)
        {
                auto it = find (args, n);
                if (it == args.end())
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



        Arguments
        parse (int argc, char *argv[])
        {
                Arguments ret;
                for (auto s : detail::catenate (argc, argv))
                        ret.push_back (detail::argument(s));
                return ret;
        }
}

#endif // ARGXX_HH_INCLUDED_20120529
