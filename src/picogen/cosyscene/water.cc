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

#include "water.hh"
#include "material.hh"
#include "quatsch-preprocessor/parsing.hh"
#include "cosyfloat.hh"

namespace picogen { namespace cosyscene {



WaterFitting::WaterFitting()
: seaLevel_(0)
{
}
void WaterFitting::setSeaLevel(CosyFloat s) {
        seaLevel_ = s;
}
CosyFloat WaterFitting::seaLevel() const {
        return seaLevel_;
}
bool WaterFitting::data_equals(WaterFitting const &rhs) const {
        return seaLevel_ == rhs.seaLevel_;
}


// Water
Water::Water()
: formation_(new TerrainFormation())
, material_(new Material())
, fitting_(new WaterFitting())
{
}
redshift::shared_ptr<TerrainFormation> Water::formation() const {
        return formation_;
}
redshift::shared_ptr<Material> Water::material() const {
        return material_;
}
redshift::shared_ptr<WaterFitting> Water::fitting() const {
        return fitting_;
}


} }
