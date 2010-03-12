//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef RULESETS_HH_INCLUDED_20100309
#define RULESETS_HH_INCLUDED_20100309

namespace kallisto {

        // Using these rulesets and enable_if enables much nicer error messages.
        // E.g., instead of three pages of template errors when trying to
        // take one's complement on a floating point array, it will simply be
        // printed "no match for 'operator~' in '~foo'" by gcc 4.4.
        // Also, it enables clients to cherry-pick what they need (and get
        // nice err-messages on false usage), e.g. a ColorSpectrum might
        // not have any use for unary-not, even if that's supported by floats.
        struct r_unary_generic {
                enum {
                        unary_negate,
                        unary_plus,
                        unary_not
                };
        };


        struct r_unary_bitwise {
                enum { unary_ones_complement };
        };

        struct r_unary_all
        : r_unary_generic
        , r_unary_bitwise
        {};

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_negate)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_plus)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_not)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_ones_complement)


        struct r_assign_generic {
                typedef void
                        // assign_a, // not optional, see my documentation
                                     // in array<>
                        assign_s,

                        // Note: the postfix variants
                        //  return void by design, as otherwise they would be
                        //  made at least doubly as complex as their prefix-
                        //  counterparts (they would have to save the state
                        //  of the array, increase the array, and return
                        //  the saved state).
                        //
                        unary_prefix_inc, unary_postfix_inc,
                        unary_prefix_dec, unary_postfix_dec,

                        augmented_plus_a,      augmented_plus_s,
                        augmented_minus_a,     augmented_minus_s,
                        augmented_multiplies_a,augmented_multiplies_s,
                        augmented_divides_a,   augmented_divides_s,
                ;
        };

        struct r_assign_all {
                typedef void
                        augmented_mod_a, augmented_mod_s,

                        augmented_shift_left_a, augmented_shift_left_s,
                        augmented_shift_right_a, augmented_shift_right_s,
                        augmented_bitwise_and_a, augmented_bitwise_and_s,
                        augmented_bitwise_xor_a, augmented_bitwise_xor_s,
                        augmented_bitwise_or_a,  augmented_bitwise_or_s
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(assign_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(assign_s)

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_prefix_inc)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_postfix_inc)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_prefix_dec)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(unary_postfix_dec)

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_plus_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_plus_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_minus_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_minus_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_multiplies_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_multiplies_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_divides_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_divides_s)

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_mod_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_mod_s)

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_shift_left_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_shift_left_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_shift_right_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_shift_right_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_and_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_and_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_xor_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_xor_s)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_or_a)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(augmented_bitwise_or_s)


        struct r_additive {
                typedef void
                        plus_aa, plus_as, plus_sa,
                        minus_aa, minus_as, minus_sa
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(plus_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(plus_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(plus_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(minus_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(minus_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(minus_sa)



        struct r_multiplicative_generic {
                typedef void
                        multiplies_aa, multiplies_as, multiplies_sa,
                        divides_aa, divides_as, divides_sa
                ;
        };

        struct r_multiplies_integer {
                typedef void modulus_aa, modulus_as, modulus_sa;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(multiplies_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(multiplies_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(multiplies_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(divides_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(divides_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(divides_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(mod_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(mod_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(mod_sa)



        struct r_multiplicative_all
        : r_multiplicative_generic
        , r_multiplies_integer {};

        struct r_bitwise {
                typedef void
                        shift_left_aa,  shift_left_as,  shift_left_sa,
                        shift_right_aa, shift_right_as, shift_right_sa,
                        bitwise_and_aa, bitwise_and_as, bitwise_and_sa,
                        bitwise_xor_aa, bitwise_xor_as, bitwise_xor_sa,
                        bitwise_or_aa,  bitwise_or_as,  bitwise_or_sa
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_left_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_left_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_left_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_right_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_right_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(shift_right_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_and_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_and_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_and_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_xor_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_xor_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_xor_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_or_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_or_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(bitwise_or_as)

        struct r_math_special_generic {
                typedef void
                        math_min_aa, math_min_sa, math_min_as,
                        math_max_aa, math_max_sa, math_max_as
                ;
        };
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_min_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_min_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_min_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_max_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_max_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_max_as)


        struct r_math_special : r_math_special_generic {
                typedef void
                        // unary
                        math_acos,
                        math_asin,
                        math_atan,
                        math_atan2,
                        math_ceil,
                        math_cos,
                        math_cosh,
                        math_exp,
                        math_fabs,
                        math_floor,
                        math_fmod,
                        math_frexp,
                        math_ldexp,
                        math_log,
                        math_log10,
                        math_modf_aa, math_modf_sa, math_modf_as,
                        math_sin,
                        math_sinh,
                        math_sqrt,
                        math_tan,
                        math_tanh,

                        // binary
                        math_pow_aa, math_pow_sa, math_pow_as
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_acos)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_asin)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_atan)
        //KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_atan2)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_ceil)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_cos)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_cosh)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_exp)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_fabs)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_floor)
        //KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_fmod)
        //KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_frexp)
        //KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_ldexp)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_log)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_log10)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_sin)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_sinh)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_sqrt)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_tan)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_tanh)

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_pow_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_pow_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(math_pow_sa)


        struct r_reduction {
                typedef void
                        reduction_product,
                        reduction_sum,
                        reduction_length,
                        reduction_length_sq,
                        reduction_all,
                        reduction_none,
                        reduction_any
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_product)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_sum)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_length)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_length_sq)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_all)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_none)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(reduction_any)



        struct r_relational {
                typedef void
                        relational_less_aa,
                        relational_less_as,
                        relational_less_sa,
                        relational_greater_aa,
                        relational_greater_as,
                        relational_greater_sa,
                        relational_less_equal_aa,
                        relational_less_equal_as,
                        relational_less_equal_sa,
                        relational_greater_equal_aa,
                        relational_greater_equal_as,
                        relational_greater_equal_sa,
                        relational_not_equal_to_aa,
                        relational_not_equal_to_as,
                        relational_not_equal_to_sa,
                        relational_equal_to_aa,
                        relational_equal_to_as,
                        relational_equal_to_sa
                ;
        };

        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_equal_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_equal_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_less_equal_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_equal_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_equal_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_greater_equal_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_not_equal_to_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_not_equal_to_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_not_equal_to_sa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_equal_to_aa)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_equal_to_as)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(relational_equal_to_sa)

        struct r_logical {
                typedef void
                        logical_and,
                        logical_or
                ;
        };
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(logical_and)
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(logical_or)


        struct r_conditional {
                typedef void flow_if_then_else;
        };
        KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(flow_if_then_else)


        struct r_flow_control : r_relational, r_logical, r_conditional {};


        struct r_integer
        : r_assign_all
        , r_additive
        , r_multiplicative_all
        , r_bitwise
        , r_unary_all
        , r_reduction
        , r_flow_control
        , r_math_special_generic
        {};


        struct r_floating_basic
        : r_additive
        , r_assign_generic
        , r_multiplicative_generic
        , r_unary_generic
        {};

        struct r_floating
        : r_floating_basic
        , r_math_special
        , r_reduction
        , r_flow_control
        {};

        template <typename T> struct r_std : r_integer {};
        template <> struct r_std<float>       : r_floating {};
        template <> struct r_std<double>      : r_std<float> {};
        template <> struct r_std<long double> : r_std<float> {};
}

#endif // RULESETS_HH_INCLUDED_20100309
