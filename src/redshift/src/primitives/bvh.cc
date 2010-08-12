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

#include "../../include/basictypes/intersection.hh"
#include "../../include/primitives/bvh.hh"


namespace redshift { namespace primitive {



//-----------------------------------------------------------------------------
// Bvh
//-----------------------------------------------------------------------------
Bvh::Bvh() {
}



Bvh::~Bvh() {
}



BoundingBox Bvh::boundingBox() const {
        throw std::runtime_error("Bvh::boundingBox not implemented");
}



bool Bvh::doesIntersect (Ray const &) const {
        throw std::runtime_error("Bvh::doesIntersect not implemented");
}



optional<Intersection> Bvh::intersect(Ray const &) const {
        throw std::runtime_error("Bvh::intersect not implemented");
}



shared_ptr<Bsdf> Bvh::getBsdf(const DifferentialGeometry &) const {
        throw std::runtime_error("Bvh::getBsdf not implemented");
}



//-----------------------------------------------------------------------------
// Builder
//-----------------------------------------------------------------------------
BvhBuilder::BvhBuilder() {
}



void BvhBuilder::add (BoundPrimitive const &prim) {
}


shared_ptr<Bvh> BvhBuilder::toBvh() const {
        return shared_ptr<Bvh>();
}


} }
