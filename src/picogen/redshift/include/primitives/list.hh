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

#ifndef PRIMITIVE_LIST_HH_INCLUDED_20100120
#define PRIMITIVE_LIST_HH_INCLUDED_20100120

#include "../primitives/aggregate.hh"
#include "../sealed.hh"

namespace picogen { namespace redshift { namespace primitive {

        SEALED(List);

        class List
                : public Aggregate
                , MAKE_SEALED(List)
        {
        public:
                List();
                ~List ();

                bool doesIntersect (Ray const &ray) const ;
                optional<Intersection> intersect(Ray const &ray) const;

                void add (shared_ptr<Primitive> primitive) ;
                void prepare (const Scene &scene);

        private:
                List(List const&);
                List &operator = (List const&);

                typedef std::vector<shared_ptr<Primitive> > Primitives;
                Primitives primitives;
        };
} } }

#endif // PRIMITIVE_LIST_HH_INCLUDED_20100120
