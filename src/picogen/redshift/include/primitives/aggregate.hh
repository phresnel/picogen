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

#ifndef AGGREGATE_HH_INCLUDED_20100120
#define AGGREGATE_HH_INCLUDED_20100120

#include "primitive.hh"
#include "boundprimitive.hh"
#include <stdexcept>

namespace picogen { namespace redshift { namespace primitive {
        class Aggregate : public Primitive {
        public:
                virtual ~Aggregate () {}

                virtual shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & /*dgGeom*/
                ) const {
                        throw std::logic_error (
                                "getBsdf() called on Aggregate.");
                }
        };

        class BoundAggregate : public BoundPrimitive {
        public:
                virtual ~BoundAggregate () {}

                virtual shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry & /*dgGeom*/
                ) const {
                        throw std::logic_error (
                                "getBsdf() called on BoundAggregate.");
                }
        };
} } }

#endif // AGGREGATE_HH_INCLUDED_20100120
