#ifndef PARSE_PRIMITIVE_H_20120309
#define PARSE_PRIMITIVE_H_20120309

#include <boost/optional.hpp>

namespace quatsch { namespace compiler { namespace detail {

        using boost::optional;


        inline bool is_whitespace (char c)
        {
                return c==' ' || c=='\n' || c == '\r';
        }

        inline bool is_alpha (char c) {
                switch (c) {
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                case 'Y': case 'Z':
                         return true;
                default: return false;
                }
        }

        inline bool is_num (char c) {
                switch (c) {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                         return true;
                default: return false;
                }
        }

        inline bool is_alnum (char c)
        {
                return is_num(c) || is_alpha(c);
        }

        template <typename Iter>
        inline optional<Iter> integer (Iter it, Iter end, bool full=false) {
                if (is_num (*it)) {
                        while (it != end && is_num (*it))
                                ++it;
                        if (*it == 'i') {
                                if (!full || it==end)
                                        return ++it; // must end with 'i'
                        }
                }
                return optional<Iter>();
        }

        template <typename Iter>
        inline optional<Iter> floating (Iter it, Iter end, bool full=false) {
                if (is_num (*it) || *it == '.') { // must begin with number or .
                        while (it != end && is_num (*it))
                                ++it;
                        if (*it == '.') { // fractional is optional
                                ++it;
                                while (it != end && is_num (*it))
                                        ++it;
                        }
                        if (!full || it==end)
                                return it;
                }
                return optional<Iter>();
        }

} } }

#endif// PARSE_PRIMITIVE_H_20120309
