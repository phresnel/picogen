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

#include "../sealed.hh"
#include "../geometry.hh"
#include "../primitives/boundprimitive.hh"

namespace picogen { namespace redshift {
        class Material;
        class Intersection;
} }

namespace picogen { namespace redshift { namespace primitive {

        SEALED(Triangle);

        class Triangle
                : public BoundPrimitive
                , MAKE_SEALED(Triangle)
        {
        public:
                struct TextureCoordinates {
                        real_t s, t;

                        TextureCoordinates() : s(0), t(0) {}
                        TextureCoordinates(real_t s, real_t t) : s(s), t(t) {}
                };

                struct Vertex {
                        Point position;

                        TextureCoordinates textureCoordinates;

                        Vertex() {}
                        Vertex (Point position,
                                TextureCoordinates tex
                               )
                        : position (position)
                        , textureCoordinates(tex)
                        {}
                };

                Triangle(Vertex A, Vertex B, Vertex C,
                         shared_ptr<Material> material);
                ~Triangle ();

                Vertex a () const { return A; }
                Vertex b () const { return B; }
                Vertex c () const { return C; }

                BoundingBox boundingBox () const;

                bool doesIntersect (Ray const &ray) const;

                optional<Intersection> intersect(Ray const &ray) const;

                shared_ptr<Bsdf> getBsdf(const DifferentialGeometry &) const;

                shared_ptr<Material> material() const;
        private:
                Vertex A, B, C;
                shared_ptr<Material> material_;
        };
} } }

#endif // TRIANGLE_HH_INCLUDED_20100812
