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



Heightmap::Heightmap(shared_ptr<HeightFunction const> fun, real_t detail_) 
: function (fun), detail (detail_) {
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
        
        real_t step;
        const real_t step_epsilon = 0.1;
        
        if (ray.hasDifferentials) {
                step = detail*
                        min(
                                length(vector_cast<Vector> (ray(0)-ray.rx(0))),
                                length(vector_cast<Vector> (ray(0)-ray.ry(0)))
                        );
                if (step < step_epsilon) {
                        step = step_epsilon;
                }
        } else {
                step = 0.1;
                if (step < step_epsilon) {
                        step = step_epsilon;
                }
        }        
        
        // get initial sign
        const PointR first_ = vector_cast<PointR>(ray(0));
        const real_t first = (*function) (first_.x, first_.z);
        const real_t sign = first > 0;
        
        for (real_t d = 0.0; d < 100.0; d += step) {
                
                const Point curr = ray(d);
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
                                                d,
                                                curr,
                                                n
                                        )
                        ));
                }                
                if (ray.hasDifferentials) {
                        step = detail*
                                min(
                                 length(vector_cast<Vector>(ray(d)-ray.rx(d))),
                                 length(vector_cast<Vector>(ray(d)-ray.ry(d)))
                                );
                        if (step < step_epsilon) {
                                step = step_epsilon;
                        }
                } else {
                        step += step * 0.05;
                }                
        }
        
        return make_tuple (false, Intersection());
}



} }