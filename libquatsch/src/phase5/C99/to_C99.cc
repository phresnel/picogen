#include "to_C99.h"
#include "phase3/Defun.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <stdexcept>

namespace quatsch { namespace compiler { namespace phase5 {

using phase3::Program;
void tree (phase3::Tree const &, std::ostream &);

std::string to_string (Typename t)
{
        switch (t) {
        case Typename::Integer: return "int";
        case Typename::Float: return "float";
        }
        throw std::runtime_error("unhandled case in to_C99.cc:to_string(Typename)");
}

void argument (phase2::Argument const &arg, std::ostream &os)
{
        os << to_string(arg.type()) << " " << arg.name();
}

void defun_header (phase3::Defun const &d, std::ostream &os)
{
        os << to_string (d.return_type()) << " " << d.name() << " (";

        auto args = d.arguments();
        auto it = args.begin(), end = args.end();
        if (it != args.end()) argument (*it, os);
        ++it;
        for (; it!=end; ++it) {
                os << ", ";
                argument (*it, os);
        }
        os << ")";
}

void declarations (Program const &program, std::ostream &os)
{
        os << "// Function declarations.\n";
        for (phase3::DefunPtr const &dptr : program.defuns()) {
                defun_header (*dptr, os);
                os << ";\n";
        }
        os << "\n";
}

void call (phase3::Tree const &tree_, std::ostream &os)
{
        phase3::Defun const &callee = *tree_.call_callee();
        os << callee.name() << " (";

        phase3::OperandList const& call_args = tree_.call_args();
        auto it = call_args.begin(),
            end = call_args.end();

        if (it != end) {
                tree (**it, os);
                ++it;
        }
        for (; it!=end; ++it) {
                os << ", ";
                tree (**it, os);
        }
        os << ")";
}

void infix_builtin (phase3::OperandList const& ops,
                    std::string const& op,
                    std::ostream &os)
{
        auto it = ops.begin(),
            end = ops.end();

        os << "(";
        if (it != end) {
                tree (**it, os);
                ++it;
        }
        for (; it!=end; ++it) {
                os << ' ' << op << ' ';
                tree (**it, os);
        }
        os << ")";
}

void builtin (phase3::Tree const &tree_, std::ostream &os)
{
        phase2::Builtin const& builtin = *tree_.builtin();
        phase3::OperandList const& operands = tree_.builtin_args();

        std::string op;
        switch (builtin.type)
        {
        case phase2::Builtin::Addition: infix_builtin(operands, "+", os); break;
        case phase2::Builtin::Subtraction: infix_builtin(operands, "-", os); break;
        case phase2::Builtin::Multiplication: infix_builtin(operands, "*", os); break;
        case phase2::Builtin::Division: infix_builtin(operands, "/", os); break;
        default: throw std::runtime_error("unsupported builtin in C99-backend's"
                                          " builtin");
        }
}

void floating (float value, std::ostream &os)
{
        os << boost::lexical_cast<std::string> (value) << 'f';
}

void tree (phase3::Tree const &tree, std::ostream &os)
{
        typedef phase3::Tree::Type Type;
        switch (tree.type()) {
        case Type::StackRef:  os << tree.stackref_argument().name(); break;
        case Type::Integer:   os << tree.integer(); break;
        case Type::Floating:  floating(tree.floating(), os); break;
        case Type::Call:      call(tree, os); break;
        case Type::TemplateCall: break;
        case Type::Builtin: builtin (tree, os); break;
        default: throw std::runtime_error (std::string("unsupported case '") +
                                           std::to_string((int)tree.type()) +
                                           "' in to_C99::tree()");
        }
}

void definitions (Program const &program, std::ostream &os)
{
        os << "// Function definitions.\n";
        for (phase3::DefunPtr const &dptr : program.defuns()) {
                defun_header (*dptr, os);
                os << "\n{\n    return ";
                tree (*dptr->body(), os);
                os << ";\n";
                os << "}\n\n";
        }
}

void main_ (Program const &program, std::ostream &os)
{
        os << "// Main function.\n"
           << to_string(program.main()->expression_type())
           << " f (float x, float z)\n{\n"
           << "    return ";
        tree (*program.main(), os);
        os << ";\n";
        os << "}\n";

}

void includes (Program const &, std::ostream &os)
{
        os << "// Includes.\n"
           << "#include <math.h>\n"
           << "\n";
}

std::string to_C99 (Program const &program)
{
        std::stringstream ss;
        includes     (program, ss);
        declarations (program, ss);
        definitions  (program, ss);
        main_        (program, ss);
        return ss.str();
}

} } }
