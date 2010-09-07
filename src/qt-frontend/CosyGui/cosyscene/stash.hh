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


#ifndef STASH_HH_20100902
#define STASH_HH_20100902

#include <ctime>
#include <vector>
#include <cstdlib>
// Yes. This was inspired by git stash.

namespace actuarius {
        class IArchive;
}

template <typename T> class StashObject {
public:
        StashObject(T const &value)
        : time_(std::time(0)), value_(value)
        {}

        const T &value() const { return value_; }
        T &value() { return value_; }

        std::time_t time() const { return time_; }

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch);  // stash.def.hh
private:
        std::time_t time_;
        T value_;

        template <typename T_> friend class Stash;

        StashObject() {}
        friend class actuarius::IArchive;
};

template <typename T> class Stash {
public:
        // -- Vector Interface -------------------------------------------------
        typedef typename std::vector<StashObject<T> >::reverse_iterator
                         iterator;
        typedef typename std::vector<StashObject<T> >::const_reverse_iterator
                         const_iterator;

        void stash (T const &value) {
                objects.push_back(StashObject<T>(value));
        }

        void clear() { objects.clear(); }

        bool empty() const { return objects.empty(); }

        iterator       begin()       { return objects.rbegin(); }
        const_iterator begin() const { return objects.rbegin(); }
        iterator       end()         { return objects.rend(); }
        const_iterator end()   const { return objects.rend(); }
        T & back() { return objects.front().value(); }
        const T & back() const { return objects.front().value(); }
        // ---------------------------------------------------------------------

        // -- Serialization ----------------------------------------------------
        template<typename Arch> void serialize (Arch &arch); // stash.def.hh
        // ---------------------------------------------------------------------

private:
        std::vector<StashObject<T> > objects;
};

#endif // STASH_HH_20100902
