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

#ifndef TRAITS_HH_INCLUDED_20090829
#define TRAITS_HH_INCLUDED_20090829


namespace actuarius { namespace detail {
        // from boost
        template <bool B, class T = void> struct enable_if_c {
                typedef T type;
        };

        template <class T> struct enable_if_c<false, T>
        {};

        template <class Cond, class T = void>
        struct enable_if : public enable_if_c<Cond::value, T>
        {};
                
        template <bool B, class T = void>
        struct disable_if_c {
                typedef T type;
        };

        template <class T>
        struct disable_if_c<true, T>
        {};

        template <class Cond, class T = void>
        struct disable_if : public disable_if_c<Cond::value, T>
        {};
        //--

        // check if two types are identical
        template <typename LHS, typename RHS>
        struct identical { enum { value = 0 }; };
        
        template <typename T>
        struct identical<T,T> { enum { value = 1 }; };
        //--

        // check if T is a class type
        template <typename T> class is_class_type {
                struct Yes { char c[2]; };
                typedef char No;
                
                template <typename C> static Yes test (int C::*);
                template <typename C> static No test (...);
        public:
                enum { value = sizeof(Yes) == sizeof (test<T>(0)) };

        };
        //--

        // check if T has a member function void T::serialize
        template <typename Archive, typename T, bool B=is_class_type<T>::value >
        class has_serialize_function {
        public:
                enum { value = 0 };
        };

        template <typename Archive, typename T>
        class has_serialize_function<Archive, T, true> {

                struct Yes { char c[2]; };
                typedef char No;

                template <typename RET, RET (T::*) (Archive &)>
                struct enable_if_member_function_defined {
                        typedef T* type;
                };

                template <typename C> static Yes test(
                        typename enable_if_member_function_defined <
                                void, &C::serialize
                        >::type
                );
                template <typename C> static No test(...);
        public:
                enum { value = sizeof(Yes) == sizeof (test<T>(0)) };
        };
        //--
        
        // Check if T is a container type
        // (see below SUPPORTED CONTAINERS FOLLOW for supported containers)
        template <typename T, bool B=is_class_type<T>::value >
        class is_container_type {
        public:
                enum { value = 0 };
        };

        template <typename T>
        class is_container_type<T, true> {

                struct Yes { char c[2]; };
                typedef char No;

                // SUPPORTED CONTAINERS FOLLOW
                // std::vector<>
                template <typename C> static Yes test(
                        typename enable_if <identical<
                                C, 
                                std::vector<typename C::value_type, 
                                            typename C::allocator_type>
                        >, C*
                        >::type
                );
                // END OF SUPPORTED CONTAINERS
                template <typename C> static No test(...);
        public:
                enum { value = sizeof(Yes) == sizeof (test<T>(0)) };
        };
        //--
} }


#endif // TRAITS_HH_INCLUDED_20090829
