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

#include "sunsky.hh"

namespace cosyscene {


const actuarius::Enum<SunSky::Kind> SunSky::Typenames =
( actuarius::Nvp<SunSky::Kind>(SunSky::UtahSky, "utah")
| actuarius::Nvp<SunSky::Kind>(SunSky::None, "none")
);



SunSky::SunSky() : kind_(None) {
}



SunSky::Kind SunSky::kind() const {
        return kind_;
}



void SunSky::reset() {
        kind_ = None;
}



void SunSky::toUtahSky(cosyscene::UtahSky const &qs) {
        utahSky_ = qs;
        kind_ = UtahSky;
}



cosyscene::UtahSky SunSky::utahSky() const {
        return utahSky_;
}



bool SunSky::data_equals(SunSky const &rhs) const {
        if (kind_ != rhs.kind_) return false;
        switch (kind_)  {
        case UtahSky: return utahSky_ == rhs.utahSky_;
        case None: return true;
        }
        return true;
}


}
