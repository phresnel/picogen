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

#ifndef BOUNDINSTANCE_HH_INCLUDED_20100815
#define BOUNDINSTANCE_HH_INCLUDED_20100815

#include "../geometry.hh"
#include "../sealed.hh"
#include "../primitives/boundprimitive.hh"

namespace picogen { namespace redshift { namespace primitive {

        SEALED(BoundInstance);

        class BoundInstance
                : public BoundPrimitive
                , MAKE_SEALED(BoundInstance)
        {
        public:
                BoundInstance(Transform const &,
                              shared_ptr<BoundPrimitive> primitive);
                ~BoundInstance ();

                bool doesIntersect (Ray const &ray) const;
                optional<Intersection> intersect(Ray const &ray) const;

                BoundingBox boundingBox() const;

                shared_ptr<Bsdf> getBsdf(const DifferentialGeometry &) const;
                
                void prepare (const Scene &s);

        private:
                BoundInstance();
                BoundInstance(BoundInstance const&);
                BoundInstance &operator = (BoundInstance const&);

                Transform instanceToWorld, worldToInstance;
                shared_ptr<BoundPrimitive> primitive;
        };
} } }

#endif // BOUNDINSTANCE_HH_INCLUDED_20100815
