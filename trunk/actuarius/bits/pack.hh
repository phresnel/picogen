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
        typename detail::disable_if<
                detail::is_container_type<T>,
                nrp<T>
        >::type
        pack (const char *name, T &value) {
                return make_nrp (name, value);
        }
        
        template <typename T> 
        typename detail::disable_if<
                detail::is_container_type<T>,
                nerp<T>
        >::type
        pack (const char *name, Enum<T> const & enumDesc, T &value) {
                return make_nerp (name, enumDesc, value);
        }
        
        // convenience overload.
        template <typename T> 
        typename detail::disable_if<
                detail::is_container_type<T>,
                nerp<T>
        >::type
        pack (const char *name, T &value, Enum<T> const & enumDesc) {
                return make_nerp (name, enumDesc, value);
        }
        //--



        //---------------------------------------------------------------------
        // Container types.
        //---------------------------------------------------------------------
        template <typename T> 
        typename detail::enable_if<
                detail::is_container_type<T>,
                ncrp<T>
        >::type
        pack (const char *name, T &value) {                
                return make_ncrp (name, value);
        }
        


        template <typename T> 
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
}

#endif // PACK_HH_INCLUDED_20090919
