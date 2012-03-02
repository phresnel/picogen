
#include "Tree.h"
#include "Defun.h"
#include <iostream>
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase3 {

void assert_coherent_types (OperandList const &operands,
                            std::string const& message)
{
        if (operands.empty())
                return;
        const Typename etype = operands.front()->expression_type();
        for (auto it=operands.begin(), end=operands.end(); it!=end; ++it) {
                if (etype != (**it).expression_type())
                        throw std::runtime_error(message);
        }
}

void assert_nonempty (OperandList const &operands, std::string const &message)
{
        if (operands.empty())
                throw std::runtime_error (message);
}



TreePtr Tree::StackRef (size_t index, phase2::Argument const& arg)
{
        TreePtr ret {new Tree(Type::StackRef, arg.type())};
        ret->stackvar_index_ = index;
        ret->stackvar_arg_   = arg;
        return ret;
}

TreePtr Tree::Integer (int value)
{
        TreePtr ret {new Tree(Type::Integer, Typename::Integer)};
        ret->integer_value_ = value;
        return ret;
}

TreePtr Tree::Floating (float value)
{
        TreePtr ret {new Tree(Type::Floating, Typename::Float)};
        ret->floating_value_ = value;
        return ret;
}

TreePtr Tree::Builtin (phase2::BuiltinPtr builtin, OperandList operands)
{
        assert_nonempty (operands, "Tree::Builtin() called but list of operands"
                                   " is empty");
        assert_coherent_types (operands, "Tree::Builtin() called with "
                                         "incoherent operand types");
        const auto etype = (**operands.begin()).expression_type();
        TreePtr ret{new Tree (Type::Builtin, etype)};
        ret->builtin_  = builtin;
        ret->operands_ = operands;
        return ret;
}

TreePtr Tree::Call (DefunPtr defun, OperandList operands)
{
        /*assert_coherent_types (operands, "Tree::Call() called with incoherent "
                                         "operand types");*/
        const auto etype = defun->return_type();
        TreePtr ret{new Tree (Type::Call, etype)};
        ret->callee_   = defun;
        ret->operands_ = operands;
        return ret;
}

void Tree::assert_type (Tree::Type type, const char *message) const
{
        if (this->type_ != type)
                throw std::runtime_error (message);
}

Tree::Type Tree::type() const
{
        return type_;
}

Typename Tree::expression_type() const
{
        return expression_type_;
}

phase2::Argument Tree::stackref_argument() const
{
        assert_type (Type::StackRef, "stackref_name() called for non-StackRef");
        return *stackvar_arg_;
}

size_t Tree::stackref_index() const
{
        assert_type (Type::StackRef, "stackref_index() called for non-StackRef");
        return stackvar_index_;
}

int Tree::integer() const
{
        assert_type (Type::Integer, "integer() called for non-integer");
        return integer_value_;
}

float Tree::floating() const
{
        assert_type (Type::Floating, "floating() called for non-float");
        return floating_value_;
}

DefunPtr Tree::call_callee() const
{
        assert_type (Type::Call, "call_defun() called for non-call");
        return callee_;
}

OperandList Tree::call_args() const
{
        assert_type (Type::Call, "call_args() called for non-call");
        return operands_;
}

phase2::BuiltinPtr Tree::builtin() const
{
        assert_type (Type::Builtin, "builtin() called for non-call");
        return builtin_;
}

OperandList Tree::builtin_args() const
{
        assert_type (Type::Builtin, "builtin_args() called for non-call");
        return operands_;
}

} } }
