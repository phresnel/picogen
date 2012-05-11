#include "resolve_and_verify.h"
#include "Tree.h"
#include "Defun.h"
#include "Constant.h"
#include "Program.h"
#include "Template.h"
#include "ProgramContract.h"
#include "detail/parse_primitive.h"

#include <boost/optional.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace quatsch { namespace compiler { namespace phase3 {

using boost::optional;

class SymbolTable;

TreePtr resolve_tree (std::list<phase3::DefunPtr> const & defuns,
                      std::list<extern_template::TemplatePtr> const &templates,
                      phase2::TreePtr tree,
                      ErrorState &err,
                      SymbolTable &tab);

class Symbol {
public:
        enum Type {
                Argument
        };

        explicit Symbol (Type type, std::string const &sym) :
                type_(type), symbol_(sym) {}
        explicit operator std::string () const { return symbol_; }
        explicit operator Type () const { return type_; }

        std::string name() const { return symbol_; }
        Type type() const { return type_; }

        void set_argument_index (size_t argument_index) {
                argument_index_ = argument_index;
        }
        size_t argument_index() const { return argument_index_; }
        std::string argument_name() const { return symbol_; }
private:
        Type type_;
        std::string symbol_;
        size_t argument_index_;
};
bool operator== (Symbol const &lhs, std::string const& rhs) {
        return static_cast<std::string>(lhs) == rhs;
}
bool operator!= (Symbol const &lhs, std::string const& rhs) {
        return !(lhs == rhs);
}

class SymbolTable {
public:
        SymbolTable            (const SymbolTable &) = delete;
        SymbolTable& operator= (const SymbolTable &) = delete;

        SymbolTable() = default;
        SymbolTable (SymbolTable &&) = default;

        SymbolTable push() const;

        void declare_argument (phase2::Argument    const &arg);
        void declare_constant (phase3::ConstantPtr cptr);

        bool declared(std::string const &str) const;
        bool argument_declared(std::string const &str) const;
        bool constant_declared(std::string const &str) const;
        bool is_hiding (std::string const &str) const;

        size_t argument_index (std::string const &name) const;

        ConstantPtr constant(std::string const &name) const;
        phase2::Argument argument(std::string const &name) const;
private:
        std::list<phase2::Argument> args_;
        std::list<phase3::ConstantPtr> consts_;
};

SymbolTable SymbolTable::push() const
{
        SymbolTable ret;
        ret.consts_ = consts_;
        return ret;
}

void SymbolTable::declare_argument (phase2::Argument const &arg)
{
        if (argument_declared (arg.name()))
                throw std::runtime_error("SymbolTable::declare_argument() called"
                                         " for multiply defined name");
        args_.push_back (arg);
}
void SymbolTable::declare_constant (phase3::ConstantPtr c)
{
        if (constant_declared (c->name()))
                throw std::runtime_error("SymbolTable::declare_constant() called"
                                         " for multiply defined name");
        consts_.push_back (c);
}
bool SymbolTable::constant_declared (std::string const& name) const
{
        for (auto c : consts_) if (c->name() == name) return true;
        return false;
}
bool SymbolTable::argument_declared (std::string const& name) const
{
        for (auto a : args_) if (a.name() == name) return true;
        return false;
}
bool SymbolTable::declared(const std::string &name) const
{
        return argument_declared(name) || constant_declared(name);
}
bool SymbolTable::is_hiding(const std::string &str) const
{
        if (!argument_declared(str))
                throw std::runtime_error("SymbolTable::is_hiding() called for "
                                         "symbol which is not an argument");
        return constant_declared(str);

}
size_t SymbolTable::argument_index (const std::string &name) const
{
        size_t index = 0;
        for (auto a : args_) {
                if (a.name() == name) return index;
                ++index;
        }
        throw std::runtime_error ("SymbolTable::argument_index(...) called for "
                                  "unknown argument");
}
ConstantPtr SymbolTable::constant (std::string const& name) const
{
        for (auto c : consts_) if (c->name() == name) return c;
        throw std::runtime_error ("SymbolTable::constant(\""+name+"\") called, "+
                                  "but \"" + name + "\" does not exist");
}
phase2::Argument SymbolTable::argument (std::string const& name) const
{
        for (auto c : args_) if (c.name() == name) return c;
        throw std::runtime_error ("SymbolTable::argument(\""+name+"\") called, "+
                                  "but \"" + name + "\" does not exist");
}



