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

#ifndef ARRAY_HH_INCLUDED_20100310
#define ARRAY_HH_INCLUDED_20100310

#include <limits>
#include "../traits/traits.hh"

// Notes:
//  * Postfix increment/decrement return void by design (see somewhere below)
//  * copy assignment return void by design, so that nobody makes the mistake
//    of e.g. "if_then_else(a<b, a=b, a=c)", in which a=b and a=c are *not*
//    lambda expressions.

namespace kallisto {
        // Disclaimer: kallisto::array shall not be a replacement for valarray,
        // especially not for the GNU implementation, which in contrast to
        // MSVC's one is based on expression templates, and indeed very
        // performant (see also Vandevoorde/Josuttis template guide), but
        // aims to provide a configurable, compile-time fixed-size array.
        // Thanks to that specialization it seems that I am at least as fast as
        // GNU's/Gabriel's implementation for that target domain for all
        // tests I've run.
        // On the contrary, even though Gabriel's valarray is runtime-sized
        // (as per the holy standard), it often comes near my specialized
        // function; so if you need a runtime-sized array, maybe first look
        // at your compiler's implementation of valarray<>. Also, look at
        // valarray<> if you need to work with slices.




        template <typename T, unsigned int N>
        class array_rep {
        public:
                enum { size = N };
                T operator [] (unsigned int i) const {
                        return c[i];
                }
                T & operator [] (unsigned int i) {
                        return c[i];
                }
        private:
                T c[N];
        };


        template <typename T>
        class scalar_rep {
        public:
                scalar_rep (T val) : c (val) {}

                enum { size = 0 };
                T operator [] (unsigned int) const {
                        return c;
                }
                T & operator [] (unsigned int) {
                        return c;
                }
        private:
                T c;
        };


        template <typename T> struct expr_ref {
                typedef T const &type;
        };
        template <typename T>
        struct expr_ref<scalar_rep<T> > {
                typedef scalar_rep<T> type;
        };
}

#include "array_bits/rulesets.hh"



#ifdef KALLISTO_ARRAY_FOREACHELEM
# error "something already defined KALLISTO_ARRAY_FOREACHELEM"
#endif
#ifdef KALLISTO_ARRAY_FRIEND_FOREACHELEM
# error "something already defined KALLISTO_ARRAY_FRIEND_FOREACHELEM"
#endif

#define KALLISTO_ARRAY_FOREACHELEM(x) \
        for (unsigned int i=0; i<N; ++i) { \
                x; \
        }
#define KALLISTO_ARRAY_FRIEND_FOREACHELEM(x,N_) \
        for (unsigned int i=0; i<N_; ++i) { \
                x; \
        }



#include "array_bits/array.hh"
#include "array_bits/increment_decrement.hh"
#include "array_bits/binary_operators.hh"
#include "array_bits/unary_operators.hh"
#include "array_bits/relational.hh"
#include "array_bits/if_then_else.hh"
#include "array_bits/cmath.hh"
#include "array_bits/reduction.hh"



#undef KALLISTO_ARRAY_FRIEND_FOREACHELEM
#undef KALLISTO_ARRAY_FOREACHELEM


#endif // ARRAY_HH_INCLUDED_20100310
