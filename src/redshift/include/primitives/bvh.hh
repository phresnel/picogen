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

#ifndef Bvh_HH_INCLUDED_20100811
#define Bvh_HH_INCLUDED_20100811

#include "../sealed.hh"
#include "aggregate.hh"

namespace redshift { namespace primitive {

        class BvhBuilder;
        class BvhNode;

        SEALED(Bvh);
        class Bvh :
                public BoundAggregate,
                MAKE_SEALED(Bvh)
        {
        public:
                ~Bvh ();

                // -- BoundPrimitive ------------------------------------------
                BoundingBox boundingBox() const;
                // ------------------------------------------------------------

                // -- Primitive -----------------------------------------------
                bool doesIntersect (Ray const &) const;
                optional<Intersection> intersect(Ray const &) const;
                // ------------------------------------------------------------

        private:
                friend class BvhBuilder;
                Bvh (shared_ptr<BvhNode> root);

                Bvh ();
                Bvh& operator= (Bvh const &);
                Bvh (Bvh const&);

                shared_ptr<BvhNode> root;
        };

        SEALED(BvhBuilder);
        class BvhBuilder :
                MAKE_SEALED(BvhBuilder)
        {
        public:
                BvhBuilder ();
                void add (shared_ptr<BoundPrimitive> prim);
                shared_ptr<Bvh> toBvh();
        private:
                BvhBuilder (BvhBuilder const &);
                BvhBuilder& operator= (BvhBuilder const&);

                shared_ptr<BvhNode> root;
        };
} }

#endif // ACCELERATOR_HH_INCLUDED_20100811