phase2::DefunPtr find_p2_defun (phase2::Program const &prog,
                                std::string name)
{
        for (auto defun : prog.defuns())
                if (defun->name() == name) return defun;
        return phase2::DefunPtr();
}

phase3::DefunPtr find_p3_defun (std::list<phase3::DefunPtr> const &defuns,
                                std::string name)
{
        for (auto defun : defuns)
                if (defun->name() == name) return defun;
        return phase3::DefunPtr();
}


std::string idfy (extern_template::StaticParameter const &sp) {
        std::string ret = sp.name() + "_";
        switch (sp.type()) {
        case extern_template::StaticType::Float:
                ret += std::to_string (sp.floating());
                break;
        case extern_template::StaticType::Integer:
                ret += std::to_string (sp.integer());
                break;
        case extern_template::StaticType::String:
                ret += sp.string();
                break;
        }
        auto pred = [](char c) {
                // TODO: use regexes, that ascii trick is just temporary
                return !((c>='a' && c<='z') ||
                         (c>='A' && c<='Z') ||
                         (c>='0' && c<='9'));
        };
        std::replace_if (ret.begin(), ret.end(), pred, '_');
        return ret;
}
TreePtr instantiate_template (
        phase2::Tree const &tree,
        std::list<extern_template::TemplatePtr> const &templates,
        std::list<DefunPtr> const &defuns,
        ErrorState &err,
        SymbolTable &tab
) {
        const auto &name = tree.template_call_callee();
        for (auto const& tplp : templates) {
                if (name == tplp->name()) {

                        extern_template::Template      const &tpl = *tplp;
                        extern_template::Instantiation const inst =
                                        tpl.instantiate (tree.template_static_operands());

                        const auto &p2ops = tree.template_call_operands();
                        const auto &metas = inst.arguments_meta;

                        if (p2ops.size() != metas.size()) {
                                err.post_error ("incorrect number of arguments passed to"
                                                " \"" + name + "\", expected " +
                                                std::to_string(inst.arguments_meta.size()),
                                                tree.code_begin(), tree.code_end());
                                return TreePtr();
                        }

                        //auto op_iter = ops.begin(), ops_end = ops.end();
                        auto meta_iter = metas.begin();

                        int forge_index = 0;
                        auto gen_arg_name = [&forge_index] () {
                                return "__tpl_arg_" + std::to_string(forge_index++);
                        };

                        OperandList p3ops;
                        std::list<phase2::Argument> p2arglist;
                        for (auto const &p2op : p2ops) {
                                if (phase3::TreePtr p3op = resolve_tree (defuns, templates,
                                                                         p2op, err, tab))
                                {
                                        if (*meta_iter != p3op->expression_type()) {
                                                err.post_error ("call to '" + name + "': " +
                                                                "passing incompatible arguments",
                                                                p2op->code_begin(), p2op->code_end());
                                        } else {
                                                p3ops.push_back (p3op);
                                                p2arglist.emplace_back (
                                                        gen_arg_name(),
                                                        *meta_iter, tree.code_begin(),
                                                        tree.code_end());
                                        }
                                }
                                ++meta_iter;
                        }

                        std::string forged_name = "__tpl_" + name + "_";
                        for (auto const &sta : tree.template_static_operands()) {
                                forged_name += idfy(sta) + "__";
                        }

                        TreePtr  body = Tree::Instantiation (inst);
                        DefunPtr dptr (new Defun (forged_name,
                                                  p2arglist,
                                                  inst.return_type,
                                                  body));
                        return Tree::Call (dptr, p3ops);
                }
        }
        throw std::runtime_error("unknown template \"" + name + "\"");
        return TreePtr();
}



