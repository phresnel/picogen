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

#ifndef PACK_HH_INCLUDED_20090919
#define PACK_HH_INCLUDED_20090919

namespace actuarius {
        //---------------------------------------------------------------------
        // The function pack() is a convenience function that will select
        // the right call to either make_(nrp|nerp|ncrp|necrp), plus which
        // allows to flip the Enum<T> argument with the T argument.
        // Only convention is actually that the name of the entitity goes
        // left most.
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // Scalar types.
        //---------------------------------------------------------------------
        template <typename T>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                nrp<T>
        >::type
        pack (const char *name, T &value) {
                return make_nrp (name, value);
        }

        template <typename T>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                nerp<T>
        >::type
        pack (const char *name, Enum<T> const & enumDesc, T &value) {
                return make_nerp (name, enumDesc, value);
        }

        // convenience overload.
        template <typename T>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                nerp<T>
        >::type
        pack (const char *name, T &value, Enum<T> const & enumDesc) {
                return make_nerp (name, enumDesc, value);
        }
        //--


        //---------------------------------------------------------------------
        // Unnamed values.
        //---------------------------------------------------------------------
        template <typename T>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                ref<T>
        >::type pack (T &value) {
                return make_ref (value);
        }

        template <typename T>
        inline
        typename detail::enable_if<
                detail::is_container_type<T>,
                containerref<T>
        >::type
        pack (T &value) {
                return make_containerref (value);
        }



        //---------------------------------------------------------------------
        // Container types.
        //---------------------------------------------------------------------
        template <typename T>
        inline
        typename detail::enable_if<
                detail::is_container_type<T>,
                ncrp<T>
        >::type
        pack (const char *name, T &value) {
                return make_ncrp (name, value);
        }



        template <typename T>
        inline
        typename detail::enable_if<
                detail::is_container_type<T>,
                necrp<T>
        >::type
        pack (
                const char *name,
                Enum<typename T::value_type> const & enumDesc,
                T &value
        ) {
                return make_necrp (name, enumDesc, value);
        }



        // convenience overload
        template <typename T>
        inline
        typename detail::enable_if<
                detail::is_container_type<T>,
                necrp<T>
        >::type
        pack (
                const char *name,
                T &value,
                Enum<typename T::value_type> const & enumDesc
        ) {
                return make_necrp (name, enumDesc, value);
        }
        //--


        //---------------------------------------------------------------------
        // Special.
        //---------------------------------------------------------------------
        inline
        nrp<std::string>
        pack (const char* name, std::string &value) {
                return make_nrp (name, value);
        }



        //---------------------------------------------------------------------
        // Advice lists.
        //---------------------------------------------------------------------
        template <typename T, typename ADVICE_TYPE>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                nprp<T,ADVICE_TYPE>
        >::type
        pack (
                const char *name,
                ADVICE_TYPE T::* ptr,
                T &value
        ) {
                return make_nprp(name, ptr, value);
        }

        template <typename CONT, typename ADVICE_TYPE>
        inline
        typename detail::enable_if<
                detail::is_container_type<CONT>,
                npcrp<CONT,ADVICE_TYPE>
        >::type
        pack (
                const char *name,
                ADVICE_TYPE CONT::value_type::* ptr,
                CONT &value
        ) {
                return make_npcrp(name, ptr, value);
        }

        template <typename T, typename ADVICE_TYPE>
        inline
        typename detail::disable_if<
                detail::is_container_type<T>,
                nperp<T,ADVICE_TYPE>
        >::type
        pack (
                const char *name,
                ADVICE_TYPE T::* ptr,
                Enum<ADVICE_TYPE> enumDesc, T &value
        ) {
                return make_nperp(name, ptr, enumDesc, value);
        }

        template <typename CONT, typename ADVICE_TYPE>
        inline
        typename detail::enable_if<
                detail::is_container_type<CONT>,
                npecrp<CONT,ADVICE_TYPE>
        >::type
        pack (
                const char *name,
                ADVICE_TYPE CONT::value_type::* ptr,
                Enum<ADVICE_TYPE> enumDesc,
                CONT &value
        ) {
                return make_npecrp(name, ptr, enumDesc, value);
        }

        template <typename CONT, typename ADVICE_TYPE>
        inline
        typename detail::enable_if<
                detail::is_container_type<CONT>,
                pecrp<CONT,ADVICE_TYPE>
        >::type
        pack (
                ADVICE_TYPE CONT::value_type::* ptr,
                Enum<ADVICE_TYPE> enumDesc,
                CONT &value
        ) {
                return make_pecrp(ptr, enumDesc, value);
        }
}

#endif // PACK_HH_INCLUDED_20090919
