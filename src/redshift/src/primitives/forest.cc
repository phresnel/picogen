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

#include "../../include/basictypes/height-function.hh"
#include "../../include/basictypes/distribution-function.hh"


namespace redshift { namespace primitive {


//-----------------------------------------------------------------------------
// Bvh
//-----------------------------------------------------------------------------
Forest::Forest(
        shared_ptr<HeightFunction const> heightFunction,
        shared_ptr<DistributionFunction const> distFun,
        unsigned int totalTargettedCount
)
{
        BvhBuilder builder;

        shared_ptr<LSystemTree> A (new LSystemTree (
                "#scale=0.5;\n"
                "#diascale=0.04;\n"
                "r1=0.9;\n"
                "r2=0.6;\n"
                "a0=45;\n"
                "a2=45;\n"
                "d=137.5;\n"
                "wr=0.707;\n"
                "\n"
                "axiom: A(1, 10);\n"
                "\n"
                "p1 : A(l,w) --> dia(w) f(l) [down(a0)       B(l*r2, w*wr)] rollright(d) A(l*r1, w*wr);\n"
                "p2 : B(l,w) --> dia(w) f(l) [right(a2) vert C(l*r2, w*wr)] C(l*r1, w*wr);\n"
                "p3 : C(l,w) --> dia(w) f(l) [left(a2)  vert B(l*r2, w*wr)] B(l*r1, w*wr);\n",
                10,//13
                6
        ));

        shared_ptr<LSystemTree> B (new LSystemTree (
                        "\
                        d1 = 94.74;\
                        d2 = 132.63;\
                        a = 18.95;\
                        lr = 1.109;\
                        vr = 1.732;\
                        #scale=0.015;\
                        #diascale=3.6;\
                        #texuscale=1.0;\
                        #texvscale=0.01;\
                        #barktexture=\"bark.bmp\";\
                        #leaftexture=\"leaf.bmp\";\
\
                        axiom: dia(1.0) f(0) rollright(45) A;\
                        p1: A --> dia(vr) f(100)\
                                [down(a) f(100) A] rollright(d1)\
                                [down(a) f(100) A] rollright(d2)\
                                [down(a) f(100) A];\
                        p2: f(l) --> f(l*lr);\
                        p3: dia(w) --> dia(w*vr);\
                        "
                ,
                6,//7
                6
        ));

        std::cout << "{{{" << std::endl;
        srand(44);
        for (int i=0; i<5000; ++i) {
                const Transform t =
                        Transform::translation(
                                (-0.5 + rand() / (double)RAND_MAX) * 1500,
                                0,
                                (-0.5 + rand() / (double)RAND_MAX) * 1500)
                        *
                        Transform::rotationY(
                                rand()/(double)RAND_MAX * 3.14159*2)
                        ;
                if (true || rand()%3 == 0)
                        builder.add(shared_ptr<BoundPrimitive>(
                                        new BoundInstance(t, B)));
                else
                        builder.add(shared_ptr<BoundPrimitive>(
                                        new BoundInstance(t, A)));
        }
        std::cout << "---" << std::endl;
        aggregate = builder.toBvh();
        std::cout << "}}}" << std::endl;
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
