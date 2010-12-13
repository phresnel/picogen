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
#include "quatsch-preprocessor/parsing.hh"

namespace cosyscene {


const actuarius::Enum<TerrainFormation::Kind> TerrainFormation::Typenames =
( actuarius::Nvp<TerrainFormation::Kind>(TerrainFormation::QuatschSource, "quatsch-source")
| actuarius::Nvp<TerrainFormation::Kind>(TerrainFormation::QuatschPreset, "quatsch-preset")
| actuarius::Nvp<TerrainFormation::Kind>(TerrainFormation::None, "none")
);

const actuarius::Enum<TerrainMaterial::Kind> TerrainMaterial::Typenames =
( actuarius::Nvp<TerrainMaterial::Kind>(TerrainMaterial::None, "none")
| actuarius::Nvp<TerrainMaterial::Kind>(TerrainMaterial::Monochrome, "monochrome")
);


std::string QuatschPreset::getPreprocessedCode() const {
        return quatsch_preprocessor::replace(preset_, replacements_);
}
void QuatschPreset::setPreset (std::string const &preset) {
        preset_ = preset;
}
std::string QuatschPreset::preset () const {
        return preset_;
}
void QuatschPreset::setReplacement (
        std::string const &key, std::string const &value
) {
        replacements_[key] = value;
}
std::string QuatschPreset::replacement (std::string const &key) const {
        return replacements_.find(key)->second;
}
std::map<std::string, std::string> QuatschPreset::replacements() const {
        return replacements_;
}



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



void TerrainFormation::toQuatschPreset (cosyscene::QuatschPreset const &qs) {
        quatschPreset_ = qs;
        kind_ = QuatschPreset;
}



cosyscene::QuatschPreset TerrainFormation::quatschPreset() const {
        return quatschPreset_;
}



bool TerrainFormation::data_equals(TerrainFormation const &rhs) const {
        if (kind_ != rhs.kind_) return false;
        switch (kind_)  {
        case QuatschSource: return quatschSource_ == rhs.quatschSource_;
        case QuatschPreset: return quatschPreset_ == rhs.quatschPreset_;
        case None: return true;
        }
        throw std::runtime_error("TerrainFormation::data_equals() not "
                                 "fully defined");
}




TerrainMaterial::TerrainMaterial() : kind_(None) {
}



TerrainMaterial::Kind TerrainMaterial::kind() const {
        return kind_;
}



Color TerrainMaterial::monochrome() const {
        return monochrome_;
}



void TerrainMaterial::toMonochrome(Color const &val) {
        kind_ = Monochrome;
        monochrome_ = val;
}



void TerrainMaterial::reset() {
        kind_ = None;
}



bool TerrainMaterial::data_equals(const TerrainMaterial &rhs) const {
        if (kind_ != rhs.kind_) return false;

        switch (kind_) {
        case None: return true;
        case Monochrome: return monochrome_ == rhs.monochrome_;
        }
        throw std::runtime_error("TerrainMaterial::data_equals() not "
                                 "fully defined");
}



TerrainFitting::TerrainFitting()
: sizeFactor_(1,1,1), positionOffset_(0,0,0)
, lazyQuadtreeMaxRecursion_(8), lazyQuadtreeVisibleExtent_(10000)
{
}
void TerrainFitting::setSizeFactor(Vector3d v) {
        sizeFactor_ = v;
}
Vector3d TerrainFitting::sizeFactor() const {
        return sizeFactor_;
}
void TerrainFitting::setPositionOffset(Vector3d v) {
        positionOffset_ = v;
}
Vector3d TerrainFitting::positionOffset() const {
        return positionOffset_;
}
void TerrainFitting::setLazyQuadtreeMaxRecursion (unsigned int rec) {
        lazyQuadtreeMaxRecursion_ = rec;
}
unsigned int TerrainFitting::lazyQuadtreeMaxRecursion() const {
        return lazyQuadtreeMaxRecursion_;
}
void TerrainFitting::setLazyQuadtreeVisibleExtent (double s) {
        lazyQuadtreeVisibleExtent_ = s;
}
double TerrainFitting::lazyQuadtreeVisibleExtent() const {
        return lazyQuadtreeVisibleExtent_;
}


// Terrain
Terrain::Terrain()
: formation_(new TerrainFormation())
, material_(new TerrainMaterial())
, fitting_(new TerrainFitting())
{
}
redshift::shared_ptr<TerrainFormation> Terrain::formation() const {
        return formation_;
}
redshift::shared_ptr<TerrainMaterial> Terrain::material() const {
        return material_;
}
redshift::shared_ptr<TerrainFitting> Terrain::fitting() const {
        return fitting_;
}


}