TreePtr resolve_call (std::list<phase3::DefunPtr> const & defuns,
                      std::list<extern_template::TemplatePtr> const &templates,
                      phase2::TreePtr tree,
                      ErrorState &err,
                      SymbolTable &tab)
{
        phase3::DefunPtr dp = find_p3_defun (defuns, tree->call_callee());
        if (!dp) {
                err.post_error ("unresolved function call to \""
                                + tree->call_callee() + "\"",
                                tree->code_begin(), tree->code_end());
                return TreePtr();
        }
        else {
                phase3::Defun& d = *dp;
                if (d.arg_count() != tree->call_operands().size()) {
                        err.post_error ("function \"" + tree->call_callee() + "\" declared "
                                        "with " + std::to_string(d.arg_count()) + " "
                                        "arguments, but called with " +
                                        std::to_string(tree->call_operands().size()) +
                                        " arguments.",
                                        tree->code_begin(), tree->code_end());
                }
        }

        std::list<phase2::Argument> args = dp->arguments();
        auto arg_iter = args.begin();
        OperandList ops;
        for (phase2::TreePtr p2op : tree->call_operands()) {
                if (phase3::TreePtr p3op = resolve_tree (defuns, templates,
                                                         p2op, err, tab))
                {
                        if (arg_iter->type() != p3op->expression_type()) {
                                err.post_error ("call to '" + dp->name() + "': " +
                                                "passing incompatible arguments",
                                                p2op->code_begin(), p2op->code_end());
                        } else {
                                ops.push_back (p3op);
                        }
                }
                ++arg_iter;
        }

        return Tree::Call (dp, ops);
}

TreePtr resolve_builtin (std::list<DefunPtr> const & defuns,
                         std::list<extern_template::TemplatePtr> const &templates,
                         phase2::TreePtr tree,
                         ErrorState &err,
                         SymbolTable &tab)
{
        phase2::BuiltinPtr bt = tree->builtin();
        OperandList ops;
        for (auto it : tree->builtin_operands ()) {
                TreePtr op = resolve_tree (defuns, templates, it, err, tab);
                if (op) ops.push_back(op);
        }
        if (ops.size() < bt->min_operand_count
            || ops.size() > bt->max_operand_count)
        {
                std::stringstream ss;
                ss << "Builtin operator '" << bt->mnemonic_info << "' used "
                   << "with " << ops.size() << " operand(s), but requires ";

                if (bt->has_exact_operand_count()) {
                        ss << bt->min_operand_count << " operand(s)";
                }
                else if (bt->has_upper_limit()) {
                        ss << "between " << bt->min_operand_count
                           << " and " << bt->max_operand_count << " operand(s)";
                }
                else {
                        ss << "at least " << bt->min_operand_count
                           << " operand(s).";
                }
                err.post_error (ss.str(), tree->code_begin(), tree->code_end());
                return TreePtr();
        }
        return Tree::Builtin (bt, ops);
}


TreePtr resolve_template_call (std::list<extern_template::TemplatePtr> const &templates,
                               std::list<DefunPtr> const &defuns,
                               phase2::TreePtr treeptr,
                               ErrorState &err,
                               SymbolTable &tab)
{
        const phase2::Tree &tree = *treeptr;
        TreePtr d = instantiate_template (tree, templates, defuns, err, tab);
        if (!d) {
                err.post_error ("unresolved function template call to \""
                                + tree.template_call_callee() + "\"",
                                tree.code_begin(), tree.code_end());
        }
        return d;
}



