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

#ifndef PRIMITIVE_H_INCLUDED_20090301
#define PRIMITIVE_H_INCLUDED_20090301

#include "../geometry.hh"
#include "../optional.hh"
#include "../smart_ptr.hh"

namespace redshift {
        class Scene;
        class Intersection;
        class Bsdf;
        class DifferentialGeometry;
}
namespace redshift {
        class Primitive
        : public enable_shared_from_this<Primitive> {
        public:
                virtual bool doesIntersect (Ray const &) const = 0;
                virtual optional<Intersection> intersect(Ray const &) const = 0;

                virtual void prepare (const Scene &) {}

                virtual shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry &) const = 0;

                virtual ~Primitive () {}
        };
}

#endif // PRIMITIVE_H_INCLUDED_20090301
