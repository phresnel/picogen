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

#include "ignore_strict_aliasing" // because of boost::optional
#include "../../include/primitives/boundinstance.hh"
#include "../../include/basictypes/intersection.hh"
#include <stdexcept>

namespace picogen { namespace redshift { namespace primitive {




BoundInstance::BoundInstance (Transform const &transform,
                              shared_ptr<BoundPrimitive> primitive
)
: instanceToWorld(transform)
, worldToInstance(instanceToWorld.inverse())
, primitive (primitive)
{
}



BoundInstance::~BoundInstance () {
}



bool BoundInstance::doesIntersect (Ray const &ray) const {
        return primitive->doesIntersect (worldToInstance * ray);
}



optional<Intersection>
 BoundInstance::intersect(Ray const &ray) const {
        optional<Intersection> i = primitive->intersect (worldToInstance * ray);
        if (!i) return optional<Intersection>();
        i->applyTransform (instanceToWorld);
        return i;
}



shared_ptr<Bsdf> BoundInstance::getBsdf(const DifferentialGeometry &) const {
        throw std::runtime_error ("BoundInstance::getBsdf() called directly");
}



BoundingBox BoundInstance::boundingBox() const {
        return instanceToWorld * primitive->boundingBox();
}



void BoundInstance::prepare (const Scene &s) {
        primitive->prepare (s);
}
} } }
