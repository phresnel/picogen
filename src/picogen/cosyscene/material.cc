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

#include "material.hh"

namespace picogen { namespace cosyscene {

const actuarius::Enum<Material::Kind> Material::Typenames =
( actuarius::Nvp<Material::Kind>(Material::None, "none")
| actuarius::Nvp<Material::Kind>(Material::Monochrome, "monochrome")
);



Material::Material() {
        // TODO: Once we use more advanced materials, "none" should be default
        // and the material editor shall show a kind-selection
        // (as with terrain-formation)
        Color tmp;
        tmp.toRgb(cosyscene::Rgb(1,1,1));
        toMonochrome(tmp);
}



Material::Kind Material::kind() const {
        return kind_;
}



Color Material::monochrome() const {
        return monochrome_;
}



void Material::toMonochrome(Color const &val) {
        kind_ = Monochrome;
        monochrome_ = val;
}



void Material::reset() {
        kind_ = None;
}



bool Material::data_equals(const Material &rhs) const {
        if (kind_ != rhs.kind_) return false;

        switch (kind_) {
        case None: return true;
        case Monochrome: return monochrome_ == rhs.monochrome_;
        }
        throw std::runtime_error("TerrainMaterial::data_equals() not "
                                 "fully defined");
}

} }
