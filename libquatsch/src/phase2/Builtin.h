#ifndef BUILTIN_H_INCLUDED_20120227
#define BUILTIN_H_INCLUDED_20120227

#include <memory>
#include <limits>

namespace quatsch { namespace compiler { namespace phase2 {

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

} } }

#endif // BUILTIN_H_INCLUDED_20120227
