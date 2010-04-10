//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef REDSHIFT_KALLISTO_20100325
#define REDSHIFT_KALLISTO_20100325

///////////////////////////////////////////////////////////////////////////////
// Assimilate kallisto-types and define some convenience types.
///////////////////////////////////////////////////////////////////////////////

#include "../../kallisto/common.hh"

namespace redshift {

        enum {
                ALLOW_LOGSPAM = 1
        };

        typedef double real_t;
        //typedef kallisto::fixed_point_t<int64_t,16>          fixed_point_t;
        typedef real_t fixed_point_t; //<-- lasttime I checked, floats where
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
        typedef kallisto::Plane<real_t> Plane;

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
        using kallisto::distance;
        using kallisto::is_above;
        using kallisto::does_intersect;
        using kallisto::vector_cast;
        using kallisto::scalar_cast;

        using kallisto::array;
}


#endif // REDSHIFT_KALLISTO_20100325
