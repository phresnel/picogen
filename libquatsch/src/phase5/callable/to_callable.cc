
#include "to_callable.h"
#include "phase3/Defun.h"
#include <stdexcept>
#include <iostream>
#include <numeric>

namespace quatsch { namespace compiler { namespace phase5 {

/*
class ExecTree {
public:
        virtual ~ExecTree() {}
};

class Compiler {
        typedef phase3::Tree Tree;

public:
        Compiler (phase3::Program const &prog)
                : prog_(prog)
        {
                main ();
        }

private:

        void main ()
        {
                tree (*prog_.main ());
        }

        void tree (Tree const &tree)
        {
                typedef Tree::Type Type;

                switch (tree.type()) {
                case Type::StackRef: break;
                case Type::Integer: break;
                case Type::Floating: break;
                case Type::Call: break;
                case Type::TemplateCall: break;
                case Type::Instantiation: break;
                case Type::Builtin: break;
                }
        }

private:
        phase3::Program const &prog_;

};*/

typedef phase3::Tree    Tree;
typedef phase3::TreePtr TreePtr;
typedef extern_template::DynamicVariant   DynamicVariant;
typedef extern_template::DynamicArguments DynamicArguments;


DynamicVariant exec (DynamicArguments const &args, Tree const &tree);

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
DynamicVariant accumulate (DynamicArguments const &args, Tree const &tree)
{
        switch (tree.expression_type()) {
        case Typename::Float: {
                float sum = Operator<float>().start();
                for (auto const &op : tree.builtin_args ())
                        sum = Operator<float>() (sum, exec (args, *op).floating());
                return DynamicVariant::Floating(sum);
        }
        case Typename::Integer: {
                int sum = Operator<int>().start();
                for (auto const &op : tree.builtin_args ())
                        sum = Operator<int>() (sum, exec (args, *op).integer());
                return DynamicVariant::Integer(sum);
        }
        };

        throw std::runtime_error("unhandled case in to_callable:accumulate");
}

template <template <typename> class Operator>
DynamicVariant binary_op (DynamicArguments const &args, Tree const &tree)
{
        const auto &builtin_args = tree.builtin_args();
        if (builtin_args.size() != 2)
                throw std::runtime_error("to_callable::binary_op: not enough or "
                                         "too many arguments");
        auto it = builtin_args.begin();
        DynamicVariant const lhs = exec (args, **it);
        DynamicVariant const rhs = exec (args, **(++it));

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

DynamicVariant builtin (DynamicArguments const &args, Tree const &tree)
{
        typedef phase2::Builtin Builtin;

        phase2::Builtin const &b = *tree.builtin();
        switch (b.type) {
        case Builtin::Addition:       return accumulate<AddOperator> (args, tree);
        case Builtin::Subtraction:    return accumulate<SubOperator> (args, tree);
        case Builtin::Multiplication: return accumulate<MulOperator> (args, tree);
        case Builtin::Division:       return binary_op<DivOperator> (args, tree);
        }
        throw std::runtime_error ("unhandled builtin type in to_callable::builtin");
}

DynamicVariant call (DynamicArguments const &args, Tree const &tree)
{
        phase3::Defun const &def = *tree.call_callee();

        DynamicArguments call_arg_results;
        for (auto const t : tree.call_args()) {
                DynamicVariant const &res = exec (args, *t);
                call_arg_results.push_back (res);
        }

        return exec (call_arg_results, *def.body());
}

DynamicVariant exec (DynamicArguments const &args, Tree const &tree)
{
        typedef Tree::Type Type;

        switch (tree.type()) {
        case Type::StackRef: return args[tree.stackref_index()];
        case Type::Integer:  return DynamicVariant::Integer(tree.integer());
        case Type::Floating: return DynamicVariant::Floating(tree.floating());
        case Type::Call:     return call (args, tree);
        case Type::TemplateCall:  break;
        case Type::Instantiation: break;
        case Type::Builtin:  return builtin (args, tree);
        }
        throw std::runtime_error ("to_callable::exec(): unsupported tree-type");
}

extern_function to_callable (phase3::Program const &prog)
{
        using extern_template::DynamicArguments;
        using extern_template::DynamicVariant;

        //Compiler c (prog);
        DynamicVariant result = exec({DynamicVariant::Floating(0.5),
                                      DynamicVariant::Floating(0.3)},
                                     *prog.main()
                                    );

        switch (result.type()) {
        case Typename::Integer: std::cout << "=> " << result.integer() << '\n'; break;
        case Typename::Float: std::cout << "=> " << result.floating()  << '\n'; break;
        };

        return [] (DynamicArguments const &da) {
                return DynamicVariant::Floating (0.33);
        };
}

} } }
