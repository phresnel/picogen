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

#ifndef INCREMENT_DECREMENT_HH_INCLUDED_20100309
#define INCREMENT_DECREMENT_HH_INCLUDED_20100309

namespace kallisto {
        struct _invoke_preincrement {
                template <typename T> static void apply (T &elem) { ++elem; }
        };
        struct _invoke_postincrement {
                template <typename T> static void apply (T &elem) { elem++; }
        };
        struct _invoke_predecrement {
                template <typename T> static void apply (T &elem) { --elem; }
        };
        struct _invoke_postdecrement {
                template <typename T> static void apply (T &elem) { elem--; }
        };

        // prefix inc == lvalue
        // Need to be a template to enable enable_if.
        template <typename T_, unsigned int N_, typename RULES_, typename REP_>
        typename traits::enable_if<
                has_member_unary_prefix_inc<RULES_>,
                array<T_,N_,RULES_,REP_> &
        >::type
        operator ++ (array<T_,N_,RULES_,REP_> &arr) {
                _array_invoke<_invoke_preincrement>(arr);
                return arr;
        }
        // postfix inc == rvalue
        template <typename T_, unsigned int N_, typename RULES_, typename REP_>
        typename traits::enable_if<
                has_member_unary_postfix_inc<RULES_>,
                void
        >::type
        operator ++ (array<T_,N_,RULES_,REP_> &arr, int) {
                _array_invoke<_invoke_postincrement>(arr);
        }
        // prefix dec == lvalue
        template <typename T_, unsigned int N_, typename RULES_, typename REP_>
        typename traits::enable_if<
                has_member_unary_prefix_dec<RULES_>,
                array<T_,N_,RULES_,REP_> &
        >::type
        operator --(array<T_,N_,RULES_,REP_> &arr) {
                _array_invoke<_invoke_predecrement>(arr);
                return arr;
        }
        // postfix dec == rvalue
        template <typename T_, unsigned int N_, typename RULES_, typename REP_>
        typename traits::enable_if<
                has_member_unary_postfix_dec<RULES_>,
                void
        >::type
        operator -- (array<T_,N_,RULES_,REP_> &arr, int) {
                _array_invoke<_invoke_postdecrement>(arr);
        }
}

#endif // INCREMENT_DECREMENT_HH_INCLUDED_20100309
