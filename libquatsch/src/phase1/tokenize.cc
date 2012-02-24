#include "tokenize.h"
#include "quatsch_except.h"
#include <boost/optional.hpp>

namespace quatsch { namespace compiler { namespace phase1 {

using boost::optional;

namespace {
        bool is_whitespace (char c)
        {
                return c==' ' || c=='\n' || c == '\r';
        }

        bool is_alpha (char c) {
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

        bool is_num (char c) {
                switch (c) {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                         return true;
                default: return false;
                }
        }

        bool is_alnum (char c)
        {
                return is_num(c) || is_alpha(c);
        }

        template <typename Iter>
        optional<Iter> identifier (Iter it, Iter end) {
                if (is_alpha (*it) || *it == '_') {
                        while (it != end && (is_alnum (*it)||*it=='_'))
                                ++it;
                        return it;
                }
                return optional<Iter>();
        }
        template <typename Iter>
        optional<Iter> integer (Iter it, Iter end) {
                if (is_num (*it)) {
                        while (it != end && is_num (*it))
                                ++it;
                        if (*it == 'i') return ++it; // must end with 'i'
                }
                return optional<Iter>();
        }

        template <typename Iter>
        optional<Iter> floating (Iter it, Iter end) {
                if (is_num (*it) || *it == '.') { // must begin with number or .
                        while (it != end && is_num (*it))
                                ++it;
                        if (*it == '.') { // fractional is optional
                                ++it;
                                while (it != end && is_num (*it))
                                        ++it;
                        }
                        return it;
                }
                return optional<Iter>();
        }

        template <typename Iter>
        optional<Iter> template_arg (Iter it, Iter end) {
                if (*it != '{') return optional<Iter>();

                while (it != end && *it != '}')
                        ++it;
                if (it == end) throw tokenization_error();
                return ++it;
        }

        template <typename Iter>
        optional<Iter> defun (Iter it, Iter end) {
                auto ret = identifier (it, end);
                if (!ret) return optional<Iter>();

                std::string id;

                auto tmp = it;
                while (tmp != *ret) {
                        id.push_back(*tmp);
                        ++tmp;
                }

                if (id == "defun") return ret;

                return optional<Iter>();
        }

        template <typename Iter>
        optional<Iter> let (Iter it, Iter end) {
                auto ret = identifier (it, end);
                if (!ret) return optional<Iter>();

                std::string id;

                auto tmp = it;
                while (tmp != *ret) {
                        id.push_back(*tmp);
                        ++tmp;
                }

                if (id == "let") return ret;

                return optional<Iter>();
        }
}

Toque tokenize (code_iterator it, code_iterator end)
{
        Toque ret;
        while (it != end) {
                while (is_whitespace (*it)) ++it;
                if (it == end) break;

                auto inc_it = it;
                ++inc_it;

                if (*it == '(') {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::OpenParen);
                }
                else if (*it == ')')  {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::CloseParen);
                }
                else if (*it == '[')  {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::OpenBracket);
                }
                else if (*it == ']')  {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::CloseBracket);
                }
                else if (*it == ':')  {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::Colon);
                }
                else if (*it == '*'
                      || *it == '/'
                      || *it == '+'
                      || *it == '-')
                {
                        auto prev = it;
                        ++it;
                        ret.emplace (prev, it, Token::Type::Operator);
                }
                else if (auto opt = template_arg (it, end)) {
                        ret.emplace (it, *opt, Token::Type::TemplateArg);
                        it = *opt;
                }
                else if (auto opt = defun (it, end)) {
                        ret.emplace (it, *opt, Token::Type::Defun);
                        it = *opt;
                }
                else if (auto opt = let (it, end)) {
                        ret.emplace (it, *opt, Token::Type::Let);
                        it = *opt;
                }
                else if (auto opt = identifier (it, end)) {
                        ret.emplace (it, *opt, Token::Type::Identifier);
                        it = *opt;
                }
                else if (auto opt = integer (it, end)) {
                        ret.emplace (it, *opt, Token::Type::Integer);
                        it = *opt;
                }
                else if (auto opt = floating (it, end)) {
                        ret.emplace (it, *opt, Token::Type::Floating);
                        it = *opt;
                }
                else throw tokenization_error();
        }
        return ret;
}

} } }
