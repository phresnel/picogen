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

#ifndef IF_THEN_ELSE_HH_INCLUDED_20100309
#define IF_THEN_ELSE_HH_INCLUDED_20100309

namespace kallisto {



        #ifdef KALLISTO_IMPLEMENT_LOGICAL
        # error "KALLISTO_IMPLEMENT_LOGICAL already defined"
        #endif
        #define KALLISTO_IMPLEMENT_LOGICAL(NAME,SYM)                          \
        template <typename T,                                                 \
                unsigned int N,                                               \
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
        template <unsigned int N,                                             \
                typename RULES,                                               \
                typename LHS_REP, typename RHS_REP>                           \
        inline typename traits::enable_if<                                    \
                has_member_##NAME<RULES>,                                     \
                array<bool, N, RULES,                                         \
                        NAME##_rep<bool,N, RULES, LHS_REP, RHS_REP> >         \
        >::type                                                               \
        operator SYM (                                                        \
                array<bool, N, RULES, LHS_REP> const &lhs,                    \
                array<bool, N, RULES, RHS_REP> const &rhs                     \
        ) {                                                                   \
                typedef NAME##_rep<bool, N, RULES, LHS_REP, RHS_REP> Clos;    \
                return array<bool, N, RULES, Clos>(Clos(lhs.rep(),rhs.rep()));\
        }
        KALLISTO_IMPLEMENT_LOGICAL(logical_and,&&)
        KALLISTO_IMPLEMENT_LOGICAL(logical_or,||)
        #undef KALLISTO_IMPLEMENT_LOGICAL


        // --
        template <typename T, unsigned int N,
                typename RULES,
                typename IF, typename THEN, typename ELSE
        > class if_then_else_rep {
        public:
                if_then_else_rep(IF const &if_, THEN const &then_, ELSE const &else_)
                : if_(if_), then_(then_), else_(else_)
                {}

                T operator [] (unsigned int i) const {
                        return if_[i] ? then_[i] : else_[i];
                }

        private:
                typename expr_ref<IF>::type if_;
                typename expr_ref<THEN>::type then_;
                typename expr_ref<ELSE>::type else_;
        };


        // array ? array : array
        template <typename T, unsigned int N,
                typename RULES,
                typename IF, typename THEN, typename ELSE>
        inline typename traits::enable_if<
                has_member_flow_if_then_else<RULES>,
                array<T, N, RULES,
                        if_then_else_rep<T, N, RULES, IF,THEN,ELSE> >
        >::type
        if_then_else (
                array<bool, N, RULES, IF> const &if_,
                array<T, N, RULES, THEN> const &then_,
                array<T, N, RULES, ELSE> const &else_
        ) {
                typedef if_then_else_rep<T, N, RULES, IF, THEN, ELSE> Clos;
                return array<T, N, RULES, Clos>(Clos(if_.rep(), then_.rep(), else_.rep()));
        }

        // array ? scalar : array
        template <typename T, unsigned int N,
                typename RULES,
                typename IF, typename ELSE>
        inline typename traits::enable_if<
                has_member_flow_if_then_else<RULES>,
                array<T, N, RULES,
                        if_then_else_rep<T, N, RULES, IF,scalar_rep<T>,ELSE> >
        >::type
        if_then_else (
                array<bool, N, RULES, IF> const &if_,
                T then_,
                array<T, N, RULES, ELSE> const &else_
        ) {
                typedef if_then_else_rep<T, N, RULES, IF, scalar_rep<T>, ELSE> Clos;
                return array<T, N, RULES, Clos>(
                        Clos(if_.rep(), scalar_rep<T>(then_), else_.rep()));
        }


        // array ? array : scalar
        template <typename T, unsigned int N,
                typename RULES,
                typename IF, typename THEN>
        inline typename traits::enable_if<
                has_member_flow_if_then_else<RULES>,
                array<T, N, RULES,
                        if_then_else_rep<T, N, RULES, IF,THEN,scalar_rep<T> > >
        >::type
        if_then_else (
                array<bool, N, RULES, IF> const &if_,
                array<T, N, RULES, THEN> const &then_,
                T else_
        ) {
                typedef if_then_else_rep<T, N, RULES, IF, THEN, scalar_rep<T> > Clos;
                return array<T, N, RULES, Clos>(Clos(if_.rep(), then_.rep(), scalar_rep<T>(else_)));
        }


        // array ? scalar : scalar
        template <typename T, unsigned int N,
                typename RULES,
                typename IF>
        inline typename traits::enable_if_c<
                has_member_flow_if_then_else<RULES>::value
                  && !traits::is_class_type<T>::value,
                array<T, N, RULES,
                        if_then_else_rep<T, N, RULES, IF,
                                scalar_rep<T>,scalar_rep<T> >
                >
        >::type
        if_then_else (
                array<bool, N, RULES, IF> const &if_,
                T then_,
                T else_
        ) {
                typedef if_then_else_rep<T, N, RULES, IF, scalar_rep<T>, scalar_rep<T> > Clos;
                return array<T, N, RULES, Clos>(Clos(if_.rep(), scalar_rep<T>(then_), scalar_rep<T>(else_)));
        }
}

#endif // IF_THEN_ELSE_HH_INCLUDED_20100309
