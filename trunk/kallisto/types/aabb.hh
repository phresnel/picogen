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

#ifndef AABB_HH_INCLUDED_20090403
#define AABB_HH_INCLUDED_20090403

namespace kallisto {
        template <typename point_t> struct bounding_box_t {
                point_t minimum, maximum;
        };
}

namespace kallisto {
        template <typename point_t> class BoundingBox {
        public:
        
                // Use max() instead of infinity() to allow for integer types. 
                BoundingBox ()
                : minimum (
                        traits::numeric_limits<scalar_t>::max(),
                        traits::numeric_limits<scalar_t>::max(),
                        traits::numeric_limits<scalar_t>::max())
                , maximum (
                        -traits::numeric_limits<scalar_t>::max(),
                        -traits::numeric_limits<scalar_t>::max(),
                        -traits::numeric_limits<scalar_t>::max())
                {
                }
                
                
                
                BoundingBox (point_t const & p) 
                : minimum (p), maximum (p) 
                {
                }
                
                
                
                BoundingBox (point_t const & a, point_t const & b) 
                : minimum (
                        min (a.x, b.x),
                        min (a.y, b.y),
                        min (a.z, b.z))
                , maximum (
                        max (a.x, b.x),
                        max (a.y, b.y),
                        max (a.z, b.z))
                {
                }


                
                point_t getMinimum () const { return minimum; }
                point_t getMaximum () const { return maximum; }
                
                
                
                typename traits::get_scalar_type<point_t>::type
                        getMinimumX () const { return minimum.x; }
                typename traits::get_scalar_type<point_t>::type
                        getMinimumY () const { return minimum.y; }
                typename traits::get_scalar_type<point_t>::type
                        getMinimumZ () const { return minimum.z; }
                
                
                
                typename traits::get_scalar_type<point_t>::type
                        getMaximumX () const { return maximum.x; }
                typename traits::get_scalar_type<point_t>::type
                        getMaximumY () const { return maximum.y; }
                typename traits::get_scalar_type<point_t>::type
                        getMaximumZ () const { return maximum.z; }
                
        
        private:
                typedef typename
                               traits::get_scalar_type<point_t>::type scalar_t;

                point_t minimum, maximum;               
                
        };
        
        

        template <typename point_t> inline
        BoundingBox<point_t> merge ( // <-- "union" was already occupied. 
                BoundingBox<point_t> const &box, 
                point_t const & p
        ) {
                return BoundingBox<point_t> (
                        point_t (
                                min (box.getMinimumX(), p.x),
                                min (box.getMinimumY(), p.y),
                                min (box.getMinimumZ(), p.z)
                        ),
                        point_t (
                                max (box.getMaximumX(), p.x),
                                max (box.getMaximumY(), p.y),
                                max (box.getMaximumZ(), p.z)
                        )
                );
        }
        
        
        
        template <typename point_t> inline
        BoundingBox<point_t> merge ( // <-- "union" was already occupied. 
                BoundingBox<point_t> const &l, 
                BoundingBox<point_t> const &r
        ) {
                return BoundingBox<point_t> (
                        point_t (
                                min (l.getMinimumX(), r.getMinimumX()),
                                min (l.getMinimumY(), r.getMinimumY()),
                                min (l.getMinimumZ(), r.getMinimumZ())
                        ),
                        point_t (
                                max (l.getMaximumX(), r.getMaximumX()),
                                max (l.getMaximumY(), r.getMaximumY()),
                                max (l.getMaximumZ(), r.getMaximumZ())
                        )
                );
        }
        
        
        
        template <typename point_t> inline
        bool overlap (
                BoundingBox<point_t> const &l, 
                BoundingBox<point_t> const &r
        ) {
                const bool x  = (l.getMaximumX() >= r.getMinimumX())
                              & (l.getMinimumX() <= r.getMaximumX());
                const bool y  = (l.getMaximumY() >= r.getMinimumY())
                              & (l.getMinimumY() <= r.getMaximumY());
                const bool z  = (l.getMaximumZ() >= r.getMinimumZ())
                              & (l.getMinimumZ() <= r.getMaximumZ());
                return x & y & z;
        }
        
        
        
        template <typename point_t> inline
        bool inside (
                BoundingBox<point_t> const &box, 
                point_t const &point
        ) {
                const bool x  = (point.x >= box.getMinimumX())
                              & (point.x <= box.getMaximumX());
                const bool y  = (point.y >= box.getMinimumY())
                              & (point.y <= box.getMaximumY());
                const bool z  = (point.z >= box.getMinimumZ())
                              & (point.z <= box.getMaximumZ());
                return x & y & z;
        }
        
        
        
        template <typename point_t> inline
        traits::get_scalar_type<point_t> volume (
                BoundingBox<point_t> const &box 
        ) {
                typedef vector_t <CARTESIAN, traits::get_scalar_type<point_t> >
                        vec_t;

                // We do that vector cast for the case where point_t is
                // of non-cartesian type.
                const vec_t diff =
                      vector_cast<vec_t> (box.getMaximum() - box.getMinimum());

                return diff.x * diff.y * diff.z;
        }
        
        
        
        template <
                bool do_normalize_ray_direction,
                typename point_t,
                typename direction_t
        > inline
        optional<
                tuple<
                        typename traits::get_scalar_type<direction_t>::type,
                        typename traits::get_scalar_type<direction_t>::type
                >
        >
        intersect (
                Ray<point_t, direction_t> const & ray_,
                BoundingBox<point_t> const & box
        ) {
                typedef
                    typename traits::get_scalar_type<direction_t>::type real_t;
                    
                Ray<point_t, direction_t> ray;
                if (do_normalize_ray_direction) {
                        ray.position = ray_.position;
                        ray.direction = normalize (ray_.direction);
                } else {
                        ray = ray_;
                }
                        
                //TODO    
                real_t t0 = 0;//ray.minT;
                real_t t1 = 100;//ray.maxT;
                
                // X
                {
                        real_t i = real_t(1) / ray.direction.x;
                        real_t near = scalar_cast<real_t>(box.getMinimumX() - ray.position.x) * i;
                        real_t far  = scalar_cast<real_t>(box.getMaximumX() - ray.position.x) * i;
                        
                        if (near > far) swap (near, far);
                        
                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;
                        
                        if (t0 > t1) return false;
                }
                
                // Y
                {
                        real_t i = real_t(1) / ray.direction.y;
                        real_t near = scalar_cast<real_t>(box.getMinimumY() - ray.position.y) * i;
                        real_t far  = scalar_cast<real_t>(box.getMaximumY() - ray.position.y) * i;
                        
                        if (near > far) swap (near, far);
                        
                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;
                        
                        if (t0 > t1) return false;
                }
                
                // Z
                {
                        real_t i = real_t(1) / ray.direction.z;
                        real_t near = scalar_cast<real_t>(box.getMinimumZ() - ray.position.z) * i;
                        real_t far  = scalar_cast<real_t>(box.getMaximumZ() - ray.position.z) * i;
                        
                        if (near > far) swap (near, far);
                        
                        t0 = near > t0 ? near : t0;
                        t1 = far < t1 ? far : t1;
                        
                        if (t0 > t1) return false;
                }
//                std::cout<<t0<<"::"<<t1<<std::endl;
//                std::cout<<ray.minT<<".."<<ray.maxT<<std::endl;
                return make_tuple (t0, t1);
        }        
  
}

namespace kallisto { namespace unit_test {
        class test_bounding_box_t {
        private:
                const static test_bounding_box_t test;
                test_bounding_box_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // SPHERE_H_INCLUDED_20090223
