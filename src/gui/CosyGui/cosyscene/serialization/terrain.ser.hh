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


#ifndef TERRAIN_SERIALIZATION_HH_20100902
#define TERRAIN_SERIALIZATION_HH_20100902

#include "../terrain.hh"
#include "cosyscene/serialization/stash.ser.hh"
#include "cosyscene/serialization/color.ser.hh"
#include "cosyscene/serialization/vector3d.ser.hh"

namespace cosyscene {

template<typename Arch>
inline void QuatschSource::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack("code", code_);
}

template <typename Arch>
inline void QuatschPreset::serialize(Arch &arch) {
        using actuarius::pack;
        arch & pack("preset", preset_);

        std::vector<StringKeyValue> repls;
        if (Arch::serialize) {
                typedef std::map<std::string, std::string>::const_iterator iter;
                for (iter it=replacements_.begin(), end=replacements_.end();
                        it!=end; ++it)
                {
                        repls.push_back(StringKeyValue(it->first,
                                                       it->second));
                }
        }

        arch & pack ("replacement", repls);

        if (Arch::deserialize) {
                typedef std::vector<StringKeyValue>::const_iterator iter;
                for (iter it=repls.begin(), end=repls.end(); it!=end; ++it) {
                        replacements_[it->key] = it->value;
                }
        }
}
template <typename Arch>
inline void StringKeyValue::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack("key", key) & pack("value", value);
}

template<typename Arch>
inline void TerrainFormation::serialize (Arch &arch) {
        using actuarius::pack;
        if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);

        arch & pack("type", kind_, Typenames);

        switch (kind_) {
        case QuatschSource: arch & pack ("parameters", quatschSource_); break;
        case QuatschPreset: arch & pack ("parameters", quatschPreset_); break;
        case None: break;
        }
}

template<typename Arch>
inline void Material::serialize (Arch &arch) {
        using actuarius::pack;
        if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);

        arch & pack("type", kind_, Typenames);

        switch (kind_) {
        case Monochrome: arch & pack ("parameters", monochrome_); break;
        case None: break;
        }
}

template<typename Arch>
inline void TerrainFitting::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack("size-factor", sizeFactor_)
             & pack("position-offset", positionOffset_)
             & pack("lazy-quadtree-visible-extent", lazyQuadtreeVisibleExtent_)
             & pack("lazy-quadtree-max-recursion", lazyQuadtreeMaxRecursion_)
             ;

}

template<typename Arch>
inline void Terrain::serialize (Arch &arch) {
        using actuarius::pack;

        /*if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);*/

        arch & pack("formation", *formation_);
        arch & pack("material", *material_);
        arch & pack("world-fitting", *fitting_);
}

} // namespace cosyscene

#endif // TERRAIN_SERIALIZATION_HH
