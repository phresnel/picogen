#ifndef TREE_H_INCLUDED_20120125
#define TREE_H_INCLUDED_20120125

#include "Typename.h"
#include "code_iterator.h"
#include <memory>
#include <list>
#include <limits>

namespace quatsch { namespace compiler { namespace phase2 {

class Tree;
typedef std::shared_ptr<Tree> TreePtr;
typedef std::list<TreePtr> TreePtrList;

struct Builtin {
        enum Type {
                Addition,
                Subtraction,
                Multiplication,
                Division
        };
        const Type type;
        const size_t min_operand_count;
        const size_t max_operand_count;
        const bool commutative;

        Builtin() = delete;
protected:
        Builtin (Type type, size_t min_operand_count, size_t max_operand_count,
                 bool commutative)
                : type(type), min_operand_count(min_operand_count),
                  max_operand_count(max_operand_count), commutative(commutative)
        {}
};
using std::numeric_limits;
struct BuiltinAdd : Builtin {
        BuiltinAdd() : Builtin(Builtin::Addition, 1, numeric_limits<size_t>::max(), true) {}
};
struct BuiltinSub : Builtin {
        BuiltinSub() : Builtin(Builtin::Subtraction, 1, numeric_limits<size_t>::max(), false) {}
};
struct BuiltinMul : Builtin {
        BuiltinMul() : Builtin(Builtin::Multiplication, 1, numeric_limits<size_t>::max(), true) {}
};
struct BuiltinDiv : Builtin {
        BuiltinDiv() : Builtin(Builtin::Division, 2, 2, false) {}
};
typedef std::shared_ptr<Builtin> BuiltinPtr;

std::ostream& operator<< (std::ostream&, Builtin);

class Tree {
public:
        enum class Type {
                Identifier,
                Integer,
                Floating,
                //Constant,
                Call,
                TemplateCall,
                Builtin     // e.g. '*'
        };

        static TreePtr Identifier (code_iterator from, code_iterator to,
                                   std::string id);
        static TreePtr Integer    (code_iterator from, code_iterator to,
                                   int value);
        static TreePtr Floating   (code_iterator from, code_iterator to,
                                   float value);
        //static TreePtr Constant   (code_iterator from, code_iterator to,
        //                           std::string const &name, Constant const &c);
        static TreePtr Call       (code_iterator from, code_iterator to,
                                   std::string callee,
                                   std::list<TreePtr> operands);
        static TreePtr TemplateCall(code_iterator from, code_iterator to,
                                    std::string callee,
                                    std::list<std::pair<std::string,std::string> > const&,
                                    std::list<TreePtr> operands);
        static TreePtr Builtin    (code_iterator from, code_iterator to,
                                   BuiltinPtr builtin, std::list<TreePtr> operands);

        code_iterator code_begin() const;
        code_iterator code_end  () const;

        Type type() const;

        std::string identifier   () const;
        int         integer      () const;
        float       floating     () const;

        std::string call_callee  () const;
        TreePtrList call_operands() const;

        std::string template_call_callee  () const;
        TreePtrList template_call_operands() const;
        std::list<std::pair<std::string,std::string> > template_static_operands() const;

        BuiltinPtr builtin() const;
        TreePtrList builtin_operands() const;

private:
        Tree (code_iterator from, code_iterator to);

        void assert_type (Type type, const char *message) const;
private:
        // Common.
        code_iterator from_, to_;
        Type type_;

        // Specific.
        std::string identifier_;
        int integer_;
        float floating_;

        std::string callee_;
        std::list<TreePtr> operands_;

        phase2::BuiltinPtr builtin_;

        std::list<std::pair<std::string,std::string> > template_args_;

};

} } }

#endif // TREE_H_INCLUDED_20120125
