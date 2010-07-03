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

#include "../testing/unit_test.hh"
#include "../testing/static_assert.hh"

namespace kallisto { namespace traits {
        template <typename T> struct get_type { typedef T type; };

        struct true_value { enum { value = true }; };
        struct false_value { enum { value = false }; };


        ///////////////////////////////////////////////////////////////////////
        // types_equal
        //   types_equal<A,B>::value = true,  if A == B
        //   types_equal<A,B>::value = false, if A != B
        ///////////////////////////////////////////////////////////////////////

        // 'types_equal' IS OBSOLETE, USE 'same_type'
        // looks like deprecated does not work for class templates
        template<typename A, typename B>
        struct types_equal  : public false_value  {};

        template<typename T>
        struct types_equal<T,T> : public true_value {};

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

// Neither gcc nor MSVC correctly evaluate the following tests.
// E.g., MSVC will evaluate ((S*)0)->XXX) (see can_deref_C_to_XXX),
// even if XXX is a static member.
// That's the reason why I have not given any class a more meaningful name.

struct YesNo {
protected:
        typedef char Yes;
        class No { char c[2]; };
};

template <typename C, template <typename> class test, bool do_exec> struct test_closure {
        enum { value = test<C>::value };
};
template <typename C, template <typename> class test> struct test_closure<C, test, false> {
        enum { value = 0 };
};


#ifdef KALLISTO_IMPLEMENT_HAS_MEMBER_XXX
# error "someone already defined KALLISTO_IMPLEMENT_HAS_MEMBER_XXX"
#endif

#define KALLISTO_IMPLEMENT_HAS_MEMBER_XXX(XXX)                                 \
                                                                               \
template <typename C> struct has_type_##XXX : ::kallisto::traits::YesNo {      \
        /* murk<> helps to let us call is_type() with an integer argument */   \
        template <typename S> struct murk { murk(int); };                      \
                                                                               \
        template <typename S> static Yes is_type (murk<typename S::XXX>);      \
        template <typename S> static No  is_type (...);                        \
                                                                               \
        enum { value = sizeof(Yes) == sizeof(is_type<C>(0)) };                 \
};                                                                             \
                                                                               \
                                                                               \
template <typename C> struct can_deref_C_to_##XXX : ::kallisto::traits::YesNo {\
        /* murk<> helps to let us call is_type() with an integer argument */   \
        template <int> struct murk { murk(int); };                             \
                                                                               \
        template<typename S> static Yes is_type (murk< sizeof(((S*)0)->XXX) >);\
        template<typename S> static No  is_type (...);                         \
                                                                               \
        enum { value = sizeof(Yes) == sizeof(is_type<C>(0)) };                 \
};                                                                             \
                                                                               \
template <typename C> struct can_take_address_of_##XXX                         \
        : ::kallisto::traits::YesNo                                            \
{                                                                              \
        /* murk<> helps to let us call is_type() with an integer argument */   \
        template <int> struct murk { murk(int); };                             \
                                                                               \
        template <typename S> static Yes is_type (murk< sizeof(&S::XXX) >);    \
        template <typename S> static No  is_type (...);                        \
                                                                               \
        enum { value = sizeof(Yes) == sizeof(is_type<C>(0)) };                 \
};                                                                             \
                                                                               \
template <typename C> struct has_member_##XXX {                                \
private:                                                                       \
        enum {                                                                 \
        test0 = ::kallisto::traits::test_closure<                              \
                                       C, has_type_##XXX, true>::value,        \
        test1 = test0 ||                                                       \
                ::kallisto::traits::test_closure<                              \
                     C, can_deref_C_to_##XXX, !test0>::value,                  \
        test2 = test1 ||                                                       \
                ::kallisto::traits::test_closure<                              \
                     C, can_take_address_of_##XXX, !test1>::value              \
        };                                                                     \
public:                                                                        \
        enum { value = test2 };                                                \
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
        //   * g++ 4.4.3
        //   * MSVC 2010 (but MSVC will not be officially supported, on time shot)
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
