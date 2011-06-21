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

#include "direction3d.hh"
#include "geometry.hh"

#include <cmath>

namespace picogen { namespace cosyscene {


Direction3d::Direction3d() : x_(0), y_(0), z_(0) {
}



Direction3d::Direction3d(CosyFloat x, CosyFloat y, CosyFloat z) {
        const CosyFloat l = CosyFloat(1) / length(x,y,z);
        x_ = x * l;
        y_ = y * l;
        z_ = z * l;
}



CosyFloat Direction3d::x() const {
        return x_;
}



CosyFloat Direction3d::y() const {
        return y_;
}



CosyFloat Direction3d::z() const {
        return z_;
}


} }