TreePtr resolve_tree (std::list<phase3::DefunPtr> const & defuns,
                      std::list<extern_template::TemplatePtr> const &templates,
                      phase2::TreePtr tree,
                      ErrorState &err,
                      SymbolTable &tab)
{
        using namespace phase2;
        switch (tree->type()) {
        case phase2::Tree::Type::Identifier:
                if (tab.constant_declared (tree->identifier())) {
                        ConstantPtr cptr = tab.constant (tree->identifier());
                        switch (cptr->type()) {
                         case Typename::Integer: return phase3::Tree::Integer (cptr->integer());
                         case Typename::Float: return phase3::Tree::Floating (cptr->floating());
                         default: throw std::runtime_error ("non-implemented constant-type case in resolve_tree()");
                        }
                }
                else if (tab.argument_declared (tree->identifier())) {
                        const std::string& name = tree->identifier();
                        const size_t index      = tab.argument_index(name);
                        return Tree::StackRef (index, tab.argument(name));
                }
                else {
                        err.post_error ("unresolved id \"" + tree->identifier() + "\"",
                                        tree->code_begin(), tree->code_end());
                        return TreePtr();
                }
        case phase2::Tree::Type::Integer:
                return Tree::Integer (tree->integer());
        case phase2::Tree::Type::Floating:
                return Tree::Floating (tree->floating());
        case phase2::Tree::Type::Call:
                return resolve_call (defuns, templates, tree, err, tab);
        case phase2::Tree::Type::TemplateCall:
                return resolve_template_call (templates, defuns, tree, err, tab);
        case phase2::Tree::Type::Builtin:
                return resolve_builtin (defuns, templates, tree, err, tab);
        default:
                throw std::runtime_error("unhandled case in resolve_tree(...)");
        }
        return TreePtr();
}



void resolve_defun (std::list<DefunPtr> const &defuns,
                    std::list<extern_template::TemplatePtr> const &templates,
                    phase2::DefunPtr dptr,
                    ErrorState &err,
                    const SymbolTable &intab,
                    phase3::DefunPtr &out)
{
        phase2::Defun const &defun = *dptr;

        //scoped_push_frame psf (tab);

        SymbolTable tab (intab.push ());

        for (auto arg : defun.arguments()) {
                if (tab.declared (arg.name())) { // TODO: should emit hiding warning instead of erroring
                        err.post_error ("argument or constant \"" + arg.name() +
                                        + "\" already defined.",
                                        arg.code_begin(), arg.code_end()
                                        );
                }
                else tab.declare_argument (arg);
        }
        TreePtr body = resolve_tree (defuns, templates, defun.body(), err, tab);

        if (defun.return_type() != body->expression_type()) {
                err.post_error ("function '" + defun.name() + "' declared with return type "
                                + to_string (defun.return_type()) + ", but expression yields "
                                + to_string (body->expression_type()),
                                defun.code_begin(), defun.code_end());
        }

        out->set_body (body);
}



void resolve_defuns (phase2::Program const &prog,
                     std::list<extern_template::TemplatePtr> const &templates,
                     ErrorState &err, SymbolTable &tab,
                     std::list<DefunPtr> &p3_defuns)
{
        /*for (auto dptr : prog.defuns())
                resolve_defun (prog, dptr, err, tab);*/
        for (auto p3_defun : p3_defuns) {
                phase2::DefunPtr p2_defun = find_p2_defun (prog, p3_defun->name());
                resolve_defun (p3_defuns, templates,
                               p2_defun, err, tab, p3_defun);
        }
}



std::list<phase3::DefunPtr>
forward_declare_defuns (phase2::Program const &prog)
{
        std::list<phase3::DefunPtr> ret;
        for (auto dptr : prog.defuns()) {
                ret.push_back (Defun::ForwardDeclare(dptr->name(),
                                                     dptr->arguments(),
                                                     dptr->return_type()));
        }
        return ret;
}


template <typename L, typename R> struct same_type { enum { value = false }; };
template <typename T> struct same_type<T,T> { enum { value = true }; };

