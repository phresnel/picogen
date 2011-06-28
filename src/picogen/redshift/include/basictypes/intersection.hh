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

#ifndef INTERSECTION_H_INCLUDED_20090301
#define INTERSECTION_H_INCLUDED_20090301

#include "../geometry.hh"
#include "../smart_ptr.hh"
#include "../sealed.hh"
#include "../basictypes/differentialgeometry.hh"


namespace picogen { namespace redshift { class Primitive; } }

namespace picogen { namespace redshift {
        SEALED(Intersection);

        class Intersection : MAKE_SEALED(Intersection) {
        public:
                Intersection (
                        redshift::Primitive const &,
                        DifferentialGeometry const &geom);

                Intersection () ;

                Intersection (Intersection const &) ;
                Intersection & operator = (Intersection const &);

                Point getCenter() const;
                Normal getGeometricNormal() const ;
                Normal getShadingNormal() const ;
                DifferentialGeometry getDifferentialGeometry() const ;

                //shared_ptr<Primitive const> getPrimitive () const ;
                Primitive const &getPrimitive () const ;

                void applyTransform (Transform const &t);
        private:

                Primitive const *primitive;
                DifferentialGeometry differentialGeometry;
        };
} }

#endif // INTERSECTION_H_INCLUDED_20090301
