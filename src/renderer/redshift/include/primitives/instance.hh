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

#ifndef INSTANCE_HH_INCLUDED_20100813
#define INSTANCE_HH_INCLUDED_20100813

#include "../geometry.hh"
#include "../sealed.hh"
#include "../primitives/primitive.hh"

namespace redshift { namespace primitive {

        SEALED(Instance);

        class Instance
                : public Primitive
                , MAKE_SEALED(Instance)
        {
        public:
                Instance(Transform const &, shared_ptr<Primitive> primitive);
                ~Instance ();

                bool doesIntersect (Ray const &ray) const;
                optional<Intersection> intersect(Ray const &ray) const;

                void prepare (const Scene &);
                
                shared_ptr<Bsdf> getBsdf(const DifferentialGeometry &) const;               

        private:
                Instance();
                Instance(Instance const&);
                Instance &operator = (Instance const&);

                Transform instanceToWorld, worldToInstance;
                shared_ptr<Primitive> primitive;
        };
} }

#endif // INSTANCE_HH_INCLUDED_20100813
