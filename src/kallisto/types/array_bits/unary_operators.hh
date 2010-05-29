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

#ifndef UNARY_OPERATORS_HH_INCLUDED_20100309
#define UNARY_OPERATORS_HH_INCLUDED_20100309

namespace kallisto {
        // -- Unary operators -------------------------------------------------
        #ifdef KALLISTO_ARRAY_IMPLEMENT_UNARY
        # error "someone already defined KALLISTO_ARRAY_IMPLEMENT_UNARY"
        #endif

        #define KALLISTO_ARRAY_IMPLEMENT_UNARY(sym, NAME)                     \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename OP>                                                  \
        class unary_##NAME##_rep {                                            \
        public:                                                               \
                unary_##NAME##_rep (OP const &op)                             \
                : op(op) {}                                                   \
                                                                              \
                T operator [] (unsigned int i) const {                        \
                        return sym op[i];                                     \
                }                                                             \
        private:                                                              \
                typename expr_ref<OP>::type op;                               \
        };                                                                    \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename REP>                                                 \
        inline typename traits::enable_if<                                    \
                has_member_unary_##NAME<RULES>,                               \
                array<T, N, RULES, unary_##NAME##_rep<T, N, RULES, REP> >     \
        >::type operator sym (                                                \
                array<T, N, RULES, REP> const &op                             \
        ) {                                                                   \
                return array<T, N, RULES,                                     \
                  unary_##NAME##_rep<T, N, RULES, REP> >                      \
                  (unary_##NAME##_rep<T, N, RULES, REP>(op.rep()));           \
        }

        KALLISTO_ARRAY_IMPLEMENT_UNARY(-, negate)
        KALLISTO_ARRAY_IMPLEMENT_UNARY(+, plus)
        KALLISTO_ARRAY_IMPLEMENT_UNARY(!, not)
        KALLISTO_ARRAY_IMPLEMENT_UNARY(~, ones_complement)

        #undef KALLISTO_ARRAY_IMPLEMENT_UNARY
}

#endif // UNARY_OPERATORS_HH_INCLUDED_20100309
