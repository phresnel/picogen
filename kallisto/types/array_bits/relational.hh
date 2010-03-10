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

#ifndef RELATIONAL_HH_INCLUDED_20100309
#define RELATIONAL_HH_INCLUDED_20100309

namespace kallisto {

        #ifdef KALLISTO_IMPLEMENT_RELATION
        # error "someone already defined KALLISTO_IMPLEMENT_RELATION"
        #endif
        #define KALLISTO_IMPLEMENT_RELATION(NAME,SYM)                         \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS, typename RHS                                    \
        > class NAME##_rep {                                                  \
        public:                                                               \
                NAME##_rep(LHS const &lhs, RHS const &rhs)                    \
                : lhs(lhs), rhs(rhs)                                          \
                {}                                                            \
                                                                              \
                bool operator [] (unsigned int i) const {                     \
                        return lhs[i] SYM rhs[i];                             \
                }                                                             \
                                                                              \
        private:                                                              \
                typename expr_ref<LHS>::type lhs;                             \
                typename expr_ref<RHS>::type rhs;                             \
        };                                                                    \
                                                                              \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS_REP, typename RHS_REP>                           \
        inline typename traits::enable_if<                                    \
                has_member_relational_##NAME##_aa<RULES>,                     \
                array<bool, N, RULES,                                         \
                        NAME##_rep<T, N, RULES, LHS_REP, RHS_REP> >           \
        >::type                                                               \
        operator SYM (                                                        \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                typedef NAME##_rep<T, N, RULES, LHS_REP, RHS_REP> Clos;       \
                return array<bool, N, RULES, Clos>(                           \
                        Clos(lhs.rep(), rhs.rep()));                          \
        }                                                                     \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename LHS_REP>                                             \
        inline typename traits::enable_if<                                    \
                has_member_relational_##NAME##_as<RULES>,                     \
                array<bool, N, RULES,                                         \
                        NAME##_rep<T, N, RULES, LHS_REP, scalar_rep<T> > >    \
        >::type                                                               \
        operator SYM (                                                        \
                array<T, N, RULES, LHS_REP> const &lhs,                       \
                T rhs                                                         \
        ) {                                                                   \
                typedef NAME##_rep<T, N, RULES, LHS_REP, scalar_rep<T> > Clos;\
                return array<bool, N, RULES, Clos>(                           \
                        Clos(lhs.rep(), scalar_rep<T>(rhs)));                 \
        }                                                                     \
        template <typename T, unsigned int N,                                 \
                typename RULES,                                               \
                typename RHS_REP>                                             \
        inline typename traits::enable_if<                                    \
                has_member_relational_##NAME##_sa<RULES>,                     \
                array<bool, N, RULES,                                         \
                        NAME##_rep<T, N, RULES, scalar_rep<T>, RHS_REP > >    \
        >::type                                                               \
        operator SYM (                                                        \
                T lhs,                                                        \
                array<T, N, RULES, RHS_REP> const &rhs                        \
        ) {                                                                   \
                typedef NAME##_rep<T, N, RULES, scalar_rep<T>, RHS_REP > Clos;\
                return array<bool, N, RULES, Clos>(                           \
                        Clos(scalar_rep<T>(lhs), rhs.rep()));                 \
        }
        KALLISTO_IMPLEMENT_RELATION(less,<)
        KALLISTO_IMPLEMENT_RELATION(greater,>)
        KALLISTO_IMPLEMENT_RELATION(less_equal,<=)
        KALLISTO_IMPLEMENT_RELATION(greater_equal,>=)
        KALLISTO_IMPLEMENT_RELATION(equal_to,==)
        KALLISTO_IMPLEMENT_RELATION(not_equal_to,!=)
        #undef KALLISTO_IMPLEMENT_RELATION
}
#endif // RELATIONAL_HH_INCLUDED_20100309
