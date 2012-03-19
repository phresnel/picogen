#include "tokenize.h"
#include "quatsch_except.h"
#include "detail/parse_primitive.h"
#include <boost/optional.hpp>

namespace quatsch { namespace compiler { namespace phase1 {

using boost::optional;

namespace {
        using namespace compiler::detail;

        template <typename Iter>
        Iter eat_whitespace (Iter it, Iter end)
        {
                while (it != end && is_whitespace (*it)) ++it;
                return it;
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

        /*
        template <typename Iter>
        optional<Iter> template_arg (Iter it, Iter end) {
                if (*it != '{') return optional<Iter>();

                while (it != end && *it != '}')
                        ++it;
                if (it == end) throw tokenization_error();
                return ++it;
        }
        */

        struct TemplateStaticArgRet {
                code_iterator end;
                code_iterator val_begin, val_end;
        };

        template <typename Iter>
        optional<TemplateStaticArgRet>
        template_staticarg_integer(Iter it, Iter end)
        {
                if (*it != '{') return optional<TemplateStaticArgRet>();
                it = eat_whitespace (++it, end);
                const code_iterator val_begin = it;

                if (auto v = compiler::detail::integer (it, end)) it = *v;
                else return optional<TemplateStaticArgRet>();

                if (it == end) throw tokenization_error();
                const auto val_end = it;
                it = eat_whitespace (it, end);
                if (*it != '}') return optional<TemplateStaticArgRet>();

                return TemplateStaticArgRet{++it, val_begin, val_end};
        }
        template <typename Iter>
        optional<TemplateStaticArgRet>
        template_staticarg_floating(Iter it, Iter end)
        {
                if (*it != '{') return optional<TemplateStaticArgRet>();
                it = eat_whitespace (++it, end);
                const auto val_begin = it;
                if (auto v = compiler::detail::floating (it, end)) it = *v;
                else return optional<TemplateStaticArgRet>();

                if (it == end) throw tokenization_error();
                const auto val_end = it;
                it = eat_whitespace (it, end);
                if (*it != '}') return optional<TemplateStaticArgRet>();

                return TemplateStaticArgRet{++it, val_begin, val_end};
        }
        template <typename Iter>
        optional<TemplateStaticArgRet>
        template_staticarg_string(Iter it, Iter end)
        {
                if (*it != '{') return optional<TemplateStaticArgRet>();
                ++it;
                const auto val_begin = it;
                auto val_end = it;
                while (it != end && *it != '}') {
                        val_end = ++it;
                }
                if (it == end) throw tokenization_error();
                return TemplateStaticArgRet{++it, val_begin, val_end};
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
                while (compiler::detail::is_whitespace (*it)) ++it;
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
                else if (auto opt = template_staticarg_integer (it, end)) {
                        ret.emplace (opt->val_begin, opt->val_end,
                                     Token::Type::TemplateArg_Integer);
                        it = opt->end;
                }
                else if (auto opt = template_staticarg_floating (it, end)) {
                        ret.emplace (opt->val_begin, opt->val_end,
                                     Token::Type::TemplateArg_Floating);
                        it = opt->end;
                }
                else if (auto opt = template_staticarg_string (it, end)) {
                        ret.emplace (opt->val_begin, opt->val_end,
                                     Token::Type::TemplateArg_String);
                        it = opt->end;
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
