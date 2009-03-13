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

namespace redshift { namespace primitive {
        
        DefineFinalizer(ClosedSphere);
        
        class ClosedSphere : public Primitive, DoFinalize(ClosedSphere) {
        public:
                ClosedSphere();
                ~ClosedSphere ();
                
                // If only the information is need whether the ray hits or not.
                bool doesIntersect (RayDifferential const &ray);

                // Full fledged information including Normal, U/V, etc.
                tuple<bool,Intersection> intersect(RayDifferential const &ray);                
        };
} }

#endif // PRIMITIVE_CLOSEDSPHERE_H_INCLUDED_20090313
