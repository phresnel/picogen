#include "Builtin.h"

namespace quatsch { namespace compiler { namespace phase2 {

Builtin::Builtin (Type type,
                  size_t min_operand_count, size_t max_operand_count,
                  bool commutative, std::string mnemonic_info)
        : type(type), min_operand_count(min_operand_count),
          max_operand_count(max_operand_count), commutative(commutative),
          mnemonic_info(mnemonic_info)
{}

bool Builtin::has_upper_limit() const
{
        return max_operand_count < std::numeric_limits<size_t>::max();
}

bool Builtin::has_exact_operand_count() const
{
        return min_operand_count == max_operand_count;
}

} } }
