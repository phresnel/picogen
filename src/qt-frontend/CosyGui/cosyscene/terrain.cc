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


const actuarius::Enum<TerrainFormation::Kind> TerrainFormation::Typenames =
( actuarius::Nvp<TerrainFormation::Kind>(TerrainFormation::QuatschSource, "quatsch-source")
| actuarius::Nvp<TerrainFormation::Kind>(TerrainFormation::None, "none")
);



TerrainFormation::TerrainFormation() : kind_(None) {
}



TerrainFormation::Kind TerrainFormation::kind() const {
        return kind_;
}



void TerrainFormation::reset() {
        kind_ = None;
}



void TerrainFormation::toQuatschSource (cosyscene::QuatschSource const &qs) {
        quatschSource_ = qs;
        kind_ = QuatschSource;
}



cosyscene::QuatschSource TerrainFormation::quatschSource() const {
        return quatschSource_;
}



bool TerrainFormation::data_equals(TerrainFormation const &rhs) const {
        if (kind_ != rhs.kind_) return false;
        switch (kind_)  {
        case QuatschSource: return quatschSource_ == rhs.quatschSource_;
        case None: return true;
        }
        return true;
}


// Terrain
Terrain::Terrain()
: formation_(new TerrainFormation())
{
}
redshift::shared_ptr<TerrainFormation> Terrain::formation() const {
        return formation_;
}
bool Terrain::data_equals(Terrain const &rhs) const {
        return formation_->data_equals(*rhs.formation_);
}


}
