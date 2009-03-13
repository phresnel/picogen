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

#include <kallisto/common.hh>

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
        typedef kallisto::fixed_point_t<int64_t,16>          fixed_point_t;        

        // Cartesian Types
        typedef kallisto::Point<kallisto::CARTESIAN,
                                fixed_point_t>               CartesianPoint;
        typedef kallisto::Normal<kallisto::CARTESIAN,real_t> CartesianNormal;
        typedef kallisto::Vector<kallisto::CARTESIAN,real_t> CartesianVector;


        // Std-Types
        typedef CartesianPoint  Point;
        typedef CartesianNormal Normal;
        typedef CartesianVector Vector;

        typedef kallisto::Sphere<Point,long double>  Sphere;
        typedef kallisto::Ray<Point,Vector>          Ray;
        typedef kallisto::RayDifferential<Point,Vector> RayDifferential;
        typedef kallisto::OrthonormalBasis<Vector>   OrthonormalBasis;
        
}

#include "basictypes/rgb.hh"
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
                
                extern real_t const epsilon;
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

#endif // SETUP_HH_INCLUDED_20090303
