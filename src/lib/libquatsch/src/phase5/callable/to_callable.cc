
#include "DynamicVariant.h"
#include "to_callable.h"
#include "phase3/Defun.h"
#include <stdexcept>
#include <iostream>
#include <numeric>

namespace quatsch { namespace compiler { namespace phase5 {

typedef phase3::Tree    Tree;
typedef phase3::TreePtr TreePtr;

DynamicVariant exec (Tree const &tree, DynamicArguments const &args);



template <typename T>
struct AddOperator {
        T start() const { return 0; }
        T operator () (T lhs, T rhs) const { return lhs + rhs; }
};
template <typename T>
struct SubOperator {
        T start() const { return 0; }
        T operator () (T lhs, T rhs) const { return lhs - rhs; }
};
template <typename T>
struct MulOperator {
        T start() const { return T(1); }
        T operator () (T lhs, T rhs) const { return lhs * rhs; }
};
template <typename T>
struct DivOperator {
        T operator () (T lhs, T rhs) const { return lhs / rhs; }
};



template <template <typename> class Operator>
DynamicVariant accumulate (Tree const &tree, DynamicArguments const &args)
{
        switch (tree.expression_type()) {
        case Typename::Float: {
                float sum = Operator<float>().start();
                for (auto const &op : tree.builtin_args ())
                        sum = Operator<float>() (sum, exec (*op, args).floating());
                return DynamicVariant::Floating(sum);
        }
        case Typename::Integer: {
                int sum = Operator<int>().start();
                for (auto const &op : tree.builtin_args ())
                        sum = Operator<int>() (sum, exec (*op, args).integer());
                return DynamicVariant::Integer(sum);
        }
        };

        throw std::runtime_error("unhandled case in to_callable:accumulate");
}



template <template <typename> class Operator>
DynamicVariant binary_op (Tree const &tree, DynamicArguments const &args)
{
        const auto &builtin_args = tree.builtin_args();
        if (builtin_args.size() != 2)
                throw std::runtime_error("to_callable::binary_op: not enough or "
                                         "too many arguments");
        auto it = builtin_args.begin();
        DynamicVariant const lhs = exec (**it, args);
        DynamicVariant const rhs = exec (**(++it), args);

        switch (lhs.type()) {
        case Typename::Float:
                return DynamicVariant::Floating (Operator<float>()(lhs.floating(),
                                                                   rhs.floating()));
        case Typename::Integer:
                return DynamicVariant::Integer (Operator<int>()(lhs.integer(),
                                                                rhs.integer()));
        }

        throw std::runtime_error ("unhandled typename in to_callable::binary_op");
}



DynamicVariant builtin (Tree const &tree, DynamicArguments const &args)
{
        typedef phase2::Builtin Builtin;

        phase2::Builtin const &b = *tree.builtin();
        switch (b.type) {
        case Builtin::Addition:       return accumulate<AddOperator> (tree, args);
        case Builtin::Subtraction:    return accumulate<SubOperator> (tree, args);
        case Builtin::Multiplication: return accumulate<MulOperator> (tree, args);
        case Builtin::Division:       return binary_op<DivOperator>  (tree, args);
        }
        throw std::runtime_error ("unhandled builtin type in to_callable::builtin");
}



DynamicVariant call (Tree const &tree, DynamicArguments const &args)
{
        phase3::Defun const &def = *tree.call_callee();

        DynamicArguments call_arg_results;
        for (auto const t : tree.call_args()) {
                DynamicVariant const &res = exec (*t, args);
                call_arg_results.push_back (res);
        }

        return exec (*def.body(), call_arg_results);
}



DynamicVariant exec (Tree const &tree, DynamicArguments const &args)
{
        typedef Tree::Type Type;

        switch (tree.type()) {
        case Type::StackRef:     return args[tree.stackref_index()];
        case Type::Integer:      return DynamicVariant::Integer(tree.integer());
        case Type::Floating:     return DynamicVariant::Floating(tree.floating());
        case Type::Call:         return call (tree, args);
        case Type::Instantiation:return tree.instantiation().function(args);
        case Type::Builtin:      return builtin (tree, args);
        }
        throw std::runtime_error ("to_callable::exec(): unsupported tree-type");
}



quatsch_function to_callable (phase3::Program const &prog)
{
        auto ret = [prog](DynamicArguments const &args) -> DynamicVariant {
                return exec(*prog.main(), args);
        };
        return ret;
}

} } }
