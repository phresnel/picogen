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

#ifndef PRIMITIVE_HEIGHTMAP_HH_INCLUDED_20090318
#define PRIMITIVE_HEIGHTMAP_HH_INCLUDED_20090318

#include "../setup.hh"
#include "../basictypes/height-function.hh"
#include "../primitives/primitive.hh"
#include "../rendertargets/rendertarget.hh"

namespace redshift { namespace primitive {


DefineFinalizer(Heightmap);
class Heightmap
        : public Primitive
        , DoFinalize(Heightmap)
{
public:
        Heightmap(shared_ptr<HeightFunction const> fun, real_t detail);
        ~Heightmap ();
        
        bool doesIntersect (RayDifferential const &ray) const;
        bool doesIntersect (Ray const &ray) const;

        optional<Intersection> intersect(RayDifferential const &ray) const;
        optional<Intersection> intersect(Sample const &sample) const;

        shared_ptr<Bsdf> LazyQuadtree::getBsdf(
                const DifferentialGeometry & dgGeom
        ) const {
                return shared_ptr<Bsdf> (new bsdf::Lambertian (
                        dgGeom, dgGeom.getNormal(), Color::fromRgb(1,0,0)
                ));
        }

private:
        // forbid
        Heightmap();
        Heightmap(Heightmap const&);
        Heightmap &operator = (Heightmap const&);
        
        // .
        optional<Intersection> intersect(RayDifferential const &,real_t) const;
        
        // data
        mutable std::vector<real_t> depthBuffer;                
        shared_ptr<HeightFunction const> function;
        real_t detail;
        BoundingBox boundingBox;
};

} }

#endif // PRIMITIVE_HEIGHTMAP_HH_INCLUDED_20090318
