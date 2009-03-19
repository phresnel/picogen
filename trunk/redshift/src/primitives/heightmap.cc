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

#include "../../include/setup.hh"
#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/primitives/primitive.hh"
#include "../../include/primitives/heightmap.hh"

namespace redshift { namespace primitive {



Heightmap::Heightmap(shared_ptr<HeightFunction const> fun) 
: function (fun) {
}



Heightmap::~Heightmap () {
}


                
bool Heightmap::doesIntersect (RayDifferential const &ray) const {
        return get<0>(intersect (ray));
}



tuple<bool,Intersection>
Heightmap::intersect(RayDifferential const &ray) const {

        // ugly, get rid of that namespace qualify
        typedef kallisto::Point<kallisto::CARTESIAN,real_t> PointR;
        
        real_t step = 0.5*0.125;
        
        // get initial sign
        const PointR first_ = vector_cast<PointR>(ray(0));
        const real_t first = (*function) (first_.x, first_.z);
        const real_t sign = first > 0;
        
        for (real_t distance = 0.0; distance < 100.0; distance += step) {
                
                const Point curr = ray(distance);
                const PointR currr = vector_cast<PointR> (curr);
                const real_t h = (*function) (currr.x, currr.z);
                if ((currr.y < h) != sign) {
                        real_t const s = 0.1;
                        const Vector u (s, (*function) (currr.x+s, currr.z)-h, 0.0);
                        const Vector v (0.0, (*function) (currr.x, currr.z+s)-h, s);
                        const Normal n_ = vector_cast<Normal>(cross (normalize(u), normalize(v)));
                        const Normal n = sign?n_:-n_;
                        return make_tuple (true, 
                                Intersection(shared_from_this(),
                                        DifferentialGeometry(
                                                distance,
                                                curr,
                                                n
                                        )
                        ));
                }
                step += step * 0.05; 
        }
        
        return make_tuple (false, Intersection());
}



} }