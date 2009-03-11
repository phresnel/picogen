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

#ifndef SMART_PTR_HH_INCLUDED_20090304
#define SMART_PTR_HH_INCLUDED_20090304

///////////////////////////////////////////////////////////////////////////////
// Assimilate some smart pointers from boost.
///////////////////////////////////////////////////////////////////////////////
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
namespace redshift {
        
        /*
       
        // scoped_ptr
        template <typename T>
        struct scoped_ptr : boost::scoped_ptr<T> {
                // Assimilate constructors.
                explicit scoped_ptr (T * v) : boost::scoped_ptr<T> (v) {}
        private:
                // noncopyable
                scoped_ptr();
                scoped_ptr (scoped_ptr const &);
                scoped_ptr & operator = (scoped_ptr const &); 
        };
        
        // scoped_array
        template <typename T>
        struct scoped_array : boost::scoped_array<T> {
                // Assimilate constructors.
                explicit scoped_array (T * v) : boost::scoped_array<T> (v) {}
        private:
                // noncopyable
                scoped_array();
                scoped_array (scoped_array const &);
                scoped_array & operator = (scoped_array const &); 
        };
        
        // shared_ptr
        template <typename T>
        struct shared_ptr : boost::shared_ptr<T> {
                // Assimilate constructors.
                shared_ptr ()
                        : boost::shared_ptr<T> () {}
                
                explicit shared_ptr (T * v)
                        : boost::shared_ptr<T> (v) {}
                
                shared_ptr (shared_ptr const & v)
                        : boost::shared_ptr<T> (v) {}
                // The rest is okay via derivation.
        };
        
        // shared_ptr
        template <typename T>
        struct shared_array : boost::shared_array<T> {
                // Assimilate constructors.
                shared_array () : boost::shared_array<T> () {}
                
                explicit shared_array (T * v)
                        : boost::shared_array<T> (v) {}

                shared_array (shared_array const & v)
                        : boost::shared_array<T> (v) {}
                // The rest is okay via derivation.
        };*/
        
        using boost::scoped_ptr;
        using boost::scoped_array;
        using boost::shared_ptr;
        using boost::shared_array;
        
}

#endif // SMART_PTR_HH_INCLUDED_20090304
