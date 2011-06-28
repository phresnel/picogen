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

#ifndef PRIMITIVE_WATERPLANE_H_INCLUDED_20100119
#define PRIMITIVE_WATERPLANE_H_INCLUDED_20100119

#include "../geometry.hh"
#include "../primitives/primitive.hh"
#include "../bxdf/mirror.hh"
#include "../basictypes/height-function.hh"

namespace picogen { namespace redshift {
        class HeightFunction;
        class Intersection;
} }

namespace picogen { namespace redshift { namespace primitive {

        SEALED(WaterPlane);

        class WaterPlane
                : public Primitive
                , MAKE_SEALED(WaterPlane)
        {
        public:
                WaterPlane(real_t height, shared_ptr<HeightFunction const> fun, const Color &color);
                ~WaterPlane ();

                bool doesIntersect (Ray const &ray) const;
                optional<Intersection> intersect(Ray const &ray) const;

                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & dgGeom
                ) const {
                        shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));
                        bsdf->add (shared_ptr<Bxdf>(new bsdf::Mirror (color)));
                        return bsdf;
                }

        private:
                WaterPlane();
                WaterPlane(WaterPlane const&);
                WaterPlane &operator = (WaterPlane const&);

                shared_ptr<HeightFunction const> fun;
                real_t height;
                Color color;
        };
} } }

#endif // PRIMITIVE_WaterPlane_H_INCLUDED_20100119
