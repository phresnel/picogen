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

#ifndef CMATH_HH_INCLUDED_20100309
#define CMATH_HH_INCLUDED_20100309

#include <cmath>
namespace kallisto {
        // -- math/special functions ------------------------------------------
        #ifdef KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN
        # error "someone already defined KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN"
        #endif

        #define KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(NAME)                      \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename OP>                                                  \
        class math_##NAME##_rep {                                             \
        public:                                                               \
                math_##NAME##_rep (OP const &op)                              \
                : op(op) {}                                                   \
                                                                              \
                T operator [] (unsigned int i) const {                        \
                        return std::NAME(op[i]);                              \
                }                                                             \
        private:                                                              \
                typename expr_ref<OP>::type op;                               \
        };                                                                    \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename REP>                                                 \
        inline typename traits::enable_if<                                    \
                has_member_math_##NAME<RULES>,                                \
                array<T, N, RULES, math_##NAME##_rep<T, N, RULES, REP> >      \
        >::type NAME (                                                        \
                array<T, N, RULES, REP> const &op                             \
        ) {                                                                   \
                typedef math_##NAME##_rep<T, N, RULES, REP> MathClos;         \
                return array<T, N, RULES, MathClos> (MathClos(op.rep()));     \
        }

        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(acos)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(asin)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(atan)
        //KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(atan2)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(ceil)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(cos)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(cosh)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(exp)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(fabs)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(floor)
        //KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(fmod)
        //KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(frexp)
        //KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(ldexp)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(log)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(log10)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(sin)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(sinh)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(sqrt)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(tan)
        KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN(tanh)

        #undef KALLISTO_ARRAY_IMPLEMENT_UNARY_FUN



        #ifdef KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN
        # error "someone already defined KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN"
        #endif

        #define KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN(NAME)                     \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS, typename RHS>                                   \
        class math_##NAME##_rep {                                             \
        public:                                                               \
                math_##NAME##_rep (LHS const &lhs, RHS const &rhs)            \
                : lhs(lhs), rhs(rhs) {}                                       \
                                                                              \
                T operator [] (unsigned int i) const {                        \
                        return std::NAME(lhs[i], rhs[i]);                     \
                }                                                             \
        private:                                                              \
                typename expr_ref<LHS>::type lhs;                             \
                typename expr_ref<RHS>::type rhs;                             \
        };                                                                    \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS_REP, typename RHS_REP>                           \
        inline typename traits::enable_if<                                    \
                has_member_math_##NAME##_aa<RULES>,                           \
                array<T, N, RULES,                                            \
                        math_##NAME##_rep<T,N,RULES,LHS_REP,RHS_REP> >        \
        >::type NAME (                                                        \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                typedef math_##NAME##_rep<T, N, RULES, LHS_REP,RHS_REP>       \
                        MathClos;                                             \
                return array<T, N, RULES, MathClos> (                         \
                        MathClos(lhs.rep(), rhs.rep()));                      \
        }                                                                     \
                                                                              \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS_REP>                                             \
        inline typename traits::enable_if<                                    \
                has_member_math_##NAME##_as<RULES>,                           \
                array<T, N, RULES,                                            \
                  math_##NAME##_rep<T,N,RULES,LHS_REP,scalar_rep<T> > >       \
        >::type NAME (                                                        \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                T rhs                                                         \
        ) {                                                                   \
                typedef math_##NAME##_rep<T, N, RULES,                        \
                                LHS_REP, scalar_rep<T> >                      \
                                MathClos;                                     \
                return array<T, N, RULES, MathClos> (                         \
                        MathClos(lhs.rep(), scalar_rep<T>(rhs)));             \
        }                                                                     \
                                                                              \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename RHS_REP>                                             \
        inline typename traits::enable_if<                                    \
                has_member_math_##NAME##_sa<RULES>,                           \
                array<T, N, RULES,                                            \
                  math_##NAME##_rep<T,N,RULES,scalar_rep<T>,RHS_REP> >        \
        >::type NAME (                                                        \
                T lhs,                                                        \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                typedef math_##NAME##_rep<T, N, RULES,                        \
                        scalar_rep<T>,RHS_REP>                                \
                        MathClos;                                             \
                return array<T, N, RULES, MathClos> (                         \
                        MathClos(lhs, rhs.rep()));                            \
        }

        //KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN(modf)
        KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN(pow)

        #undef KALLISTO_ARRAY_IMPLEMENT_BINARY_FUN
}

#endif // CMATH_HH_INCLUDED_20100309
