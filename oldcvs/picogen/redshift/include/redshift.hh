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

#ifndef REDSHIFT_H_INCLUDED_20090223
#define REDSHIFT_H_INCLUDED_20090223

#include <kallisto/common.hh>

namespace redshift {
        typedef float real_t;
        typedef kallisto::fixed_point_t<int64_t,16> fixed_point_t;
        //typedef real_t fixed_point_t;
        //typedef real_t radius_t;

        // Cartesian Types
        typedef kallisto::Point<kallisto::CARTESIAN,fixed_point_t> CartesianPoint;
        typedef kallisto::Normal<kallisto::CARTESIAN,real_t> CartesianNormal;
        typedef kallisto::Vector<kallisto::CARTESIAN,real_t> CartesianVector;


        // Std-Types
        typedef CartesianPoint Point;
        typedef CartesianNormal Normal;
        typedef CartesianVector Vector;

        typedef kallisto::Sphere<Point,real_t>     Sphere;
        typedef kallisto::Ray<Point,Vector>        Ray;
        typedef kallisto::OrthonormalBasis<Vector> OrthonormalBasis;

        struct IScene {};
}


namespace redshift {
        struct Rgb {
                real_t r, g, b;
        };

        typedef Rgb Color;

        template <typename T> struct is_rgb
        : public kallisto::traits::false_value {};
        template <> struct is_rgb<Rgb>
        : public kallisto::traits::true_value {};
}

namespace redshift {
        class IncomingRay {
                Ray const ray;
        };

        struct OutgoingRay {
                Ray const ray;
                Color const color;
                real_t intensity;
                real_t probability;
        };
}

#include "material.hh"

#endif // REDSHIFT_H_INCLUDED_20090223
