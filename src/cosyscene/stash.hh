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
#include <algorithm>
#include <cstdlib>
#include <string>
// Yes. This was inspired by git stash.

namespace actuarius {
        class IArchive;
}

namespace picogen { namespace cosyscene {

template <typename T> class StashObject {
public:
        StashObject(T const &value)
        : time_(std::time(0)), value_(value)
        {}

        StashObject(T const &value, std::string const &description)
        : time_(std::time(0)), value_(value), description_(description)
        {}

        StashObject(std::time_t time, T const &value, std::string const &description)
        : time_(time), value_(value), description_(description)
        {}

        const T &value() const { return value_; }
        T &value() { return value_; }

        std::time_t time() const { return time_; }

        std::string description() const { return description_; }
        void setDescription (const std::string &d) { description_ = d; }

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch);  // stash.def.hh
private:
        std::time_t time_;
        T value_;
        std::string description_;

        template <typename T_> friend class Stash;

        StashObject() {}
        friend class actuarius::IArchive;
};

template <typename T> class Stash {
public:
        // -- Vector Interface -------------------------------------------------
        typedef typename std::vector<StashObject<T> >::iterator
                         iterator;
        typedef typename std::vector<StashObject<T> >::const_iterator
                         const_iterator;

        void stash (T const &value,
                    const std::string &description=std::string()
        ) {
                push_back(StashObject<T>(value,description));
        }

        void push_back (const StashObject<T> &so) {
                objects.push_back (so);
        }

        void clear() { objects.clear(); }

        bool empty() const { return objects.empty(); }

        iterator       begin()       { return objects.begin(); }
        const_iterator begin() const { return objects.begin(); }
        iterator       end()         { return objects.end(); }
        const_iterator end()   const { return objects.end(); }
        T & back() { return objects.back().value(); }
        const T & back() const { return objects.back().value(); }
        // ---------------------------------------------------------------------


        // -- Algo -------------------------------------------------------------
        bool contains_data (T const &value) const {
                EqualityPredicate ep(value, false);
                return end() != std::find_if(begin(), end(), ep);
        }

        void kill_all (T const &value) {
                objects.erase (std::remove_if (begin(), end(),
                                               EqualityPredicate(value, true)),
                               end());
        }
        // ---------------------------------------------------------------------


        // -- Serialization ----------------------------------------------------
        template<typename Arch> void serialize (Arch &arch); // stash.def.hh
        // ---------------------------------------------------------------------

private:
        std::vector<StashObject<T> > objects;

        struct EqualityPredicate {
                EqualityPredicate (StashObject<T> const &stashObject,
                                   bool isFalseForItemsWithDescription)
                : stashObject(stashObject)
                , isFalseForItemsWithDescription(isFalseForItemsWithDescription)
                {}

                EqualityPredicate (T const &stashObject,
                                   bool isFalseForItemsWithDescription)
                : stashObject(stashObject)
                , isFalseForItemsWithDescription(isFalseForItemsWithDescription)
                {}
                bool operator () (StashObject<T> const &rhs) const {
                        if (isFalseForItemsWithDescription) {
                                if (rhs.description() != "")
                                        return false;
                        }
                        return stashObject.value().data_equals (rhs.value());
                }
        private:
                StashObject<T> stashObject;
                bool isFalseForItemsWithDescription;
        };
};



// This brings some stash function to derived classes.
template <typename DERIVED> class StashableMixin {
public:
        const Stash<DERIVED>& getStash() const {
                return stash_;
        }

        Stash<DERIVED>& getStash() {
                return stash_;
        }

        void setStash(Stash<DERIVED> const &stash) {
                stash_ = stash;
        }

        void stash(const std::string &description=std::string()) {
                DERIVED tmp = *static_cast<DERIVED*>(this);
                tmp.clearStash();
                stash_.stash(tmp, description);
        }

        void clearStash() {
                stash_.clear();
        }

        virtual bool data_equals(DERIVED const &rhs) const = 0;

        bool is_default () const {
                return data_equals(DERIVED());
        }

        // The following should be implemented by derivers
        // (used by some widgets)
        //  bool data_equals(DERIVED const &rhs) const;
protected:
        cosyscene::Stash<DERIVED> stash_;
};

} }


#endif // STASH_HH_20100902
