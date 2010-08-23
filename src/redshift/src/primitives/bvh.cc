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

#include "../../include/constants.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/primitives/bvh.hh"
#include "../../include/tuple.hh"
#include "../../include/optional.hh"


namespace redshift { namespace primitive {

//-----------------------------------------------------------------------------
// Node
//-----------------------------------------------------------------------------
struct BvhNode {
        std::vector<shared_ptr<BoundPrimitive> > primitives;
        scoped_ptr<BvhNode> childA, childB;
        BoundingBox boundingBox;

        typedef std::vector<shared_ptr<BoundPrimitive> >::iterator It;
        typedef std::vector<shared_ptr<BoundPrimitive> >::const_iterator CIt;

        void add (shared_ptr<BoundPrimitive> prim) {
                primitives.push_back (prim);
        }

        void compile() {
                boundingBox.reset();
                for (It it = primitives.begin(); it!=primitives.end(); ++it) {
                        boundingBox = merge (boundingBox,
                                             ((**it).boundingBox()));
                }

                if (primitives.size() <= 5) {
                        return;
                }

                int splitAxis_ = 0;
                if (boundingBox.size(1) > boundingBox.size(splitAxis_)) splitAxis_ = 1;
                if (boundingBox.size(2) > boundingBox.size(splitAxis_)) splitAxis_ = 2;
                const int splitAxis = splitAxis_;
                const real_t center = boundingBox.center(splitAxis);

                childA.reset(new BvhNode);
                childB.reset(new BvhNode);

                for (It it = primitives.begin(); it!=primitives.end(); ++it) {
                        const real_t s = (**it).boundingBox().center(splitAxis);
                        //std::cout << s << " ";
                        if (s < center) {
                                //std::cout << " <" << std::endl;
                                childA->add(*it);
                        } else if (s > center) {
                                //std::cout << " >" << std::endl;
                                childB->add(*it);
                        } else if (childA->primitives.size()<childB->primitives.size()) {
                                //std::cout << " <" << std::endl;
                                childA->add(*it);
                        } else {
                                //std::cout << " >" << std::endl;
                                childB->add(*it);
                        }
                }

                if (childA->primitives.empty() || childB->primitives.empty()) {
                        childA.reset();
                        childB.reset();
                } else {
                        primitives.clear();
                        childA->compile();
                        childB->compile();
                }
        }

        bool doesIntersect (Ray const &ray) const {
                if (!does_intersect<false>(ray, boundingBox))
                        return false;
                if (childA && childA->doesIntersect(ray))
                        return true;
                if (childB && childB->doesIntersect(ray))
                        return true;

                for (CIt it=primitives.begin(); it!=primitives.end(); ++it) {
                        if ((*it)->doesIntersect(ray))
                                return true;
                }

                return false;
        }

        bool intersectsBound(Ray const &ray) const {
                return does_intersect<false>(ray, boundingBox);
        }

        real_t intersectBound (Ray const &ray) const {
                const optional<tuple<real_t,real_t> > t =
                        kallisto::intersect<false>(ray, boundingBox);
                if (!t) return constants::infinity;
                return get<0>(*t);
        }

        bool hasChildren() const {
                return (bool)childA || (bool)childB;
        }

        optional<Intersection> intersect(Ray const &ray) const {

                using redshift::optional;
                using redshift::tuple;
                using redshift::get;

                real_t nearest = constants::infinity;
                real_t tmp;
                optional<Intersection> nearestI, tmpI;

                for (CIt it=primitives.begin();
                        it!=primitives.end(); ++it
                ) {
                        if ((tmpI=(*it)->intersect (ray))
                           && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }
                }

                if (hasChildren()) {
                        const real_t min_t[2] = {
                                childA->intersectBound(ray),
                                childB->intersectBound(ray)
                        };
                        const BvhNode* children[2] = {
                                childA.get(),
                                childB.get()
                        };
                        const int near = min_t[0] < min_t[1] ? 0 : 1;
                        const int far = 1 - near;

                        if (min_t[near] < nearest)
                        if ((tmpI = children[near]->intersect(ray))
                         && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }

                        if (min_t[far] < nearest)
                        if ((tmpI = children[far]->intersect(ray))
                         && (tmp=length(ray.position-tmpI->getCenter())) < nearest
                        ) {
                                nearest = tmp;
                                nearestI = tmpI;
                        }
                }

                return nearestI;
        }
};


//-----------------------------------------------------------------------------
// Bvh
//-----------------------------------------------------------------------------
Bvh::Bvh(shared_ptr<BvhNode> root) : root(root) {
}



Bvh::~Bvh() {
}



BoundingBox Bvh::boundingBox() const {
        return root->boundingBox;
}



bool Bvh::doesIntersect (Ray const &ray) const {
        return root->doesIntersect(ray);
}



optional<Intersection> Bvh::intersect(Ray const &ray) const {
        if (!root->intersectsBound(ray))
                return optional<Intersection>();
        return root->intersect(ray);
}



//-----------------------------------------------------------------------------
// Builder
//-----------------------------------------------------------------------------
BvhBuilder::BvhBuilder() : root(new BvhNode) {
}



void BvhBuilder::add (shared_ptr<BoundPrimitive> prim) {
        root->add(prim);
}


shared_ptr<Bvh> BvhBuilder::toBvh() {
        root->compile();
        shared_ptr<Bvh> ret = shared_ptr<Bvh>(new Bvh(root));
        root.reset();
        return ret;
}


} }
