#ifndef BUILTIN_H_INCLUDED_20120227
#define BUILTIN_H_INCLUDED_20120227

#include <memory>
#include <limits>
#include <string>

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

        bool has_upper_limit()         const ;
        bool has_exact_operand_count() const ;

        const std::string mnemonic_info;

        Builtin() = delete;

protected:
        Builtin (Type type, size_t min_operand_count, size_t max_operand_count,
                 bool commutative, std::string mnemonic_info);
};
typedef std::shared_ptr<Builtin> BuiltinPtr;


struct BuiltinAdd : Builtin {
        BuiltinAdd() : Builtin(Builtin::Addition,
                               1, std::numeric_limits<size_t>::max(), true, "+")
        {}
};
struct BuiltinSub : Builtin {
        BuiltinSub() : Builtin(Builtin::Subtraction,
                               1, std::numeric_limits<size_t>::max(), false, "-")
        {}
};
struct BuiltinMul : Builtin {
        BuiltinMul() : Builtin(Builtin::Multiplication,
                               1, std::numeric_limits<size_t>::max(), true, "*")
        {}
};
struct BuiltinDiv : Builtin {
        BuiltinDiv() : Builtin(Builtin::Division,
                               2, 2, false, "/")
        {}
};

} } }

#endif // BUILTIN_H_INCLUDED_20120227
