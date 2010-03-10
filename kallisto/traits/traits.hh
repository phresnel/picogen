//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef TRAITS_H__INCLUDED__20090210
#define TRAITS_H__INCLUDED__20090210

namespace kallisto { namespace traits {
        template <typename T> struct get_type { typedef T type; };

        struct true_value { enum { value = true }; };
        struct false_value { enum { value = false }; };


        ///////////////////////////////////////////////////////////////////////
        // types_equal
        //   types_equal<A,B>::value = true,  if A == B
        //   types_equal<A,B>::value = false, if A != B
        ///////////////////////////////////////////////////////////////////////
        template<typename A, typename B>
        struct types_equal : public false_value {};

        template<typename T>
        struct types_equal<T,T> : public true_value  {};

        STATIC_ASSERT(!(types_equal<int32_t,int64_t>::value));
        STATIC_ASSERT(!(types_equal<uint64_t,int64_t>::value));
        STATIC_ASSERT((types_equal<float,float>::value));


        ///////////////////////////////////////////////////////////////////////
        // same_type
        //   same_type<A,B>::value = true,  if A == B
        //   same_type<A,B>::value = false, if A != B
        ///////////////////////////////////////////////////////////////////////
        template<typename A, typename B>
        struct same_type : public false_value {};

        template<typename T>
        struct same_type<T,T> : public true_value  {};

        STATIC_ASSERT(!(same_type<int32_t,int64_t>::value));
        STATIC_ASSERT(!(same_type<uint64_t,int64_t>::value));
        STATIC_ASSERT((same_type<float,float>::value));



        ///////////////////////////////////////////////////////////////////////
        // is_class_type
        ///////////////////////////////////////////////////////////////////////
        template <typename T> class is_class_type {
                typedef char Yes;
                class No { char c[2]; };

                template <typename S> static Yes test(int S::*);
                template <typename S> static No  test(...);
        public:
                enum { value = sizeof(Yes) == sizeof(test<T>(0)) };
        };


        ///////////////////////////////////////////////////////////////////////
        // enable_if
        ///////////////////////////////////////////////////////////////////////
        template <bool B, class T = void>
        struct enable_if_c { typedef T type; };

        template <class T>
        struct enable_if_c<false, T> {};

        template <class Cond, class T = void>
        struct enable_if : public enable_if_c<Cond::value, T> {};

        template <class Cond, class T = void>
        struct disable_if : public enable_if_c<!Cond::value, T> {};


} }


///////////////////////////////////////////////////////////////////////////////
// has_member_XXX
///////////////////////////////////////////////////////////////////////////////
namespace kallisto { namespace traits { // << it is the intentention that counts

#ifdef KALLISTO_IMPLEMENT_HAS_MEMBER_XXX
# error "someone already defined KALLISTO_IMPLEMENT_HAS_MEMBER_XXX"
#endif

#define KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(XXX)                                \
template <bool cond, typename S, template <typename> class probe>             \
struct has_member_##XXX##_if_then {                                           \
        enum { value = probe<S>::value };                                     \
};                                                                            \
                                                                              \
template <typename S, template <typename> class probe>                        \
struct has_member_##XXX##_if_then<false,S,probe> {                            \
        enum { value = false };                                               \
};                                                                            \
                                                                              \
