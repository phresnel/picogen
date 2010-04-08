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

#ifndef CACHE_HH_INCLUDED_20100508
#define CACHE_HH_INCLUDED_20100508

#include <list>
#include <map>

namespace kallisto { namespace cache {
        enum UnloadStrategy {
                prune,
                collect
        };

        template <class T, UnloadStrategy US, void (*UNLOAD)(T o), class D>
        class cache_index {
        public:
                typedef typename std::list<T>::iterator iterator;
                typedef typename std::list<T>::const_iterator const_iterator;

                cache_index (unsigned int capacity)
                : capacity_ (capacity)
                {}

                const_iterator begin () const {
                        return elements.begin();
                }

                const_iterator end () const {
                        return elements.end();
                }

                void load (T p) {
                        return D::load(p);
                }

                void unload (T p) {
                        remove_index (p);
                        UNLOAD(p);
                }

                void remove_index (T p) {
                        elements.erase (dict[p]);
                        dict.erase(p);
                }

                void collect () {
                        return D::collect();
                }

                bool cached (T p) const {
                        return dict.end() != dict.find(p);
                }

                bool full () const {
                        return elements.size() >= capacity_;
                }

                bool exceeded () const {
                        return elements.size() > capacity_;
                }

                unsigned int capacity () const {
                        return capacity_;
                }

                unsigned int size () const {
                        return elements.size();
                }

                unsigned int stride () const {
                        return exceeded() ? elements.size()-capacity_
                                          : 0;
                }

        protected:

                typedef std::list<T> Elements;
                typedef std::map<T, typename std::list<T>::iterator> Dictionary;
                Elements elements;
                Dictionary dict;

                unsigned int capacity_;


                // [lib.list.modifiers]
                // * list::[push_front|push_back|insert]()
                //      do not invalidate any iterator
                // * list::[pop_front|pop_back|erase|clear]()
                //      only invalidate iterators + references to erased elements

                // Forbid:
                cache_index (cache_index const &);
                cache_index &operator = (cache_index const &);

        };



        template <typename T, UnloadStrategy US, void (*UNLOAD)(T o)>
        class mru_cache_index
        : public cache_index<T, US, UNLOAD, mru_cache_index<T,US,UNLOAD> > {
        public:
                mru_cache_index (unsigned int capacity)
                : cache_index<T, US, UNLOAD, mru_cache_index<T,US,UNLOAD> >(capacity)
                {}

                void load (T p) {
                        if (cached (p)) {
                                reload (p);
                        } else {
                                if (US == prune) unload_last_if_full();
                                load_and_update_index(p);
                        }
                }

                void collect () {
                        while (this->exceeded())
                                this->unload (this->elements.back());
                }

        private:

                void load_and_update_index(T p) {
                        this->elements.push_front (p);
                        this->dict[p] = this->elements.begin();
                }

                void unload_last_if_full () {
                        if (this->full())
                                this->unload (this->elements.back());
                }

                void reload (T p) {
                        this->elements.splice (
                                this->elements.begin(),
                                this->elements, this->dict[p]);
                }

                // Forbid
                mru_cache_index (mru_cache_index const &);
                mru_cache_index &operator = (mru_cache_index const &);
        };
} }

#endif
