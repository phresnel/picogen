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

#ifndef SETUP_HH_INCLUDED_20090303
#define SETUP_HH_INCLUDED_20090303

#include "../../kallisto/common.hh"
#include <utility>

#define DefineFinalizer(x) \
        class x##Finalize { \
        private: \
                friend class x ; \
                x##Finalize() {} \
                x##Finalize(x##Finalize const &) {} \
        }
#define DoFinalize(x) virtual x##Finalize



///////////////////////////////////////////////////////////////////////////////
// Assimilate kallisto-types and define some convenience types.
///////////////////////////////////////////////////////////////////////////////
namespace redshift {

        enum {
                ALLOW_LOGSPAM = 1
        };

        typedef float real_t;
        //typedef kallisto::fixed_point_t<int64_t,16>          fixed_point_t;
        typedef float fixed_point_t; //<-- lasttime I checked, floats where
                                       // at only roughly 70% of runtime
                                       // compared to int


        // Cartesian Types
        typedef kallisto::Point<kallisto::CARTESIAN,
                                fixed_point_t>               CartesianPoint;
        typedef kallisto::Point<kallisto::CARTESIAN,
                                real_t>                      CartesianPointF;
        typedef kallisto::Normal<kallisto::CARTESIAN,real_t> CartesianNormal;
        typedef kallisto::Vector<kallisto::CARTESIAN,real_t> CartesianVector;


        // Std-Types
        typedef CartesianPoint  Point;
        typedef CartesianPointF PointF;
        typedef CartesianNormal Normal;
        typedef CartesianVector Vector;

        typedef kallisto::Vector<kallisto::CARTESIAN,
                                fixed_point_t> PointCompatibleVector;

        typedef kallisto::BoundingBox<Point>            BoundingBox;
        typedef kallisto::BoundingBox<PointF>           BoundingBoxF;
        typedef kallisto::Sphere<Point,real_t>          Sphere;
        typedef kallisto::Ray   <Point,Vector>          Ray;
        typedef kallisto::Transform<real_t> Transform;
        typedef kallisto::RayDifferential<Point,Vector> RayDifferential;
        typedef kallisto::OrthonormalBasis<Vector>      OrthonormalBasis;

        using kallisto::intersect;
        using kallisto::does_intersect;
        using kallisto::vector_cast;
        using kallisto::scalar_cast;

        using kallisto::array;
}


namespace redshift {

        template <uint32_t m, uint32_t a, uint32_t c>
        class LCG {
        public:
                LCG (uint32_t s) : curr(s) {}

                uint32_t operator () () {
                        return curr = ((uint64_t)(((uint64_t)a*curr)&0xFFFFFFFFUL) + c) % m;
                }

                void seed (uint32_t s) {
                        curr = s;
                }
        private:
                uint32_t curr;
        };

        template <uint32_t m, uint32_t a, uint32_t c>
        class LCGf : private LCG<m,a,c> {
        public:
                LCGf(uint32_t s) : LCG<m,a,c>(s) {}

                float operator () () {
                        return LCG<m,a,c>::operator()() * (1.f/(1.f+m));
                }

                void seed (uint32_t s) {
                        LCG<m,a,c>::seed(s);
                }
        };

        class Random {
        public:
                Random (uint32_t A, uint32_t B, uint32_t C, uint32_t D)
                : rand(A,B,C,D)
                {}

                Random (Random const &x)
                : rand(x.rand)
                {}

                Random& operator = (Random const &rhs) {
                        rand = rhs.rand;
                        return *this;
                }

                double operator () () {
                        return rand();
                }

                void skip (uint32_t u) {
                        rand.skip(u);
                }
        private:
                Random () ;

                kallisto::random::marsaglia::UNI rand;
        };
}



namespace redshift {
        template <typename T> inline T min (T const &lhs, T const &rhs) {
                return lhs < rhs ? lhs : rhs;
        }

        template <typename T> inline T max (T const &lhs, T const &rhs) {
                return lhs > rhs ? lhs : rhs;
        }

        using std::pair;
        using std::make_pair;

        // Should be in kallisto.
        class Interval {
        public:
                Interval (real_t min, real_t max)
                : min_(min), max_(max)
                {
                        assert_valid();
                }
                void setMin (real_t min) { min_ = min; assert_valid(); }
                void setMax (real_t max) { max_ = max; assert_valid(); }
                real_t min() const { return min_; }
                real_t max() const { return max_; }
                real_t mag() const { return max_-min_; }
        private:
                void assert_valid() {}
                real_t min_, max_;
        };
}

