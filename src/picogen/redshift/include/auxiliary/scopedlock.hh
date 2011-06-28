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

#ifndef SCOPEDLOCK_HH_INCLUDED_20100221
#define SCOPEDLOCK_HH_INCLUDED_20100221

#include "mutex.hh"
namespace picogen { namespace redshift {
        struct ScopedLock {
                explicit ScopedLock(Mutex& m) : mut(m), locked(true) { mut.Lock(); }
                ~ScopedLock() { Unlock(); }
                void Unlock() { if(!locked) return; locked=false; mut.Unlock(); }
                void LockAgain() { if(locked) return; mut.Lock(); locked=true; }
        private:
                Mutex& mut;
                bool locked;
        private:
                void operator=(const ScopedLock&);
                ScopedLock(const ScopedLock&);
        };
} }

#endif // SCOPEDLOCK_HH_INCLUDED_20100221
