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

#include "terrain.hh"

namespace cosyscene {


const actuarius::Enum<Terrain::Kind> Terrain::Typenames =
( actuarius::Nvp<Terrain::Kind>(Terrain::QuatschSource, "quatsch-source")
| actuarius::Nvp<Terrain::Kind>(Terrain::None, "none")
);



Terrain::Terrain() : kind_(None) {
}



Terrain::Kind Terrain::kind() const {
        return kind_;
}



void Terrain::reset() {
        kind_ = None;
}



void Terrain::toQuatschSource (cosyscene::QuatschSource const &qs) {
        quatschSource_ = qs;
        kind_ = QuatschSource;
}



cosyscene::QuatschSource Terrain::quatschSource() const {
        return quatschSource_;
}



bool Terrain::data_equals(Terrain const &rhs) const {
        if (kind_ != rhs.kind_) return false;
        switch (kind_)  {
        case QuatschSource: return quatschSource_ == rhs.quatschSource_;
        case None: return true;
        }
        return true;
}


}
