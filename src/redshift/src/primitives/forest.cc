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

#include <stack>
#include <stdexcept>
#include <boost/optional.hpp>

#include "../../include/constants.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/primitives/forest.hh"
#include "../../include/primitives/lsystemtree.hh"
#include "../../include/primitives/boundinstance.hh"


namespace redshift { namespace primitive {


//-----------------------------------------------------------------------------
// Bvh
//-----------------------------------------------------------------------------
Forest::Forest()
{
        shared_ptr<LSystemTree> tree (new LSystemTree (
                "d1 = 94.74;\n"
                "d2 = 132.63;\n"
                "a = 18.95;\n"
                "lr = 1.109;\n"
                "vr = 1.732;\n"
                "#scale=0.05;\n"
                "#diascale=1.5;\n"

                "axiom: dia(1.0) f(0) rollright(45) A;\n"
                "p1: A --> dia(vr) f(100) \n"
                        "[down(a) f(100) A] rollright(d1)\n"
                        "[down(a) f(100) A] rollright(d2)\n"
                        "[down(a) f(100) A];\n"
                "p2: f(l) --> f(l*lr);\n"
                "p3: dia(w) --> dia(w*vr);\n",
                5,
                5
        ));
        BvhBuilder builder;
        for (int i=0; i<25000; ++i) {
                const Transform t =
                        /*Transform::rotationY(
                                rand()/(double)RAND_MAX * 3.14159*2)
                        **/
                        Transform::translation(
                                (-0.5 + rand() / (double)RAND_MAX) * 1024*10,
                                0,
                                (-0.5 + rand() / (double)RAND_MAX) * 1024*10)
                        ;
                builder.add(shared_ptr<BoundPrimitive>(
                                new BoundInstance(t, tree)));
        }
        aggregate = builder.toBvh();
}



Forest::~Forest() {
}



BoundingBox Forest::boundingBox() const {
        return aggregate->boundingBox();
}



bool Forest::doesIntersect (Ray const &ray) const {
        return aggregate->doesIntersect(ray);
}



optional<Intersection> Forest::intersect(Ray const &ray) const {
        return aggregate->intersect(ray);
}



shared_ptr<Bsdf> Forest::getBsdf(const DifferentialGeometry & dgGeom) const {
        return aggregate->getBsdf(dgGeom);
}

} }
