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

#ifndef PRIMITIVE_HORIZONPLANE_H_INCLUDED_20100119
#define PRIMITIVE_HORIZONPLANE_H_INCLUDED_20100119

#include "../setup.hh"
#include "../basictypes/intersection.hh"
#include "../primitives/primitive.hh"
#include "../material/lambertian.hh"

namespace redshift { namespace primitive {

        DefineFinalizer(HorizonPlane);

        class HorizonPlane
                : public Primitive
                , DoFinalize(HorizonPlane)
        {
        public:
                HorizonPlane(real_t height, const Color &color);
                ~HorizonPlane ();

                bool doesIntersect (RayDifferential const &ray) const;
                bool doesIntersect (Ray const &ray) const;

                optional<Intersection>
                        intersect(RayDifferential const &ray) const;

                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & dgGeom
                ) const {
                        shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));
                        bsdf->add (shared_ptr<Bxdf>(new bsdf::Lambertian (color)));
                        return bsdf;
                }

        private:
                HorizonPlane();
                HorizonPlane(HorizonPlane const&);
                HorizonPlane &operator = (HorizonPlane const&);

                real_t height;
                Color color;
        };
} }

#endif // PRIMITIVE_HORIZONPLANE_H_INCLUDED_20100119