template <typename C> class has_member_##XXX {                                \
                                                                              \
        typedef char Yes;                                                     \
        class No { char c[2]; };                                              \
                                                                              \
        /*                                                                    \
        // murk and merk allow us to encode expressions like                  \
        // "sizeof(&T::XXX)" within argument lists, and still have someone    \
        // take an int (see their ctors)                                      \
        */                                                                    \
        template <int>      struct murk { murk(int); };                       \
        template <typename> struct merk { merk(int); };                       \
                                                                              \
                                                                              \
        /*                                                                    \
        // Test 0 yields true for XXX that are ...                            \
        //                                                                    \
        //   * member variables            -> "enum{} XXX;"                   \
        //   * non-static member functions -> "void XXX (){}"                 \
        //   * non-static member variables -> "static float XXX;"             \
        //   * static member functions     -> "static void XXX (){}"          \
        //   * static member variables     -> "static float XXX;"             \
        //                                                                    \
        // => Types may also be aggregates et al -> "struct K{};static K XXX;"\
        */                                                                    \
        template <typename S> static Yes test0(murk<sizeof(&S::XXX)>);        \
        template <typename S> static No test0(...);                           \
                                                                              \
        /*                                                                    \
        // Test 1 yields true for XXX that are ...                            \
        //                                                                    \
        //   * types -> "typedef void XXX; class XXX; struct XXX; enum XXX{};"\
        */                                                                    \
        template <typename S> static Yes test1(merk<typename S::XXX>);        \
        template <typename S> static No test1(...);                           \
                                                                              \
                                                                              \
        /*                                                                    \
        // Test 2 _could_ yield true for ...                                  \
        //                                                                    \
        //   * rvalue constants            -> "enum{XXX};"  [!]               \
        //   * non-static member variables -> "enum{}XXX;"                    \
        //   * static member variables     -> "static float XXX;"             \
        //                                                                    \
        // ... but we can't run the test, even in SFINAE, when T::XXX happens \
        // to be a non-static member function. Hence we only enable it        \
        // once we captured the non-static member function, or when we have   \
        // already proven that it is not one, and use it for ...              \
        //                                                                    \
        //   * rvalue constants            -> "enum{XXX};"                    \
        //                                                                    \
        // .                                                                  \
        */                                                                    \
        template <typename S> class test2 {                                   \
                template <typename T> static Yes test(murk<sizeof(T::XXX)>);  \
                template <typename T> static No test(...);                    \
        public:                                                               \
                enum { value = sizeof(Yes)==sizeof(test<S>(0)) };             \
        };                                                                    \
                                                                              \
        enum { test0result = sizeof(Yes) == sizeof(test0<C>(0)) };            \
        enum { test1result = sizeof(Yes) == sizeof(test1<C>(0)) };            \
        enum { test2result = has_member_##XXX##_if_then<                      \
                                !test0result&&!test1result,                   \
                                C,                                            \
                                test2                                         \
                        >::value                                              \
        };                                                                    \
                                                                              \
public:                                                                       \
        enum {                                                                \
                value = test0result || test1result || test2result             \
        };                                                                    \
                                                                              \
};

#if 0

KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(FOOO)

struct Foo { enum{FOOO}; };
struct Bar { enum{} FOOO; };
struct Frob { void FOOO (){} };
struct Farb { static void FOOO (); };
struct Frobnicate { typedef void FOOO; };
struct Frobnicaterate { class FOOO {}; };
struct Frobnicateration { enum FOOO {}; };

struct Froobnicate { static float FOOO; };
struct Ferbicate { float FOOO; };
struct Frooobnicate { struct klass {}; klass FOOO; };
struct Noofrigate { struct klass {}; static klass FOOO; };
struct Frobtibob : Foo {};

struct Negrigate {};

#include <iostream>
int main () {
        // Yields 1 for all but Negrigate for
        //   * g++ 4.3 (g++-4.3 (Debian 4.3.4-6) 4.3.4)
        //   * g++ 4.4 (g++ (Debian 4.4.2-9) 4.4.3 20100108 (prerelease))
        //   * g++ 4.5 (g++ (GCC) 4.5.0 20100306 (experimental))
        std::cout << "Foo: " << has_member_FOOO<Foo>::value << std::endl;
        std::cout << "Bar: " << has_member_FOOO<Bar>::value << std::endl;
        std::cout << "Frob: " << has_member_FOOO<Frob>::value << std::endl;
        std::cout << "Farb: " << has_member_FOOO<Farb>::value << std::endl;
        std::cout << "Frobnicate: " << has_member_FOOO<Frobnicate>::value << std::endl;
        std::cout << "Frobnicaterate: " << has_member_FOOO<Frobnicaterate>::value << std::endl;
        std::cout << "Frobnicateration: " << has_member_FOOO<Frobnicateration>::value << std::endl;
        std::cout << "Froobnicate: " << has_member_FOOO<Froobnicate>::value << std::endl;
        std::cout << "Ferbicate: " << has_member_FOOO<Ferbicate>::value << std::endl;
        std::cout << "Frooobnicate: " << has_member_FOOO<Frooobnicate>::value << std::endl;
        std::cout << "Noofrigate: " << has_member_FOOO<Noofrigate>::value << std::endl;
        std::cout << "Frobtibob: " << has_member_FOOO<Frobtibob>::value << std::endl;
        std::cout << "Negrigate:" << has_member_FOOO<Negrigate>::value << std::endl;
}
#endif
} }

namespace kallisto { namespace unit_test {
        class test_traits {
        private:
                const static test_traits test;
                test_traits () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // TRAITS_H__INCLUDED__20090210
