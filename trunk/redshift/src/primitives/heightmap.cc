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

#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"

#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"
#include "../../include/basictypes/sample.hh"


#include "../../include/primitives/primitive.hh"
#include "../../include/primitives/heightmap.hh"


namespace redshift { namespace primitive {



Heightmap::Heightmap(shared_ptr<HeightFunction const> fun, real_t detail_) 
: function (fun), detail (detail_) {
        for (int i=0; i<1024; ++i) {
                const real_t x = 4000*((real_t)rand() / (real_t)RAND_MAX-0.5);
                const real_t z = 4000*((real_t)rand() / (real_t)RAND_MAX-0.5);
                const real_t h = (*function) (x, z);
                boundingBox = merge (boundingBox,  
                                     vector_cast<Point>(Vector(x,h,z)));
        }
        // TODO: add some epsilon to height (see 1024*1024 render for why)
        
        /*boundingBox = BoundingBox (
                Point(scalar_cast<fixed_point_t>(-100),
                        scalar_cast<fixed_point_t>(-10),
                        scalar_cast<fixed_point_t>(-100)),
                Point(scalar_cast<fixed_point_t>(100),
                        scalar_cast<fixed_point_t>(10),
                        scalar_cast<fixed_point_t>(100)));*/
}



Heightmap::~Heightmap () {
}


                
bool Heightmap::doesIntersect (RayDifferential const &ray) const {
        // TODO shouldn't do full intersect()
        return intersect (ray);
}



optional<Intersection>
Heightmap::intersect(RayDifferential const &ray) const {
        return intersect (ray, 0.0);
}



optional<Intersection>
Heightmap::intersect(RayDifferential const &ray, real_t minT) const {

        // ugly, get rid of that namespace qualify
        typedef kallisto::Point<kallisto::CARTESIAN,real_t> PointR;
        
        // Early exit?
        const optional<tuple<real_t,real_t> > B = 
                                kallisto::intersect<true> (ray, boundingBox);
        if (!B) {
                return false;
        }
                
        if (get<0>(*B) > minT) {
                minT = get<0>(*B); 
        }
        real_t maxT = get<1>(*B);
//        std::cout << minT << ":" << maxT << std::endl;
        
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
        
        // Note that in the for-condition, we add step to maxT, which is a hack
        // to prevent holes in the terrain for when the ray leaves the bounding
        // box at the bottom, without changing the sign first.
        for (real_t d = minT; d < maxT + step; d += step) {
                
                const Point curr = ray(d);
                const PointR currr = vector_cast<PointR> (curr);
                const real_t h = (*function) (currr.x, currr.z);
                if ((currr.y < h) != sign) {
                        const real_t s = 0.1;

                        const Vector
                                u (s, (*function) (currr.x+s, currr.z)-h, 0.0),
                                v (0.0, (*function) (currr.x, currr.z+s)-h, s);
                        
                        const Normal n_ (vector_cast<Normal>(
                                          cross (normalize(u), normalize(v))));

                        const Normal n (sign ? n_ : -n_);

                        return Intersection (shared_from_this(),
                                             DifferentialGeometry(d,curr,n));
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
        
        return false;
}



// TODO implement case where the screen is rendered top->down (vline wise)
// TODO implement case where the camera orientation is ugly

optional<Intersection> Heightmap::intersect(Sample const &sample) const {

        const size_t width = sample.renderTarget->getWidth();
        const int u  = ((int)sample.imageCoordinates.u<0)         ? (0)
                     : ((size_t)sample.imageCoordinates.u>=width) ? (width-1)
                     : ((int)sample.imageCoordinates.u);

        if (depthBuffer.size() != width) {
                depthBuffer.clear();
                depthBuffer.resize (width);                
                
                const optional<Intersection> t = intersect (sample.primaryRay);
                
                if(t)depthBuffer [u] = t->getDistance();
                else depthBuffer [u] = constants::zero;

                return t;
        } else {
                const optional<Intersection> t = intersect (sample.primaryRay, 
                                                            depthBuffer [u]);
                if(t) depthBuffer [u] = t->getDistance();
                return t;
        }        
}



} }