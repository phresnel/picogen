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

#ifndef BINARY_OPERATORS_HH_INCLUDED_20100309
#define BINARY_OPERATORS_HH_INCLUDED_20100309

namespace kallisto {
        // -- Binary operators ------------------------------------------------
        #ifdef KALLISTO_ARRAY_IMPLEMENT_BINARY
        # error "someone has already defined KALLISTO_ARRAY_IMPLEMENT_BINARY"
        #endif

        #define KALLISTO_ARRAY_IMPLEMENT_BINARY(sym, NAME)                    \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS, typename RHS>                                   \
        class NAME##_rep {                                                    \
        public:                                                               \
                NAME##_rep (LHS const &lhs, RHS const &rhs)                   \
                : lhs(lhs), rhs(rhs) {}                                       \
                                                                              \
                T operator [] (unsigned int i) const {                        \
                        return lhs[i] sym rhs[i];                             \
                }                                                             \
        private:                                                              \
                typename expr_ref<LHS>::type lhs;                             \
                typename expr_ref<RHS>::type rhs;                             \
        };                                                                    \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS_REP, typename RHS_REP>                           \
        inline typename traits::enable_if<                                    \
                has_member_##NAME##_aa<RULES>,                                \
                array<T, N, RULES, NAME##_rep<T, N, RULES, LHS_REP, RHS_REP> >\
        >::type operator sym (                                                \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                return array<T, N, RULES,                                     \
                  NAME##_rep<T, N, RULES, LHS_REP, RHS_REP> >                 \
                  (NAME##_rep<T, N, RULES, LHS_REP, RHS_REP>(                 \
                        lhs.rep(), rhs.rep()));                               \
        }                                                                     \
        template <typename T, unsigned int N,                                 \
                typename RULES, typename LHS_REP>                             \
        inline typename traits::enable_if<                                    \
                has_member_##NAME##_as<RULES>,                                \
                array<T, N, RULES,                                            \
                        NAME##_rep<T, N, RULES, LHS_REP, scalar_rep<T> > >    \
        >::type operator sym (                                                \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                T rhs                                                         \
        ) {                                                                   \
                return array<T, N, RULES,                                     \
                  NAME##_rep<T, N, RULES, LHS_REP, scalar_rep<T> > >          \
                  (NAME##_rep<T, N, RULES, LHS_REP, scalar_rep<T> >(          \
                                lhs.rep(), scalar_rep<T>(rhs)));              \
        }                                                                     \
        template <typename T, unsigned int N,                                 \
                typename RULES, typename RHS_REP>                             \
        inline typename traits::enable_if<                                    \
                has_member_##NAME##_sa<RULES>,                                \
                array<T, N, RULES,                                            \
                        NAME##_rep<T, N, RULES, scalar_rep<T>, RHS_REP > >    \
        >::type operator sym (                                                \
                T lhs,                                                        \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                return array<T, N, RULES,                                     \
                  NAME##_rep<T, N, RULES, scalar_rep<T>, RHS_REP > >          \
                  (NAME##_rep<T, N, RULES, scalar_rep<T>, RHS_REP>(           \
                        scalar_rep<T>(lhs), rhs.rep()));                      \
        }


        KALLISTO_ARRAY_IMPLEMENT_BINARY(+, plus)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(-, minus)

        KALLISTO_ARRAY_IMPLEMENT_BINARY(*, multiplies)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(/, divides)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(%, mod)

        KALLISTO_ARRAY_IMPLEMENT_BINARY(<<, shift_left)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(>>, shift_right)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(&, bitwise_and)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(^, bitwise_xor)
        KALLISTO_ARRAY_IMPLEMENT_BINARY(|, bitwise_or)

        #undef KALLISTO_ARRAY_IMPLEMENT_BINARY
}

#endif // BINARY_OPERATORS_HH_INCLUDED_20100309