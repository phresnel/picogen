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

#ifndef PRIMITIVE_CLOSEDSPHERE_H_INCLUDED_20090313
#define PRIMITIVE_CLOSEDSPHERE_H_INCLUDED_20090313

#include "../geometry.hh"
#include "../basictypes/intersection.hh"
#include "../shapes/closedsphere.hh"
#include "../primitives/primitive.hh"
#include "../material/lambertian.hh"
#include "../material/mirror.hh"

namespace redshift { namespace primitive {

        SEALED(ClosedSphere);

        class ClosedSphere
                : public Primitive
                , MAKE_SEALED(ClosedSphere)
        {
        public:
                ClosedSphere(Point const & center, real_t radius);
                ~ClosedSphere ();

                bool doesIntersect (Ray const &ray) const;

                optional<Intersection> intersect(Ray const &ray) const;

                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & dgGeom
                ) const {
                        shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));
                        bsdf->add (shared_ptr<Bxdf>(new bsdf::Lambertian (Color(1))));
                        return bsdf;
                }

        private:
                shape::ClosedSphere sphereData;

                ClosedSphere();
                ClosedSphere(ClosedSphere const&);
                ClosedSphere &operator = (ClosedSphere const&);
        };
} }

#endif // PRIMITIVE_CLOSEDSPHERE_H_INCLUDED_20090313