#include "auxiliary/mutex.hh"
#include "auxiliary/scopedlock.hh"

#include "basictypes/rgb.hh"
#include "basictypes/spectrum.hh"

namespace redshift {
        //class Rgb;
        typedef Rgb Color;
}

///////////////////////////////////////////////////////////////////////////////
// Some constants.
///////////////////////////////////////////////////////////////////////////////
namespace redshift {
        namespace static_config {
                enum { debug = 0 }; // should be set by #define macro
        }

        namespace constants {

                // TODO clean up epsilon value
                template <typename> struct epsilon_value;
                template <> struct epsilon_value<float> {
                        static float const value;
                };
                template <> struct epsilon_value<double> {
                        static double const value;
                };
                template <> struct epsilon_value<long double> {
                        static long double const value;
                };


                // TODO clean up infinity value
                template <typename> struct infinity_value;
                template <> struct infinity_value<float> {
                        static float const value;
                };
                template <> struct infinity_value<double> {
                        static double const value;
                };
                template <> struct infinity_value<long double> {
                        static long double const value;
                };

                template <typename> struct real_max_value;
                template <> struct real_max_value<float> {
                        static float const value;
                };
                template <> struct real_max_value<double> {
                        static double const value;
                };
                template <> struct real_max_value<long double> {
                        static long double const value;
                };


                // TODO clean up pi value
                template <typename> struct pi_value;
                template <> struct pi_value<float> {
                        static float const value;
                };
                template <> struct pi_value<double> {
                        static double const value;
                };
                template <> struct pi_value<long double> {
                        static long double const value;
                };


                extern real_t const epsilon;
                extern real_t const real_max;
                extern real_t const pi;
                extern real_t const inv_pi;
                extern real_t const infinity;

                template<typename T> inline T km2m (T const &v) {
                        return T(1000) * v;
                }

                extern real_t const earth_radius_m;
                extern real_t const sun_radius_m;
                extern real_t const jupiter_radius_m;

                extern real_t const zero;
                extern real_t const one;
                extern real_t const two;
        }
}

#include "smart_ptr.hh"
#include "tuple.hh"
#include "optional.hh"
namespace redshift {

        inline tuple<Vector,Vector,Vector> coordinateSystem (const Normal &normal_) {
                using std::fabs; using std::sqrt;

                const Vector normal (vector_cast<Vector> (normal_));
                Vector v2;
                if (fabs (normal.x) > fabs (normal.y)) {
                        const real_t invLen = 1.f / sqrtf(normal.x*normal.x + normal.z*normal.z);
                        v2 = Vector (-normal.z*invLen, 0.f, normal.x*invLen);
                } else {
                        const real_t invLen = 1.f / sqrtf(normal.y*normal.y + normal.z*normal.z);
                        v2 = Vector (0.f, normal.z*invLen, -normal.y*invLen);
                }
                const Vector v3 (cross (normal, v2));
                return make_tuple (v2, normal, v3);
        }

        inline Vector reflect (const Vector &in, const Normal &n) {
                const real_t f = dot(vector_cast<Vector>(n),in) * 2.f;
                return in - vector_cast<Vector>(n*f);
        }


        // The R postfix is for "rejection sampled"
        inline tuple<real_t,real_t> uniformDiskR (Random &rand) {
                real_t x,y;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                } while (x*x+y*y > 1);
                return make_tuple(x,y);
        }
        inline tuple<real_t,real_t,real_t> uniformSphereR (Random &rand) {
                real_t x,y,z;
                do {
                        x = 2*rand()-1;
                        y = 2*rand()-1;
                        z = 2*rand()-1;
                } while (x*x+y*y+z*z > 1);
                return make_tuple(x,y,z);
        }
        inline tuple<real_t,real_t,real_t> cosineHemisphereR (Random &rand) {
                const tuple<real_t,real_t> r2 = uniformDiskR(rand);
                const real_t x = get<0>(r2);
                const real_t z = get<1>(r2);
                return make_tuple(x, std::sqrt(1-x*x-z*z), z);
        }
}

#include "../../skylab/include/preetham.hh"

#endif // SETUP_HH_INCLUDED_20090303
