//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
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

#ifndef LAZY_QUADTREE_HH_INCLUDED_20100116
#define LAZY_QUADTREE_HH_INCLUDED_20100116

#include "../setup.hh"
#include "../basictypes/intersection.hh"
#include "../primitives/primitive.hh"
#include "../basictypes/height-function.hh"

namespace redshift { namespace primitive {
        
        class LazyQuadtreeImpl;
        
        DefineFinalizer(LazyQuadtree);
        
        class LazyQuadtree
                : public Primitive                
                , DoFinalize(LazyQuadtree)
        {
        public:
                LazyQuadtree(shared_ptr<HeightFunction const> fun, real_t size);
                ~LazyQuadtree ();
                
                bool doesIntersect (RayDifferential const &ray) const;
                bool doesIntersect (Ray const &ray) const;

                optional<Intersection>
                        intersect(RayDifferential const &ray) const;

                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & dgGeom) const;
        private:

                LazyQuadtree();
                LazyQuadtree(LazyQuadtree const&);
                LazyQuadtree &operator = (LazyQuadtree const&);

                shared_ptr<LazyQuadtreeImpl> impl;
        };
} }

#endif // LAZY_QUADTREE_HH_INCLUDED_20100116
