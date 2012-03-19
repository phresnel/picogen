#include "parse.h"
#include "quatsch_except.h"
#include "Template.h"
#include "detail/parse_primitive.h"
#include <iostream>
#include <list>
#include <boost/optional.hpp>

namespace quatsch { namespace compiler { namespace phase2 {

using boost::optional;

bool try_expression (phase1::Toque &, phase2::TreePtr &);


bool try_scalar_expression (phase1::Toque &toque, phase2::TreePtr &out)
{
        if (toque.empty())
                return false;

        if (toque.front() == phase1::Token::Type::Identifier) {
                phase1::Token const &tok = toque.front();
                out = Tree::Identifier(tok.begin(), tok.end(),
                                       tok.value<std::string>());
                toque.pop();
                return true;
        }
        else if (toque.front() == phase1::Token::Type::Integer) {
                phase1::Token const &tok = toque.front();
                out = Tree::Integer(tok.begin(), tok.end(),
                                    tok.value<int>());
                toque.pop();
                return true;
        }
        else if (toque.front() == phase1::Token::Type::Floating) {
                phase1::Token const &tok = toque.front();
                out = Tree::Floating(tok.begin(), tok.end(),
                                     tok.value<float>());
                toque.pop();
                return true;
        }

        return false;
}

std::list<TreePtr> expression_list (phase1::Toque &toque)
{
        std::list<TreePtr> exprs;
        TreePtr expr;
        while (try_expression(toque, expr))
                exprs.push_back(expr);
        return exprs;
}


bool try_builtin (phase1::Toque &toque, TreePtr &out)
{
        if (!toque.is_seq({phase1::Token::OpenParen,
                           phase1::Token::Operator}))
                return false;

        toque.pop();
        const phase1::Token tok = toque.front();
        const std::string   op  = tok.value<std::string>();
        toque.pop();

        const auto args = expression_list (toque);
        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        BuiltinPtr b;
        if (op == "+") b.reset (new BuiltinAdd);
        else if (op == "-") b.reset (new BuiltinSub);
        else if (op == "*") b.reset (new BuiltinMul);
        else if (op == "/") b.reset (new BuiltinDiv);

        out = Tree::Builtin(tok.begin(), args.back()->code_end(), b, args);
        return true;
}

bool try_call (phase1::Toque &toque, TreePtr &out)
{
        if (!toque.is_seq({phase1::Token::OpenParen,
                           phase1::Token::Identifier}))
                return false;

        toque.pop();
        const phase1::Token tok = toque.front();
        const std::string   id  = tok.value<std::string>();
        toque.pop();

        const auto args = expression_list (toque);
        out = Tree::Call(tok.begin(), args.back()->code_end(), id, args);

        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        return true;
}

optional<extern_template::StaticParameter>
try_template_call_arg (phase1::Toque &toque)
{
        using namespace extern_template;

        if (!toque.is_seq(phase1::Token::Identifier))
                return optional<StaticParameter>();

        auto const peeked = toque.peek(1);
        if (!peeked) return optional<StaticParameter>();

        if (*peeked != phase1::Token::TemplateArg_Integer
         && *peeked != phase1::Token::TemplateArg_Floating
         && *peeked != phase1::Token::TemplateArg_String)
                return optional<StaticParameter>();

        // syntax: "foo{bar frob}"
        auto name_tok  = toque.take(),
             value_tok = toque.take();
        const std::string name  (name_tok.begin().str_iter(),
                                 name_tok.end().str_iter());

        if (*peeked == phase1::Token::TemplateArg_Integer) {
                return StaticParameter::Integer (name, value_tok.value<int>());
        }
        if (*peeked == phase1::Token::TemplateArg_Floating) {
                return StaticParameter::Float (name, value_tok.value<float>());
        }
        if (*peeked == phase1::Token::TemplateArg_String) {
                return StaticParameter::String (name, value_tok.value<std::string>());
        }
        throw std::runtime_error ("impossible codepath in try_template_call_arg");
}

bool try_template_call (phase1::Toque &toque, TreePtr &out)
{
        if (!toque.is_seq({phase1::Token::OpenParen,
                           phase1::Token::OpenBracket}))
                return false;

        auto begin = toque.front().begin();

        toque.pop(); if (toque.empty()) throw unexpected();
        toque.pop(); if (toque.empty()) throw unexpected();

        const std::string name = toque.take().value<std::string>();

        // parse static arguments
        std::list<extern_template::StaticParameter> targs;
        while (auto const param = try_template_call_arg (toque)) {
                targs.push_back (*param);
        }
        if (toque.front() != phase1::Token::CloseBracket) {
                throw unexpected();
        }
        toque.pop();
        // --

        const auto args = expression_list (toque);
        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        out = Tree::TemplateCall(begin, args.back()->code_end(), name,
                                 targs, args);
        return true;
}


bool try_expression (phase1::Toque &toque, phase2::TreePtr &out)
{
        if (try_scalar_expression (toque, out) ||
            try_builtin           (toque, out) ||
            try_call              (toque, out) ||
            try_template_call     (toque, out))
                return true;
        return false;
}


TreePtr expression (phase1::Toque &toque)
{
        phase2::TreePtr out;
        if (!try_expression (toque, out))
                throw unexpected();
        return out;
}


Defun::argument_list arg_list (phase1::Toque &toque)
{
        Defun::argument_list ret;

        if (toque.front() != phase1::Token::OpenParen) throw unexpected();
        toque.pop();

        while (!toque.empty() && toque.front() == phase1::Token::Identifier) {
                phase1::Token argtok = toque.front();
                ret.emplace_back(argtok.value<std::string>(),
                                 argtok.begin(), argtok.end());
                auto &arg = ret.back(); // possibly need to re-assign type.
                toque.pop();

                if (toque.is_seq({phase1::Token::Colon,
                                  phase1::Token::Identifier}))
                {
                        toque.pop();
                        phase1::Token typetok = toque.front();
                        std::string typename_ = typetok.value<std::string>();
                        if (typename_ == "int") {
                                arg.reset_type (Typename::Integer);
                        } else if (typename_ == "float") {
                                arg.reset_type (Typename::Float);
                        } else {
                                throw unknown_typename();
                        }
                        toque.pop();
                }
        }

        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        return ret;
}



void defun_impl (phase1::Toque &toque, DefunPtr& dptr)
{
        if (toque.front() != phase1::Token::Identifier) throw unexpected();

        code_iterator const code_begin = toque.front().begin();
        auto name = toque.front().value<std::string>();
        toque.pop();

        Typename rtype = Typename::Float;
        if (toque.is_seq(phase1::Token::Colon,
                         phase1::Token::Identifier))
        {
                toque.pop();
                auto typename_ = toque.front().value<std::string>();
                if (typename_ == "float") rtype = Typename::Float;
                else if (typename_ == "int") rtype = Typename::Integer;
                else throw unexpected();
                toque.pop();
        }

        const Defun::argument_list args = arg_list  (toque);
        const TreePtr              body = expression(toque);

        dptr.reset (new Defun (name, args, rtype, body,
                               code_begin, body->code_end()));
}

bool try_defun (phase1::Toque &toque, DefunPtr &out)
{
        if (!toque.is_seq ({phase1::Token::OpenParen,
                            phase1::Token::Defun}))
                return false;
        toque.pop(2);

        DefunPtr pout;
        defun_impl (toque, pout);

        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        out = pout;
        return true;
}

bool try_let (phase1::Toque &toque, ConstantPtr &out)
{
        if (!toque.is_seq (phase1::Token::OpenParen,
                           phase1::Token::Let,
                           phase1::Token::Identifier))
                return false;
        const code_iterator code_begin = toque.peek(2)->begin();
        const std::string id = toque.peek(2)->value<std::string>();
        toque.pop(3);

        Typename typename_ = Typename::Float;
        if (toque.is_seq(phase1::Token::Colon)) {
                toque.pop();
                if (toque.empty()) throw unexpected_eof();

                const std::string ts = toque.front().value<std::string>();
                toque.pop();

                     if (ts == "int")   typename_ = Typename::Integer;
                else if (ts == "float") typename_ = Typename::Float;
                else throw unexpected();
        }

        if (toque.empty()) throw unexpected_eof();

        TreePtr expr;
        if (!try_expression (toque, expr)) {
                throw constexpr_expected();
        }

        if (toque.empty()) throw unexpected_eof();

        const code_iterator code_end = toque.front().end();
        ConstantPtr ret;
        switch (typename_) {
        case Typename::Integer:
                ret = Constant::Integer (id, expr, code_begin, code_end);
                break;
        case Typename::Float:
                ret = Constant::Floating (id, expr, code_begin, code_end);
                break;
        default:
                throw unknown_typename();
        }

        if (toque.front() != phase1::Token::CloseParen) throw unexpected();
        toque.pop();

        out = ret;
        return true;

#if 0
        phase1::Token valtok = toque.front();
        toque.pop();
;
        switch (typename_) {
        case Typename::Integer:
                if (valtok.type() != phase1::Token::Integer) {
                        throw incompatible_constexpr();
                }
                if (toque.front() != phase1::Token::CloseParen) throw unexpected();
                toque.pop();
                out = Constant::Integer (id, valtok.value<int>());
                return true;
        case Typename::Float:
                if (valtok.type() != phase1::Token::Floating) {} // TODO: emit a warning
                if (toque.front() != phase1::Token::CloseParen) throw unexpected();
                toque.pop();
                out = Constant::Floating (id,   valtok.value<float>());
                return true;
        default: throw std::runtime_error("unhandled typename_ in parse.cc:try_let()");
        }
#endif
}

bool try_main (phase1::Toque &toque, TreePtr &out)
{
        return try_expression (toque, out);
}


void program (phase1::Toque &toque, ProgramPtr &prog)
{

        std::list<DefunPtr> defuns;
        std::list<ConstantPtr> constants;
        TreePtr main;
        bool has_main = false;

        while (!toque.empty()) {
                DefunPtr dptr;
                ConstantPtr cptr;
                if (try_defun (toque, dptr)) {
                        defuns.push_back (dptr);
                }
                else if (try_let (toque, cptr)) {
                        constants.push_back (cptr);
                }
                else if (try_main (toque, main)) {
                        if (has_main)
                                throw multiple_entry_points();
                        has_main = true;
                }
                else {
                        throw unexpected();
                }
        }

        if (!has_main)
                throw missing_entry_point();

        prog.reset (new Program (main, defuns, constants));
}


ProgramPtr parse (phase1::Toque const &toq_)
{
        ProgramPtr ret;

        phase1::Toque toq = toq_;
        program (toq, ret);

        return ret;
}

} } }
