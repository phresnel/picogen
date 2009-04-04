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

#ifndef SHAPE_H_INCLUDED_20090301
#define SHAPE_H_INCLUDED_20090301

namespace redshift { namespace shape {
        class Shape {
        public:
                // If only the information is need whether the ray hits or not.
                virtual bool doesIntersect (Ray const &ray) const = 0;

                // If only the intersection distance is needed.
                // TODO: is below thing really needed?
                //virtual tuple<bool,real_t>bool intersect(Ray const &ray) = 0;

                // Full fledged information including Normal, U/V, etc.
                virtual
                optional<DifferentialGeometry> intersect(Ray const &ray)
                const = 0;
        };
} }

#endif // SHAPE_H_INCLUDED_20090301
