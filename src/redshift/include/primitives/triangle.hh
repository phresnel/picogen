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

#ifndef TRIANGLE_HH_INCLUDED_20100812
#define TRIANGLE_HH_INCLUDED_20100812

#include "../geometry.hh"
#include "../basictypes/intersection.hh"
#include "../primitives/boundprimitive.hh"
#include "../material/lambertian.hh"
#include "../material/mirror.hh"

namespace redshift { namespace primitive {

        SEALED(Triangle);

        class Triangle
                : public BoundPrimitive
                , MAKE_SEALED(Triangle)
        {
        public:
                struct Vertex {
                        Point position;

                        Vertex() {}
                        Vertex (Point position)
                        : position (position)
                        {}
                };

                Triangle(Vertex A, Vertex B, Vertex C);
                ~Triangle ();

                BoundingBox boundingBox () const;

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
                Triangle();
                Triangle(Triangle const&);
                Triangle &operator = (Triangle const&);

                Vertex A, B, C;
        };
} }

#endif // TRIANGLE_HH_INCLUDED_20100812
