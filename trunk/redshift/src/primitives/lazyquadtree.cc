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

#include "../../include/primitives/lazyquadtree.hh"


//#############################################################################
// LazyQuadtreeImpl
//#############################################################################
namespace redshift { namespace primitive {
class LazyQuadtreeImpl {
public:
        LazyQuadtreeImpl (
                shared_ptr<HeightFunction const> fun,
                real_t size
        )
        : fun(fun)
        {
                initBB (size,min(1000.f,(size*size*size)/100));
        }

        bool doesIntersect (Ray const &ray) const {
                return does_intersect<false> (ray, primaryBB);
        }

        optional<DifferentialGeometry> intersect (Ray const &ray) const {
                const optional<tuple<real_t,real_t> > i = 
                                        redshift::intersect<false>(ray,primaryBB);
                if (!i) return false;

                const real_t minT = get<0>(*i);
                const real_t maxT = get<1>(*i);

                return DifferentialGeometry(
                        minT,
                        ray(minT),
                        Normal(0,1,0)
                );
        }
private:
        shared_ptr<HeightFunction const> fun;
        BoundingBox primaryBB;
        void initBB(const real_t size, const int numSamples) {
                real_t minh = 10000000.0f , maxh = -10000000.0f;
                MersenneTwister<real_t,0,1> mt (4654641);
                const real_t size05 = size/2;
                for (int x=0; x<numSamples; ++x) {
                        const real_t uv[] = {
                                mt.rand()*size-size05,
                                mt.rand()*size-size05
                        };
                        const real_t h = (*fun)(uv[0],uv[1]);
                        if (h < minh) minh = h;
                        if (h > maxh) maxh = h;
                }
                primaryBB = BoundingBox (
                        Point(
                                scalar_cast<BoundingBox::scalar_t>(-size05),
                                scalar_cast<BoundingBox::scalar_t>(minh),
                                scalar_cast<BoundingBox::scalar_t>(-size05)
                        ),
                        Point(
                                scalar_cast<BoundingBox::scalar_t>(size05),
                                scalar_cast<BoundingBox::scalar_t>(maxh),
                                scalar_cast<BoundingBox::scalar_t>(size05)
                        )
                );
        }
};
} }



//#############################################################################
// LazyQuadtree
//#############################################################################
namespace redshift { namespace primitive {

LazyQuadtree::LazyQuadtree (shared_ptr<HeightFunction const> fun, real_t size)
: impl (new LazyQuadtreeImpl (fun, size)) {
}



LazyQuadtree::~LazyQuadtree () {
}



bool LazyQuadtree::doesIntersect (RayDifferential const &ray) const {
        return impl->doesIntersect (ray);
}



bool LazyQuadtree::doesIntersect (Ray const &ray) const {
        return impl->doesIntersect (ray);
}



optional<Intersection>
 LazyQuadtree::intersect(RayDifferential const &ray) const {

        /*optional<DifferentialGeometry> 
                       i(sphereData.intersect(ray));
        if (i) { 
                return Intersection (shared_from_this(), *i);
        } else {
                return false;
        }*/
        const optional<DifferentialGeometry> dg = impl->intersect (ray);
        if (!dg) return false;
        return Intersection (
                shared_from_this(),
                *dg
        );
}



} }
