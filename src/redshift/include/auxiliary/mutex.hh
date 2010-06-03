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

#ifndef MUTEX_HH_INCLUDED_20100221
#define MUTEX_HH_INCLUDED_20100221

#ifdef _OPENMP
#include <omp.h>

namespace redshift {
        struct Mutex {
                Mutex() { omp_init_lock(&lock); }
                ~Mutex() { omp_destroy_lock(&lock); }
                void Lock() { omp_set_lock(&lock); }
                void Unlock() { omp_unset_lock(&lock); }

                bool Test () {
                        return !!omp_test_lock (&lock);
                }

                Mutex(const Mutex& ) { omp_init_lock(&lock); }
                Mutex& operator= (const Mutex& ) { return *this; }
        public:
                omp_lock_t lock;
        };
}
#else
#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace redshift {
        struct Mutex {
                Mutex() {  }
                ~Mutex() {  }
                void Lock() { lock.lock(); }
                void Unlock() { lock.unlock(); }

                bool Test () {
                        return !!lock.try_lock ();
                }

                //Mutex(const Mutex& ) { lock); }
                //Mutex& operator= (const Mutex& ) { return *this; }
        public:
                boost::interprocess::interprocess_mutex lock;
        };
}
#endif

#endif // MUTEX_HH_INCLUDED_20100221
