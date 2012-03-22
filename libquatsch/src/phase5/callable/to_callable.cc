
#include "to_callable.h"
#include <stdexcept>
#include <iostream>

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

DynamicVariant exec (DynamicArguments const &args, Tree const &tree)
{
        typedef Tree::Type Type;

        switch (tree.type()) {
        case Type::StackRef: throw std::runtime_error("unsupported"); break;
        case Type::Integer: return DynamicVariant::Integer(tree.integer());
        case Type::Floating: return DynamicVariant::Floating(tree.floating());
        case Type::Call: break;
        case Type::TemplateCall: break;
        case Type::Instantiation: break;
        case Type::Builtin: break;
        }
        throw std::runtime_error ("to_callable::exec(): unsupported tree-type");
}


extern_function to_callable (phase3::Program const &prog)
{
        using extern_template::DynamicArguments;
        using extern_template::DynamicVariant;

        //Compiler c (prog);
        std::cout << "=> " << exec({DynamicVariant::Floating(0.5),
                                    DynamicVariant::Floating(0.3)},
                                   *prog.main()
                                  ).floating() << std::endl;

        return [] (DynamicArguments const &da) {
                return DynamicVariant::Floating (0.33);
        };
}

} } }
