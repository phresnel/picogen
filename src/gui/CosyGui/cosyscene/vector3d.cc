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

#include "vector3d.hh"
#include "geometry.hh"

#include <cmath>

namespace cosyscene {



Vector3d::Vector3d() : x_(0), y_(0), z_(0) {
}



Vector3d::Vector3d(CosyFloat x, CosyFloat y, CosyFloat z) : x_(x), y_(y), z_(z) {
}



CosyFloat Vector3d::x() const {
        return x_;
}



CosyFloat Vector3d::y() const {
        return y_;
}



CosyFloat Vector3d::z() const {
        return z_;
}



CosyFloat length (Vector3d d) {
        return length (d.x(), d.y(), d.z());
}

}
