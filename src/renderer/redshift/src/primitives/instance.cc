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

#include "../../include/primitives/instance.hh"
#include "../../include/basictypes/intersection.hh"

namespace redshift { namespace primitive {




Instance::Instance (Transform const &transform,
                    shared_ptr<Primitive> primitive
)
: instanceToWorld(transform)
, worldToInstance(instanceToWorld.inverse())
, primitive (primitive)
{
}



Instance::~Instance () {
}



bool Instance::doesIntersect (Ray const &ray) const {
        return primitive->doesIntersect (worldToInstance * ray);
}



optional<Intersection>
 Instance::intersect(Ray const &ray) const {
        optional<Intersection> i = primitive->intersect (worldToInstance * ray);
        if (!i) return optional<Intersection>();
        i->applyTransform (instanceToWorld);
        return i;
}



shared_ptr<Bsdf> Instance::getBsdf(const DifferentialGeometry &dg) const {
        return shared_ptr<Bsdf>();
}



void Instance::prepare (const Scene &s) {
        primitive->prepare (s);
}
} }
