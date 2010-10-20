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

#include "navigation.hh"

namespace cosyscene {


const actuarius::Enum<Navigation::Kind> Navigation::Typenames =
( actuarius::Nvp<Navigation::Kind>(Navigation::YawPitchRoll, "yawpitchroll")
| actuarius::Nvp<Navigation::Kind>(Navigation::None, "none")
);



Navigation::Navigation() : kind_(None) {
}



Navigation::Kind Navigation::kind() const {
        return kind_;
}



void Navigation::reset() {
        kind_ = None;
}



void Navigation::toYawPitchRoll(cosyscene::YawPitchRoll const &ypr) {
        yawPitchRoll_ = ypr;
        kind_ = YawPitchRoll;
}



cosyscene::YawPitchRoll Navigation::yawPitchRoll() const {
        return yawPitchRoll_;
}




bool Navigation::data_equals(Navigation const &rhs) const {
        if (kind_ != rhs.kind_) return false;
        switch (kind_)  {
        case YawPitchRoll: return yawPitchRoll_ == rhs.yawPitchRoll_;
        case None: return true;
        }
        return true;
}


}
