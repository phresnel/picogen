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

#include "../../include/primitives/list.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/constants.hh"

namespace redshift { namespace primitive {



List::List ()
{
}



List::~List () {
}



bool List::doesIntersect (Ray const &ray) const {
        optional<Intersection> tmp;
        for (Primitives::const_iterator it=primitives.begin();
                it!=primitives.end(); ++it
        ) {
                if ((*it)->doesIntersect (ray)) {
                        return true;
                }
        }
        return false;
}



optional<Intersection> List::intersect(Ray const &ray) const {
        real_t nearest = constants::real_max, tmp;
        optional<Intersection> nearestI, tmpI;
        for (Primitives::const_iterator it=primitives.begin();
                it!=primitives.end(); ++it
        ) {
                if ((tmpI=(*it)->intersect (ray))
                   && (tmp=tmpI->getDistance()) < nearest
                ) {
                        nearest = tmp;
                        nearestI = tmpI;
                }
        }
        return nearestI;
}



void List::add (shared_ptr<Primitive> primitive) {
        primitives.push_back (primitive);
}



void List::prepare (const Scene &scene) {
        for (Primitives::const_iterator it=primitives.begin();
                it!=primitives.end(); ++it
        ) {
                (*it)->prepare (scene);
        }
}



} }
