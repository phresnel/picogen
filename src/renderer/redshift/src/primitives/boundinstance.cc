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



shared_ptr<Bsdf> BoundInstance::getBsdf(const DifferentialGeometry &dg) const {
        return shared_ptr<Bsdf>();
}


/*
lsystem-tree {
                        code:   d1 = 94.74\;
                                d2 = 132.63\;
                                a = 18.95\;
                                lr = 1.109\;
                                vr = 1.732\;
                                #scale=0.01\;
                                #diascale=1.5\;
                                axiom: dia(1.0) f(0) rollright(45) A\;
                                p1: A --> dia(vr) f(100)
                                        [down(a) f(100) A] rollright(d1)
                                        [down(a) f(100) A] rollright(d2)
                                        [down(a) f(100) A]\;
                                p2: f(l) --> f(l*lr)\;
                                p3: dia(w) --> dia(w*vr)\;
                        ;
                        level:5;
                        slices:5;
                }
*/
BoundingBox BoundInstance::boundingBox() const {
        return instanceToWorld * primitive->boundingBox();
}


} }
