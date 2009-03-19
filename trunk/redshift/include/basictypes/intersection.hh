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

namespace redshift { namespace primitive { class Primitive; } }

namespace redshift {
        DefineFinalizer(Intersection);
        
        class Intersection : DoFinalize(Intersection) {
        public:
                Intersection (
                        shared_ptr<redshift::primitive::Primitive const>,
                        DifferentialGeometry const &geom);
                        
                Intersection () ;
                
                Intersection (Intersection const &) ;                
                Intersection & operator = (Intersection const &);                
                
                real_t getDistance() const ;                
                Normal getNormal() const ;                
                DifferentialGeometry getDifferentialGeometry() const ;
                
                shared_ptr<primitive::Primitive const> getPrimitive () const ;
                
        private:               

                shared_ptr<primitive::Primitive const> primitive;
                DifferentialGeometry differentialGeometry;
        };
}

#endif // INTERSECTION_H_INCLUDED_20090301
