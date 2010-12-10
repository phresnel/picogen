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


#ifndef TERRAIN_HH_20100902
#define TERRAIN_HH_20100902

#include "stash.hh"
#include <string>
#include <actuarius/bits/enum.hh>

namespace cosyscene {
class QuatschSource;
class TerrainFormation;

class QuatschSource {
public:
        QuatschSource() {}
        QuatschSource(std::string const &code) : code_(code) {}

        void setCode (std::string const &code) { code_ = code; }
        std::string code () const { return code_; }

        template<typename Arch>
        void serialize (Arch &arch);
private:
        std::string code_;
};
inline bool operator == (QuatschSource const &lhs, QuatschSource const &rhs) {
        return lhs.code() == rhs.code();
}

} // namespace cosyscene



namespace cosyscene {
struct StringKeyValue {
        std::string key, value;

        StringKeyValue() {}
        StringKeyValue(std::string const &key, std::string const &value)
                : key(key), value(value)
        {}

        template <typename Arch>
        void serialize (Arch &arch);
};
class QuatschPreset {
public:
        QuatschPreset() {}
        QuatschPreset(std::string const &preset) : preset_(preset) {}

        void setPreset (std::string const &preset);
        std::string preset () const;

        void setReplacement (std::string const &key, std::string const &value);
        std::string replacement (std::string const &key) const;
        std::map<std::string, std::string> replacements() const;

        std::string getPreprocessedCode() const;

        template<typename Arch>
        void serialize (Arch &arch);
private:
        std::string preset_;
        std::map<std::string, std::string> replacements_;
};
inline bool operator == (QuatschPreset const &lhs, QuatschPreset const &rhs) {
        return true;
}

} // namespace cosyscene



namespace cosyscene {
class TerrainFormation : public StashableMixin<TerrainFormation>
{
public:
        enum Kind {
                None,
                QuatschSource,
                QuatschPreset
        };

        TerrainFormation();

        Kind kind() const;
        void reset ();

        void toQuatschSource (cosyscene::QuatschSource const &qs);
        cosyscene::QuatschSource quatschSource() const;

        void toQuatschPreset(cosyscene::QuatschPreset const &);
        cosyscene::QuatschPreset quatschPreset() const;

        bool data_equals(TerrainFormation const &rhs) const;

        template<typename Arch>
        void serialize (Arch &arch);
        static const actuarius::Enum<Kind> Typenames;

private:
        Kind kind_;

        cosyscene::QuatschSource quatschSource_;
        cosyscene::QuatschPreset quatschPreset_;
};

} // namespace cosyscene



#include "color.hh"
namespace cosyscene {
class TerrainMaterial : public StashableMixin<TerrainMaterial> {
public:
        enum Kind {
                None,
                Monochrome
        };

        TerrainMaterial();

        Kind kind() const;
        Color monochrome() const;
        void reset();

        void toMonochrome(Color const &);

        bool data_equals(TerrainMaterial const &rhs) const;

        template<typename Arch>
        void serialize (Arch &arch);
        static const actuarius::Enum<Kind> Typenames;

private:
        Kind kind_;
        Color monochrome_;

};
} // namespace cosyscene



#include "vector3d.hh"
namespace cosyscene {
class WorldFitting
{
public:
        WorldFitting();

        void setSizeFactor (Vector3d);
        Vector3d sizeFactor() const;

        void setPositionOffset (Vector3d);
        Vector3d positionOffset() const;

        template <typename Arch> void serialize (Arch &arch);
private:
        Vector3d sizeFactor_, positionOffset_;
};
} // namespace cosyscene


#include "redshift/include/smart_ptr.hh"
namespace cosyscene {

class Terrain
{
public:
        Terrain() ;

        redshift::shared_ptr<TerrainFormation> formation() const;
        redshift::shared_ptr<TerrainMaterial>  material() const;
        redshift::shared_ptr<WorldFitting> fitting() const;

        template<typename Arch>
        void serialize (Arch &arch);

private:
        redshift::shared_ptr<TerrainFormation> formation_;
        redshift::shared_ptr<TerrainMaterial> material_;
        redshift::shared_ptr<WorldFitting> fitting_;
};

} // namespace cosyscene

#endif // TERRAIN_HH_20100902