template <typename T>
optional<T>
evalue_constexpr (phase2::Tree const& expr, ErrorState &err)
{
        switch (expr.type())
        {
        case phase2::Tree::Type::Identifier:
                err.post_error ("identifiers are not allowed in constant expressions",
                                expr.code_begin(), expr.code_end());
                break;
        case phase2::Tree::Type::Integer:
                if (!same_type<int,T>::value) {
                        err.post_error("used integer literal for non-integer constant",
                                       expr.code_begin(), expr.code_end());
                } else {
                        return expr.integer();
                }
        case phase2::Tree::Type::Floating: {
                if (!same_type<float,T>::value) {
                        err.post_error("used float literal for non-float constant",
                                       expr.code_begin(), expr.code_end());
                } else {
                        return expr.floating();
                }
        } break;
        case phase2::Tree::Type::Call:
        case phase2::Tree::Type::TemplateCall:
                err.post_error ("calls are not allowed in constant expressions",
                                expr.code_begin(), expr.code_end());
                break;
        case phase2::Tree::Type::Builtin:
                throw std::runtime_error ("builtin expressions currently not supported within constant expressions");
        default:
                throw std::runtime_error ("unhandled type in 'T evaluate_constexpr()'");
        }

        return optional<T>();
}

ConstantPtr evaluate_constant (phase2::Constant const &c, ErrorState &err)
{
        ConstantPtr ret;
        switch (c.type()) {
        case phase2::Constant::Type::Integer: {
                auto e = evalue_constexpr<int> (*c.expression(), err);
                if (e) return Constant::Integer (c.name(), *e);
                break;
        }
        case phase2::Constant::Type::Floating:{
                auto e = evalue_constexpr<float> (*c.expression(), err);
                if (e) return Constant::Floating (c.name(), *e);
                break;
        }
        default: throw std::runtime_error ("unhandled type in " __FILE__ ":"
                                           "evalue_constant()");
        }

        return ConstantPtr();
}


void evaluate_constants (phase2::Program const &prog, SymbolTable &tab,
                         ErrorState &err)
{
        for (auto cptr : prog.constants()) {
                if (auto c = evaluate_constant (*cptr, err)) {
                        if (tab.constant_declared (c->name())) {
                                err.post_error("constant \"" + c->name()
                                               + "\" defined multiple times",
                                               cptr->code_begin(), cptr->code_end());
                        } else {
                                tab.declare_constant (c);
                        }
                }
                else {
                        err.post_error ("constant expression for '" +
                                        cptr->name() + "' can't be evaluated",
                                        cptr->code_begin(), cptr->code_end());
                }
        }
}



void check_for_multiple_defuns (phase2::Program const &prog, ErrorState &err)
{
        std::set<std::string> names;
        for (auto dptr : prog.defuns()) {
                if (names.find (dptr->name()) != names.end())
                        err.post_error("function \"" + dptr->name()
                                       + "\" defined multiple times",
                                       dptr->code_begin(), dptr->code_end());
                names.insert (dptr->name());
        }
}


ProgramPtr resolve_and_verify (
        phase2::Program const &prog,
        std::list<extern_template::TemplatePtr> const &templates,
        ProgramContract const &ptype,
        ErrorState &err)
{
        SymbolTable tab;

        for (auto const &arg : ptype.arguments) {
                tab.declare_argument (phase2::Argument (arg.name, arg.type,
                                                        prog.main()->code_begin(),
                                                        prog.main()->code_end()));
        }

        check_for_multiple_defuns (prog, err);
        evaluate_constants (prog, tab, err);
        std::list<DefunPtr> defuns = forward_declare_defuns (prog);
        resolve_defuns (prog, templates, err, tab, defuns);
        TreePtr main = resolve_tree (defuns, templates, prog.main(), err, tab);

        if (err.has_errors()) return ProgramPtr();
        return ProgramPtr (new Program (main, defuns));
}

} } }
