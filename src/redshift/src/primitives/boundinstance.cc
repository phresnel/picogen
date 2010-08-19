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

#include "../../include/primitives/boundinstance.hh"
#include "../../include/basictypes/intersection.hh"
#include <stdexcept>

namespace redshift { namespace primitive {



BoundInstance::BoundInstance (Transform const &transform,
                              shared_ptr<BoundPrimitive> primitive
)
: transform(transform.inverse())
, primitive (primitive)
{
}



BoundInstance::~BoundInstance () {
}



bool BoundInstance::doesIntersect (Ray const &ray) const {
        return primitive->doesIntersect (transform * ray);
}



optional<Intersection>
 BoundInstance::intersect(Ray const &ray) const {
        optional<Intersection> i = primitive->intersect (transform * ray);
        if (!i) return optional<Intersection>();
        i->applyTransform (transform.inverse());
        return i;
}



shared_ptr<Bsdf> BoundInstance::getBsdf(const DifferentialGeometry &dg) const {
        return shared_ptr<Bsdf>();
}



BoundingBox BoundInstance::boundingBox() const {
        const BoundingBox orig = primitive->boundingBox();
        return BoundingBox(
                transform.inverse() * orig.minimum(),
                transform.inverse() * orig.maximum()
        );
        throw std::runtime_error("BoundInstance::boundingBox() is not implemented yet");
}


} }
