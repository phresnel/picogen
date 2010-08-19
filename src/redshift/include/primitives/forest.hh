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

#ifndef FOREST_HH_INCLUDED_20100816
#define FOREST_HH_INCLUDED_20100816

#include "../sealed.hh"
#include "bvh.hh"

namespace redshift { namespace primitive {

        SEALED(Forest);
        class Forest :
                public BoundPrimitive,
                MAKE_SEALED(Forest)
        {
        public:
                Forest();
                ~Forest ();

                // -- BoundPrimitive ------------------------------------------
                BoundingBox boundingBox() const;
                // ------------------------------------------------------------

                // -- Primitive -----------------------------------------------
                bool doesIntersect (Ray const &) const;
                optional<Intersection> intersect(Ray const &) const;
                // ------------------------------------------------------------

                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & dgGeom
                ) const;

        private:
                //Forest ();
                Forest& operator= (Forest const &);
                Forest (Forest const&);

                shared_ptr<Bvh> aggregate;
        };
} }

#endif // FOREST_HH_INCLUDED_20100816
