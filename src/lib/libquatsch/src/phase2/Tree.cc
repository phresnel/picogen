#include "Tree.h"
#include "Constant.h"

#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase2 {

Tree::Tree (code_iterator from, code_iterator to)
        : from_(from), to_(to)
{
}

TreePtr Tree::Identifier (code_iterator from, code_iterator to, std::string id)
{
        TreePtr ret{new Tree (from, to)};
        ret->type_ = Type::Identifier;
        ret->identifier_ = id;
        return ret;
}

TreePtr Tree::Integer (code_iterator from, code_iterator to, int value)
{
        TreePtr ret (new Tree(from, to));
        ret->type_ = Type::Integer;
        ret->integer_ = value;
        return ret;
}

TreePtr Tree::Floating (code_iterator from, code_iterator to, float value)
{
        TreePtr ret (new Tree(from, to));
        ret->type_ = Type::Floating;
        ret->floating_ = value;
        return ret;
}

TreePtr Tree::Call(code_iterator from, code_iterator to,
                   std::string callee, std::list<TreePtr> operands)
{
        TreePtr ret{new Tree (from, to)};
        ret->type_ = Type::Call;
        ret->callee_ = callee;
        ret->operands_ = operands;
        return ret;
}

TreePtr Tree::Builtin(code_iterator from, code_iterator to,
                      BuiltinPtr builtin, std::list<TreePtr> operands)
{
        TreePtr ret{new Tree(from, to)};
        ret->type_ = Type::Builtin;
        ret->operands_ = operands;
        ret->builtin_ = builtin;
        return ret;
}

TreePtr Tree::TemplateCall(code_iterator from, code_iterator to,
                           std::string callee,
                           std::list<extern_template::StaticParameter> const& targs,
                           std::list<TreePtr> operands)
{
        TreePtr ret{new Tree(from, to)};
        ret->type_ = Type::TemplateCall;
        ret->callee_ = callee;
        ret->operands_ = operands;
        ret->template_args_ = targs;
        return ret;
}



code_iterator Tree::code_begin() const
{
        return from_;
}
code_iterator Tree::code_end  () const
{
        return to_;
}

void Tree::assert_type (Type type, const char *message) const
{
        if (this->type() != type)
                throw std::runtime_error (message);
}

// Accessors.
Tree::Type Tree::type() const
{
        return type_;
}


std::string Tree::identifier() const
{
        assert_type (Type::Identifier, "Called Tree::identifier() for non-"
                                       "identifier.");
        return identifier_;
}

int Tree::integer() const
{
        assert_type (Type::Integer, "Called Tree::integer() for non-integer");
        return integer_;
}

float Tree::floating() const
{
        assert_type (Type::Floating, "Called Tree::floating() for non-float");
        return floating_;
}

std::string Tree::call_callee  () const
{
        assert_type (Type::Call, "Called Tree::call_callee() for non-call");
        return callee_;
}

TreePtrList Tree::call_operands() const
{
        assert_type (Type::Call, "Called Tree::call_operands() for non-call");
        return operands_;
}

std::string Tree::template_call_callee  () const
{
        assert_type (Type::TemplateCall, "Called Tree::template_call_callee()"
                                         " for non-template-call");
        return callee_;
}
TreePtrList Tree::template_call_operands() const
{
        assert_type (Type::TemplateCall, "Called Tree::template_call_operands()"
                                         " for non-template-call");
        return operands_;
}
std::list<extern_template::StaticParameter>
Tree::template_static_operands() const {
        assert_type (Type::TemplateCall, "Called Tree::template_static_operands()"
                                         " for non-template-call");
        return template_args_;
}

BuiltinPtr Tree::builtin () const
{
        assert_type (Type::Builtin, "Called Tree::builtin() for non-builtin");
        return builtin_;
}

TreePtrList Tree::builtin_operands() const
{
        assert_type (Type::Builtin, "Called Tree::builtin_operands() for non-builtin");
        return operands_;
}

} } }
