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

#ifndef TRIANGLEBVH_HH_INCLUDED_20100813
#define TRIANGLEBVH_HH_INCLUDED_20100813

#include "../sealed.hh"
#include "triangle.hh"
#include "aggregate.hh"

namespace picogen { namespace redshift { namespace primitive {

        class TriangleBvhBuilder;
        class TriangleBvhNode;
        class TriangleBvhTri;

        SEALED(TriangleBvh);
        class TriangleBvh :
                public BoundAggregate,
                MAKE_SEALED(TriangleBvh)
        {
        public:
                ~TriangleBvh ();

                // -- BoundPrimitive ------------------------------------------
                BoundingBox boundingBox() const;
                // ------------------------------------------------------------

                // -- Primitive -----------------------------------------------
                bool doesIntersect (Ray const &) const;
                optional<Intersection> intersect(Ray const &) const;
                // ------------------------------------------------------------

        private:
                friend class TriangleBvhBuilder;
                TriangleBvh (TriangleBvhTri *triangles,
                             shared_ptr<TriangleBvhNode> root);

                TriangleBvh ();
                TriangleBvh& operator= (TriangleBvh const &);
                TriangleBvh (TriangleBvh const&);

                TriangleBvhTri *triangles;
                shared_ptr<TriangleBvhNode> root;
        };

        SEALED(TriangleBvhBuilder);
        class TriangleBvhBuilder :
                MAKE_SEALED(TriangleBvhBuilder)
        {
        public:
                TriangleBvhBuilder ();
                void add (Triangle prim);
                shared_ptr<TriangleBvh> toTriangleBvh();
        private:
                TriangleBvhBuilder (TriangleBvhBuilder const &);
                TriangleBvhBuilder& operator= (TriangleBvhBuilder const&);

                std::vector<Triangle> triangles;
        };
} } }

#endif // TRIANGLEBVH_HH_INCLUDED_20100813
